#include <icp_datatype.h>
#include <acf_common.h>
#include <acf_utilities.h>
#include <acf_hwutilities.h>
#include <acf_memmgmt.h>
#include <linked_list_utilities.h> //for linked list entry sizes
#include <mcdma_hal.h>
#include <mce_hal.h>
#include <seq_hal.h>
#include <stio_hal.h>
#include <cmem_if_hal.h>
#include <acf_cbmanager.hpp>
#include <acf_odprodpattern.hpp>
#include <util.h>

//global values that can be used by advanced kernels to perform init/reduction operations
int16_t gsACF_FIRST_TILE_FLAG;
int16_t gsACF_LAST_TILE_FLAG;
int16_t gsACF_TILE_WIDTH_IN_CHUNKS;
int16_t gsACF_FIRST_CUID;
int16_t gsACF_NUM_INPUT_ITERATIONS;
int16_t gsACF_CU_ARRAY_WIDTH;

//profiling stuff
#include <apex_642cl_map.h>
#include <apex_642cl_dmem_fifo_map_type.h>
#define RESET_PERF_REG (*((volatile unsigned int *)(APEX_642CL_DMEM_FIFO_0_PERFORMANCE)) = 0)
#define QUERY_PERF_REG (*((volatile unsigned int *)(APEX_642CL_DMEM_FIFO_0_PERFORMANCE)))
struct ApuPerfInfoStruct gPerfInfo;
int32_t gProbeStart    = 0;
int32_t gProbeA        = 0;
int32_t gProbeB        = 0;

//kernel function signatures
void apu_downsample(kernel_io_desc, kernel_io_desc);
void apu_upsample(kernel_io_desc, kernel_io_desc);

//global input info for DMA+STREAM from XMEM to CMEM (populated/initialized by the host)
acf_io_desc gAcfInputDescArray[1]; //[0]=INPUT 

//global output info for STREAM+DMA from CMEM to XMEM (populated/initialized by the host)
acf_io_desc gAcfOutputDescArray[2]; //[0]=OUTPUT_DOWN [1]=OUTPUT_UP 

//global linked list memory allocation (must be 128-bit aligned)
const int gInputLinkedListMemSize = LLE_SIZE_2D_DMA_XMEM_CMEM*1 + LLE_SIZE_2D_DMA_MEM_MEM*0;
DECLARE_BYTE_ALIGNED(int, gAcfInputLinkedListMem[gInputLinkedListMemSize*2], 16); //*2 for double buffering 
const int gOutputLinkedListMemSize = LLE_SIZE_2D_DMA_XMEM_CMEM*2 + LLE_SIZE_2D_DMA_MEM_MEM*0;
DECLARE_BYTE_ALIGNED(int, gAcfOutputLinkedListMem[gOutputLinkedListMemSize*2], 16); //*2 for double buffering 

//allocate global array of local memory descriptors
ACF_LocalMemDesc gLocalMemArray[3];

//allocate memory for linked list entry database(s)
acf_llentry_desc gInputLinkedListEntryDatabase[1];
acf_llentry_desc gOutputLinkedListEntryDatabase[2];

//the physical DMEM base address from the DMA's perspective (i.e. the host/fpga perspective)
extern int gsDmemBaseAddrForDma;

//allocate global array of kernel_io_desc
kernel_io_desc gAcfKernelIoDescArray[4];

//circular buffer index tracking (NOTE: circular buffer index tracking related to kernels is currently tied to kernel_io_desc)
int16_t gCbIdxArray_pad[3]; //tied to padding

//grouping of IOs into logical transfers (managing transfers with multiple DMA)
int8_t gNumLogicalTransfersCmemIn = 0;
int8_t gNumLogicalTransfersDmemIn = 0;
int8_t gNumLogicalTransfersCmemOut = 0;
int8_t gNumLogicalTransfersDmemOut = 0;
int8_t gNumLogicalTransfersCmemIn_MCE = 0;
struct transfer gLogicalTransfersCmemIn[1];
struct transfer* gLogicalTransfersDmemIn = 0;
struct transfer gLogicalTransfersCmemOut[2];
struct transfer* gLogicalTransfersDmemOut = 0;
struct transfer* gLogicalTransfersCmemIn_MCE = 0;

int gNumInputIterations = 0;
int gInputIter          = 0;
int gProcIter           = 0;
int gInputIterMce       = 0;

int APU_DOWNSAMPLE_UPSAMPLE(int* const lpParams, int const lNumParams)
{
   RESET_PERF_REG;

   gProbeStart = QUERY_PERF_REG;

   int lRetVal = ACF_SUCCESS;

   //---------------------------
   //unpack parameters from host
   //---------------------------
   int lNumInputIterations          = lpParams[0];
   int lInputTileWidthInChunks      = lpParams[1];
   int lCoreDataCuOffset            = lpParams[2];
   int lOutputTileWidthInChunks     = lpParams[3];
   int lNumOutputIterations         = lpParams[4];
   int lOutputSkip                  = lpParams[5]; //number of output tiles to skip (related to ROI processing)
   int lLastInputTileWidthInChunks  = lpParams[6];
   int lLastOutputTileWidthInChunks = lpParams[7];
   gsDmemBaseAddrForDma             = lpParams[8];
   int lCmemHeapBaseAddr            = lpParams[9];
   int lCmemHeapSizeInbytes         = lpParams[10];
   int lDmemHeapBaseAddr            = lpParams[11];
   int lDmemHeapSizeInBytes         = lpParams[12];
   int lClover389Flag               = lpParams[13];
   gNumInputIterations              = lNumInputIterations; //for debugging

   //only make last tile width adjustment if it differs from previous tiles
   int lLastInputTileWidthInChunks_tmp = (lLastInputTileWidthInChunks == lInputTileWidthInChunks) ? -1 : lLastInputTileWidthInChunks;
   int lLastOutputTileWidthInChunks_tmp = (lLastOutputTileWidthInChunks == lOutputTileWidthInChunks) ? -1 : lLastOutputTileWidthInChunks;

   acf_res_info* lpResInfo = (acf_res_info*)(&lpParams[14]); //HW resources

   memcpy_int(&lpParams[29], (int*)gAcfInputDescArray, 1*6);
   memcpy_int(&lpParams[35], (int*)gAcfOutputDescArray, 2*6);
   int lScenarioIterAdjust = lpParams[47];
   acf_scenario_buffer_data* lpScenarioBufferData = (acf_scenario_buffer_data*)&lpParams[48];
   acf_scenario_kernel_data* lpScenarioKernelData = (acf_scenario_kernel_data*)&lpParams[60];

   //***********************************************
   //initialization---------------------------------
   //***********************************************

   //sequencer is required for generating the 'final' interrupt that indicates to the host that execution has completed
   do
   {
   ACF_InitSeqRegPtr(); //initialize global static pointer to sequencer ONCE for use by subsequent ACF_ seq hal calls
   SEQ_ENABLE(lpResInfo->apuId, lpResInfo->procIdSeqRes);
   SEQ_INIT_RESOURCE(lpResInfo->zeroSeqRes_Done, ((1 << 16) - 1)); //initialize to -1

   gsACF_TILE_WIDTH_IN_CHUNKS = lInputTileWidthInChunks;
   gsACF_FIRST_CUID           = lCoreDataCuOffset;
   gsACF_NUM_INPUT_ITERATIONS = lNumInputIterations;

   int lApuCuOffset = 0; //0 if APU0, 32 if APU1 (this is for the stream dma 'start cu' field
   if (lpResInfo->apuId == 1) lApuCuOffset = 32;

   gsACF_CU_ARRAY_WIDTH = cmem_if_hal_query_cu_array_width(0, lpResInfo->apuId); //currently required for padding optimization

   if (ACF_OrgLogXfer_CmemOnly(lpResInfo->numDmaChIn, lpResInfo->numStreamInChForDma, 1, &gNumLogicalTransfersCmemIn, gLogicalTransfersCmemIn) != 0)
   {
      lRetVal = ACF_HW_RESOURCE_ERROR;
      break;//goto EARLY_EXIT;
   }

   if (ACF_OrgLogXfer_CmemOnly(lpResInfo->numDmaChOut, lpResInfo->numStreamOutChForDma, 2, &gNumLogicalTransfersCmemOut, gLogicalTransfersCmemOut) != 0)
   {
      lRetVal = ACF_HW_RESOURCE_ERROR;
      break;//goto EARLY_EXIT;
   }

   InitVecHeap((vec08u*)lCmemHeapBaseAddr, lCmemHeapSizeInbytes);
   InitSclHeap((int8_t*)lDmemHeapBaseAddr, lDmemHeapSizeInBytes);

   //calculate padding offset info (relevant for vector inputs where input size is not a nicely divisible by chunk width)
   int lPadOffsetRefChunkWidth = lpScenarioBufferData[0].chunkWidth;
   int lPadOffsetRefChunkHeight = lpScenarioBufferData[0].chunkHeight;
   int lTmp = gAcfInputDescArray[0].width % lPadOffsetRefChunkWidth;
   int lPadOffsetR = (lTmp == 0) ? (0) : (lPadOffsetRefChunkWidth - lTmp);
   lTmp = gAcfInputDescArray[0].height % lPadOffsetRefChunkHeight;
   int lPadOffsetB = (lTmp == 0) ? (0) : (lPadOffsetRefChunkHeight - lTmp);

   for (int i=0; i<3; i++)
      gCbIdxArray_pad[i] = 0;

   //initialize array of local memory descriptors
   int lLmError = 0;

   //Memory associated with the following ports:
   //Producer: graph port 'INPUT'
   //Consumer kernel 'downsample_0' port 'INPUT_0'
   //Consumer kernel 'upsample_0' port 'INPUT_0'
   lLmError += LmInit(&gLocalMemArray[0], &lpScenarioBufferData[0], icp::DATATYPE_08U, 1, 1, -1, lPadOffsetR, lPadOffsetRefChunkWidth, lPadOffsetB, lPadOffsetRefChunkHeight, 1, 0);

   //Memory associated with the following ports:
   //Producer: kernel 'downsample_0' port 'OUTPUT_0'
   //Consumer: graph port 'OUTPUT_DOWN'
   lLmError += LmInit(&gLocalMemArray[1], &lpScenarioBufferData[1], icp::DATATYPE_08U, 1, 1, -1, lPadOffsetR, lPadOffsetRefChunkWidth, lPadOffsetB, lPadOffsetRefChunkHeight, 1, 0);

   //Memory associated with the following ports:
   //Producer: kernel 'upsample_0' port 'OUTPUT_0'
   //Consumer: graph port 'OUTPUT_UP'
   lLmError += LmInit(&gLocalMemArray[2], &lpScenarioBufferData[2], icp::DATATYPE_08U, 1, 1, -1, lPadOffsetR, lPadOffsetRefChunkWidth, lPadOffsetB, lPadOffsetRefChunkHeight, 1, 0);

   if (lLmError != 0)
   {
      lRetVal = ACF_MEMORY_ALLOCATION_ERROR;
      break;//goto EARLY_EXIT;
   }

   //initialize global array of kernel_io_desc
   IoDescInit(&gAcfKernelIoDescArray[0], &gLocalMemArray[0], 0); //associated with kernel apu_downsample port INPUT_0
   IoDescInit(&gAcfKernelIoDescArray[1], &gLocalMemArray[1], 1); //associated with kernel apu_downsample port OUTPUT_0
   IoDescInit(&gAcfKernelIoDescArray[2], &gLocalMemArray[0], 0); //associated with kernel apu_upsample port INPUT_0
   IoDescInit(&gAcfKernelIoDescArray[3], &gLocalMemArray[2], 2); //associated with kernel apu_upsample port OUTPUT_0

   //allocate and initialize linked list buffer arrays (each LL is double buffered)
   int* lInputLLBufArray[2]  = {&gAcfInputLinkedListMem[0],  &gAcfInputLinkedListMem[gInputLinkedListMemSize]};
   int* lOutputLLBufArray[2] = {&gAcfOutputLinkedListMem[0], &gAcfOutputLinkedListMem[gOutputLinkedListMemSize]};

   //allocate and initialize linked list database entries (associates each ll entry with a lm desc and an io desc)
   gInputLinkedListEntryDatabase[0].pIoDesc = &gAcfInputDescArray[0];
   gInputLinkedListEntryDatabase[0].pLmDesc = &gLocalMemArray[0];
   gInputLinkedListEntryDatabase[0].size = LLE_SIZE_2D_DMA_XMEM_CMEM;
   gInputLinkedListEntryDatabase[0].pEntry[0] = lInputLLBufArray[0] + 0*LLE_SIZE_2D_DMA_XMEM_CMEM;
   gInputLinkedListEntryDatabase[0].pEntry[1] = lInputLLBufArray[1] + 0*LLE_SIZE_2D_DMA_XMEM_CMEM;
   gInputLinkedListEntryDatabase[0]._framework_cbindex = 0;

   gOutputLinkedListEntryDatabase[0].pIoDesc = &gAcfOutputDescArray[0];
   gOutputLinkedListEntryDatabase[0].pLmDesc = &gLocalMemArray[1];
   gOutputLinkedListEntryDatabase[0].size = LLE_SIZE_2D_DMA_XMEM_CMEM;
   gOutputLinkedListEntryDatabase[0].pEntry[0] = lOutputLLBufArray[0] + 0*LLE_SIZE_2D_DMA_XMEM_CMEM;
   gOutputLinkedListEntryDatabase[0].pEntry[1] = lOutputLLBufArray[1] + 0*LLE_SIZE_2D_DMA_XMEM_CMEM;
   gOutputLinkedListEntryDatabase[0]._framework_cbindex = lOutputSkip%gLocalMemArray[1].numBuffers;
   gOutputLinkedListEntryDatabase[1].pIoDesc = &gAcfOutputDescArray[1];
   gOutputLinkedListEntryDatabase[1].pLmDesc = &gLocalMemArray[2];
   gOutputLinkedListEntryDatabase[1].size = LLE_SIZE_2D_DMA_XMEM_CMEM;
   gOutputLinkedListEntryDatabase[1].pEntry[0] = lOutputLLBufArray[0] + 1*LLE_SIZE_2D_DMA_XMEM_CMEM;
   gOutputLinkedListEntryDatabase[1].pEntry[1] = lOutputLLBufArray[1] + 1*LLE_SIZE_2D_DMA_XMEM_CMEM;
   gOutputLinkedListEntryDatabase[1]._framework_cbindex = lOutputSkip%gLocalMemArray[2].numBuffers;


   //-----------------------
   //initialize linked lists
   //-----------------------

   int lLlOffset = 0;

   lLlOffset = 0;
   for (int i=0; i<1; i++)
   {
      ACF_LLE_2DTransferConfig_XMEM_CMEM(
                lInputLLBufArray[0] + lLlOffset,                        //linked list entry address
                gInputLinkedListEntryDatabase[i].pLmDesc->e0,           //e0
                gInputLinkedListEntryDatabase[i].pIoDesc->physAddr,     //source address
                RetStreamInPeriphId(lpResInfo->streamInChForDma[0]),    //destination address (***dummy - will be reconfigured)
                gInputLinkedListEntryDatabase[i].pIoDesc->span,         //span
                gInputLinkedListEntryDatabase[i].pIoDesc->width,        //width
                lInputTileWidthInChunks,                                //input tile width in chunks
                gInputLinkedListEntryDatabase[i].pLmDesc->chunkWidth,   //chunk width
                gInputLinkedListEntryDatabase[i].pLmDesc->bufferHeight, //chunk height
                gInputLinkedListEntryDatabase[i].pLmDesc->chunkSpan,    //chunk span
                0xBEEF,                                                 //cmem address (***dummy - will be reconfigured)
                lApuCuOffset);                                          //start CU
      lLlOffset += LLE_SIZE_2D_DMA_XMEM_CMEM;
   }
   memcpy_int(lInputLLBufArray[0], lInputLLBufArray[1], gInputLinkedListMemSize);

   lLlOffset = 0;
   for (int i=0; i<2; i++)
   {
      ACF_LLE_2DTransferConfig_XMEM_CMEM(
                lOutputLLBufArray[0] + lLlOffset,                        //linked list entry address
                gOutputLinkedListEntryDatabase[i].pLmDesc->e0,           //e0
                RetStreamOutPeriphId(lpResInfo->streamOutChForDma[0]),   //source address (***dummy - will be reconfigured)
                gOutputLinkedListEntryDatabase[i].pIoDesc->physAddr,     //destination address
                gOutputLinkedListEntryDatabase[i].pIoDesc->span,         //span
                gOutputLinkedListEntryDatabase[i].pIoDesc->width,        //width
                lOutputTileWidthInChunks,                                //output tile width in chunks
                gOutputLinkedListEntryDatabase[i].pLmDesc->chunkWidth,   //chunk width
                gOutputLinkedListEntryDatabase[i].pLmDesc->chunkHeight,  //chunk height
                gOutputLinkedListEntryDatabase[i].pLmDesc->chunkSpan,    //chunk span
                0xBEEF,                                                  //cmem address (***dummy - will be reconfigured)
                lApuCuOffset + lCoreDataCuOffset);                       //start CU
      lLlOffset += LLE_SIZE_2D_DMA_XMEM_CMEM;
   }
   memcpy_int(lOutputLLBufArray[0], lOutputLLBufArray[1], gOutputLinkedListMemSize);

   ConfigureLogicalTransfersDma(gNumLogicalTransfersCmemIn, gLogicalTransfersCmemIn, gInputLinkedListEntryDatabase,
                                &lpResInfo->dmaChIn[0], &lpResInfo->streamInChForDma[0], 0);
   ConfigureLogicalTransfersDma(gNumLogicalTransfersCmemOut, gLogicalTransfersCmemOut, gOutputLinkedListEntryDatabase,
                                &lpResInfo->dmaChOut[0], &lpResInfo->streamOutChForDma[0], 1);
   //local variables
   int lInputIter = 0;
   int lInputLLBufIndex = 0;
   int lInputTransferArray[1];
   int lInputDmaProcDecFlag = 1;
   int lOutputLLBufIndex = 0;
   int lOutputTransferArray[2];
   int lProcessingProcDecFlag = 1;
   int lProcIter = 0;
   int lLaunchDelay = 0;
   int lOutputDelay = 0;
   int lWriteDelay = 0;
   int lNumIterations = lNumInputIterations + lScenarioIterAdjust;
   int lLastFlag = 0;

   //----------------------------------------------------------
   //initialize sequencer resources
   //----------------------------------------------------------

   int lFixSeqResIdx = 0; //used to move through the lFixedSeqRes array 
   int lSEQ_RESOURCE_DMA_INPUT_DATABUF_AVAIL_DMA = lpResInfo->fixedSeqRes[lFixSeqResIdx++]; //incremented by ACP when processing has completed for an input iteration

   //-----dma + stream in-----
   for (int i=0; i<gNumLogicalTransfersCmemIn; i++)
   {
      gLogicalTransfersCmemIn[i].seqResDescAvailDma = lpResInfo->fixedSeqRes[lFixSeqResIdx++];
      gLogicalTransfersCmemIn[i].seqResBufAvailApu = lpResInfo->fixedSeqRes[lFixSeqResIdx++];
   }

   //-----stream out + dma-----
   for (int i=0; i<gNumLogicalTransfersCmemOut; i++)
   {
      gLogicalTransfersCmemOut[i].seqResDescAvailDma = lpResInfo->fixedSeqRes[lFixSeqResIdx++];
      gLogicalTransfersCmemOut[i].seqResBufAvailApu = lpResInfo->fixedSeqRes[lFixSeqResIdx++];
   }

   if (lFixSeqResIdx > lpResInfo->numFixedSeqRes)
   {
      lRetVal = ACF_HW_RESOURCE_ERROR;
      break;//goto EARLY_EXIT;
   }

   seq_hal_set_rsc_counter(0, lSEQ_RESOURCE_DMA_INPUT_DATABUF_AVAIL_DMA, 2);

   //-----dma + stream in-----
   for (int i=0; i<gNumLogicalTransfersCmemIn; i++)
   {
      seq_hal_set_dma_dec(0, lpResInfo->dmaChIn[i], (1ul << gLogicalTransfersCmemIn[i].seqResDescAvailDma));
      seq_hal_set_dma_inc(0, lpResInfo->dmaChIn[i], (1ul << gLogicalTransfersCmemIn[i].seqResBufAvailApu));
      seq_hal_set_rsc_counter(0, gLogicalTransfersCmemIn[i].seqResDescAvailDma, 0);
      seq_hal_set_rsc_counter(0, gLogicalTransfersCmemIn[i].seqResBufAvailApu,   0);
   }

   //-----stream out + dma-----
   for (int i=0; i<gNumLogicalTransfersCmemOut; i++)
   {
      seq_hal_set_dma_dec(0, lpResInfo->dmaChOut[i], (1ul << gLogicalTransfersCmemOut[i].seqResDescAvailDma));
      seq_hal_set_dma_inc(0, lpResInfo->dmaChOut[i], (1ul << gLogicalTransfersCmemOut[i].seqResBufAvailApu));
      seq_hal_set_rsc_counter(0, gLogicalTransfersCmemOut[i].seqResDescAvailDma, 0);
      seq_hal_set_rsc_counter(0, gLogicalTransfersCmemOut[i].seqResBufAvailApu, 2);
   }

   int lScbMask = 0;
   //-----dma + stream in-----
   for (int i=0; i<gNumLogicalTransfersCmemIn; i++)
   {
      lScbMask |= (1ul << lpResInfo->dmaChIn[i]);
   }
   //-----stream out + dma-----
   for (int i=0; i<gNumLogicalTransfersCmemOut; i++)
   {
      lScbMask |= (1ul << lpResInfo->dmaChOut[i]);
   }
   SEQ_ENABLE_SCB(lpResInfo->apuId, lScbMask);

   //ensure interrupts associated with procid lpResInfo->procIntSeqRes[0] (dma in), lpResInfo->procIntSeqRes[1] (mce in), and lpResInfo->procIntSeqRes[2] (processing & dma out)
   //are cleared and unmasked
   int lProcIntMask = ((1<<lpResInfo->procIntSeqRes[0]) | (1<<lpResInfo->procIntSeqRes[1]) | (1<<lpResInfo->procIntSeqRes[2]));
   seq_hal_clear_interrupts(0, lProcIntMask);
   seq_hal_mask_interrupts_apu(0, lpResInfo->apuId, ~lProcIntMask);

   gProbeA = QUERY_PERF_REG; //probe for end of 'initialization' section
   gPerfInfo.init = gProbeA - gProbeStart;

   //----------------------------------------------------------
   //main execution loop
   //----------------------------------------------------------

   while (lProcIter < lNumIterations || lInputIter < lNumIterations)
   {
      //interrupt associated with DMA input
      if ((lInputIter < lNumIterations) && (1 == lInputDmaProcDecFlag))
      {
         ACF_seq_hal_proc_dec_rscmask(lpResInfo->procIntSeqRes[0], 1<<lSEQ_RESOURCE_DMA_INPUT_DATABUF_AVAIL_DMA);
         lInputDmaProcDecFlag = 0;
      }

      //no 'iterative' MCE input ...

      //interrupt associated with processing and output
      if ((lProcIter < lNumIterations) && (1 == lProcessingProcDecFlag))
      {
         int lProcDecMask = 0;
         for (int i=0; i<gNumLogicalTransfersCmemIn; i++)
            lProcDecMask |= (1<<(gLogicalTransfersCmemIn[i].seqResBufAvailApu));
         for (int i=0; i<gNumLogicalTransfersCmemOut; i++)
            lProcDecMask |= (1<<(gLogicalTransfersCmemOut[i].seqResBufAvailApu));
         ACF_seq_hal_proc_dec_rscmask(lpResInfo->procIntSeqRes[2], lProcDecMask);
         lProcessingProcDecFlag = 0;
      }

      gProbeA = QUERY_PERF_REG; //probe for idle start

      wait(32768); //this will 'wait' until an interrupt occurs (or the timer runs out)

      gProbeB = QUERY_PERF_REG; //probe for idle end
      gPerfInfo.idle += gProbeB - gProbeA;

      uint32_t isrval = ACF_seq_hal_query_interrupts() & lProcIntMask; //capture state of relevant interrupts

      if (isrval) //if any of the relevant interrupts have occurred
      {
         ACF_seq_hal_clear_interrupts(isrval); //clear the interrupts that occurred

         //**********************************************
         //input (DMA)
         //**********************************************

         if (isrval & (0x1<<lpResInfo->procIntSeqRes[0])) //input DMA
         {
            lInputDmaProcDecFlag = 1;

            //determine input DMA transfers that will take place during iteration 'lInputIter'
            lLaunchDelay = lpScenarioBufferData[0].writeDelay; //input dma launch delay
            lInputTransferArray[0] = (lInputIter >= lLaunchDelay) && (lInputIter < (lLaunchDelay + lNumInputIterations)) ? (lInputIter - lLaunchDelay) : -1; //input transfer associated with port 'INPUT'
            lLastFlag = (lInputIter == lNumIterations-1) ? 1 : 0;

            //Generate DMA input linked list
            int* lpLinkedList = lInputLLBufArray[lInputLLBufIndex];

            ACF_AppendLLE_XMEM_CMEM_IN(lpLinkedList, lInputTransferArray, 1, gInputLinkedListEntryDatabase, lNumInputIterations, lLastInputTileWidthInChunks_tmp, LLE_SIZE_2D_DMA_XMEM_CMEM);
            lpLinkedList += 24;

            for (int i=0; i<gNumLogicalTransfersCmemIn; i++)
            {
               ACF_LleSetLast(gLogicalTransfersCmemIn[i].lastEntryPtr[lInputLLBufIndex], lLastFlag);
               ACF_seq_hal_proc_inc_rscmask(lpResInfo->procIdSeqRes, 1<<gLogicalTransfersCmemIn[i].seqResDescAvailDma); //kick transfer
            }

            lInputLLBufIndex = ((0 == lInputLLBufIndex) ? 1 : 0); //toggle 
            lInputIter ++;
            gInputIter = lInputIter; //for debugging
         }

         //**********************************************
         //processing and output
         //**********************************************

         if (isrval & (0x1<<lpResInfo->procIntSeqRes[2])) //processing and output
         {
            lProcessingProcDecFlag = 1;

            //<processing> -------------------------------------------------------------------

            gProbeA = QUERY_PERF_REG; //probe for processing start

            //apu_downsample
            lLaunchDelay = lpScenarioKernelData[0].executionStartDelay;
            lOutputDelay = lpScenarioKernelData[0].extraIterToSatisfyOd;
            if (lProcIter >= lLaunchDelay && lProcIter < (lNumInputIterations + lLaunchDelay + lOutputDelay))
            {
               int16_t lLocalIter = lProcIter - lLaunchDelay;
               ACF_UpdateTileFlags(lLocalIter, lNumInputIterations);
               apu_downsample(
                  *_RETIODESC_(&gLocalMemArray[0], &gAcfKernelIoDescArray[0]),
                  *_RETIODESC_(&gLocalMemArray[1], &gAcfKernelIoDescArray[1]));
            }

            //padding and circular buffer emulation for gLocalMemArray[0]
            lWriteDelay = lpScenarioBufferData[0].writeDelay;
            if (lProcIter >= lWriteDelay && lProcIter < (lWriteDelay + lNumInputIterations))
            {
               ACF_PAD_08_e0xis1_LequaltoR(&gLocalMemArray[0], gCbIdxArray_pad[0], lProcIter - lWriteDelay, lNumInputIterations, lInputTileWidthInChunks, 1, 1, 2, 2, 0, gLocalMemArray[0].padOffsetB, 0, gLocalMemArray[0].padOffsetR, gsACF_CU_ARRAY_WIDTH);
               ACF_COPYBUF(&gLocalMemArray[0], gCbIdxArray_pad[0], lProcIter - lWriteDelay, lNumInputIterations, 1, 1, 2);
               CbIndexIncrement(&gCbIdxArray_pad[0], gLocalMemArray[0].numBuffers);
            }

            //apu_upsample
            lLaunchDelay = lpScenarioKernelData[1].executionStartDelay;
            lOutputDelay = lpScenarioKernelData[1].extraIterToSatisfyOd;
            if (lProcIter >= lLaunchDelay && lProcIter < (lNumInputIterations + lLaunchDelay + lOutputDelay))
            {
               int16_t lLocalIter = lProcIter - lLaunchDelay;
               ACF_UpdateTileFlags(lLocalIter, lNumInputIterations);
               apu_upsample(
                  *_RETIODESC_(&gLocalMemArray[0], &gAcfKernelIoDescArray[2]),
                  *_RETIODESC_(&gLocalMemArray[2], &gAcfKernelIoDescArray[3]));
            }

            gProbeB = QUERY_PERF_REG; //probe for processing end
            gPerfInfo.processing += gProbeB - gProbeA;

            //</processing> ------------------------------------------------------------------

            ACF_seq_hal_proc_inc_rscmask(lpResInfo->procIdSeqRes, 1<<lSEQ_RESOURCE_DMA_INPUT_DATABUF_AVAIL_DMA);

            //<output> -----------------------------------------------------------------------

            //setup and kick output transfer associated with iteration 'lProcIter'; if there isn't one, increment resources to keep the loop going

            //determine output transfers that should be scheduled during iteration 'lProcIter' (if any)
            lLaunchDelay = lpScenarioBufferData[1].writeDelay; //dma launch delay
            lOutputTransferArray[0] = (lProcIter >= (lLaunchDelay + lOutputSkip) && lProcIter < (lLaunchDelay + lOutputSkip + lNumOutputIterations)) ? (lProcIter - lLaunchDelay) : -1; //output transfer associated with port 'OUTPUT_DOWN'
            lLaunchDelay = lpScenarioBufferData[2].writeDelay; //dma launch delay
            lOutputTransferArray[1] = (lProcIter >= (lLaunchDelay + lOutputSkip) && lProcIter < (lLaunchDelay + lOutputSkip + lNumOutputIterations)) ? (lProcIter - lLaunchDelay) : -1; //output transfer associated with port 'OUTPUT_UP'
            lLastFlag = (lProcIter == lNumIterations-1) ? 1 : 0;

            //Generate DMA output linked list
            int* lpLinkedList = lOutputLLBufArray[lOutputLLBufIndex];

            ACF_AppendLLE_XMEM_CMEM_OUT(lpLinkedList, lOutputTransferArray, 2, gOutputLinkedListEntryDatabase, -lOutputSkip, lNumOutputIterations, lLastOutputTileWidthInChunks_tmp, LLE_SIZE_2D_DMA_XMEM_CMEM);
            lpLinkedList += 48;

            for (int i=0; i<gNumLogicalTransfersCmemOut; i++)
            {
               ACF_LleSetLast(gLogicalTransfersCmemOut[i].lastEntryPtr[lOutputLLBufIndex], lLastFlag);
               ACF_seq_hal_proc_inc_rscmask(lpResInfo->procIdSeqRes, 1<<gLogicalTransfersCmemOut[i].seqResDescAvailDma); //kick transfer
            }

            lOutputLLBufIndex = ((0 == lOutputLLBufIndex) ? 1 : 0); //toggle 


            //</output> ----------------------------------------------------------------------

            lProcIter ++;
            gProcIter = lProcIter; //for debugging
         }
      }
   }

   //----------------------------------------------------------
   //wait for output transfers to complete and clean up
   //----------------------------------------------------------

   //wait for all output transfers to complete
   int lProcDecMask = 0;
   for (int i=0; i<gNumLogicalTransfersCmemOut; i++)
      lProcDecMask |= (1<<(gLogicalTransfersCmemOut[i].seqResBufAvailApu));
   ACF_seq_hal_proc_dec_rscmask(lpResInfo->procIntSeqRes[0], lProcDecMask);
   while(seq_hal_query_interrupt(0, lpResInfo->procIntSeqRes[0]) == 0)
   {
      gProbeA = QUERY_PERF_REG; //probe for idle start

      wait(32768);

      gProbeB = QUERY_PERF_REG; //probe for idle end
      gPerfInfo.idle += gProbeB - gProbeA;
   }
   seq_hal_clear_interrupt(0, lpResInfo->procIntSeqRes[0]);
   ACF_seq_hal_proc_dec_rscmask(lpResInfo->procIntSeqRes[0], lProcDecMask);
   while(seq_hal_query_interrupt(0, lpResInfo->procIntSeqRes[0]) == 0)
   {
      gProbeA = QUERY_PERF_REG; //probe for idle start

      wait(32768);

      gProbeB = QUERY_PERF_REG; //probe for idle end
      gPerfInfo.idle += gProbeB - gProbeA;
   }
   seq_hal_clear_interrupt(0, lpResInfo->procIntSeqRes[0]);

   SEQ_DISABLE_SCB(lpResInfo->apuId, lScbMask);

   for (int i=0; i<gNumLogicalTransfersCmemIn; i++)
   {
      mcdma_hal_interruptclear_ch_done(0, lpResInfo->dmaChIn[i]);
      stio_hal_interruptclear_sti_done(0, lpResInfo->streamInChForDma[i]);
   }
   for (int i=0; i<gNumLogicalTransfersCmemOut; i++)
   {
      mcdma_hal_interruptclear_ch_done(0, lpResInfo->dmaChOut[i]);
      stio_hal_interruptclear_sto_done(0, lpResInfo->streamOutChForDma[i]);
   }
   seq_hal_mask_interrupts_apu(0, lpResInfo->apuId, lProcIntMask);


   }
   while(false);

   //signal interrupt to indicate to host that the program has completed
   ACF_seq_hal_proc_inc_rscmask(lpResInfo->procIdSeqRes, 1<<lpResInfo->zeroSeqRes_Done);
   SEQ_DISABLE(lpResInfo->apuId, lpResInfo->procIdSeqRes);

   gProbeA = QUERY_PERF_REG; //probe for total
   gPerfInfo.total = gProbeA - gProbeStart;

   return lRetVal;
}

