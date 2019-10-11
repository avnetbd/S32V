#ifndef _ACF_PROCESS_APU_ACF_FILTERS_NATIVE_KERNEL_WRAPPER
#define _ACF_PROCESS_APU_ACF_FILTERS_NATIVE_KERNEL_WRAPPER

#include <ACF_FILTERS.hpp>

//structure encapsulating the information ACF requires for each I/O
typedef struct _acf_io_info {                                      
    int32_t      pBuf;   //CL framework will initialize this with a pointer to global memory (based on associated cl_mem entity)
    int32_t      width;  //width in unit of elements
    int32_t      height; //height in unit of elements
    int32_t      span;   //(2D only) the number of bytes required to jump from one line of bytes in memory to the 'next' line of bytes in memory
    icp::DATATYPE elementDataType; //element data type (icp::DATATYPE_08U, icp::DATATYPE_16S, etc.
    int32_t      elementDimX;     //element X dimension
    int32_t      elementDimY;     //element Y dimension
} acf_io_info;

/******************************************************
pArgs must point to an array with the following format:

acf_io_info MYKERNEL_args[8];

where:
   [0] corresponds to indirect input 'SRC_IMG' source
   [1] corresponds to indirect input 'SRC_IMG' offset
   [2] corresponds to input 'LOCAL_OFFSET'
   [3] corresponds to input 'DELTA'
   [4] corresponds to input 'THRESHOLD_PREVIOUS'
   [5] corresponds to input 'VOTE_THRESHOLD'
   [6] corresponds to output 'DST_IMG'
   [7] corresponds to output 'THRESHOLD'
   [8] corresponds to output 'LANE_DATA'

and 'pArgs = (void*)MYKERNEL_args;'
******************************************************/

static void ACF_FILTERS_nativekernel(void* pArgs)
{
   acf_io_info* lpIo = 0;

   //SRC_IMG
   ACF_DataDesc SRC_IMG_src;
   ACF_DataDesc SRC_IMG_offset;
   lpIo = (acf_io_info*)((int32_t)pArgs+0*sizeof(acf_io_info));
   SRC_IMG_src.Init2D(0, (void*)lpIo->pBuf, lpIo->width, lpIo->height, lpIo->span, lpIo->elementDataType, lpIo->elementDimX, lpIo->elementDimY);
   lpIo = (acf_io_info*)((int32_t)pArgs+1*sizeof(acf_io_info));
   SRC_IMG_offset.Init2D(0, (void*)lpIo->pBuf, lpIo->width, lpIo->height, lpIo->span, lpIo->elementDataType, lpIo->elementDimX, lpIo->elementDimY);

   //LOCAL_OFFSET
   ACF_DataDesc LOCAL_OFFSET;
   lpIo = (acf_io_info*)((int32_t)pArgs+2*sizeof(acf_io_info));
   LOCAL_OFFSET.Init2D(0, (void*)lpIo->pBuf, lpIo->width, lpIo->height, lpIo->span, lpIo->elementDataType, lpIo->elementDimX, lpIo->elementDimY);

   //DELTA
   ACF_DataDesc DELTA;
   lpIo = (acf_io_info*)((int32_t)pArgs+3*sizeof(acf_io_info));
   DELTA.Init2D(0, (void*)lpIo->pBuf, lpIo->width, lpIo->height, lpIo->span, lpIo->elementDataType, lpIo->elementDimX, lpIo->elementDimY);

   //THRESHOLD_PREVIOUS
   ACF_DataDesc THRESHOLD_PREVIOUS;
   lpIo = (acf_io_info*)((int32_t)pArgs+4*sizeof(acf_io_info));
   THRESHOLD_PREVIOUS.Init2D(0, (void*)lpIo->pBuf, lpIo->width, lpIo->height, lpIo->span, lpIo->elementDataType, lpIo->elementDimX, lpIo->elementDimY);

   //VOTE_THRESHOLD
   ACF_DataDesc VOTE_THRESHOLD;
   lpIo = (acf_io_info*)((int32_t)pArgs+5*sizeof(acf_io_info));
   VOTE_THRESHOLD.Init2D(0, (void*)lpIo->pBuf, lpIo->width, lpIo->height, lpIo->span, lpIo->elementDataType, lpIo->elementDimX, lpIo->elementDimY);

   //DST_IMG
   ACF_DataDesc DST_IMG;
   lpIo = (acf_io_info*)((int32_t)pArgs+6*sizeof(acf_io_info));
   DST_IMG.Init2D(0, (void*)lpIo->pBuf, lpIo->width, lpIo->height, lpIo->span, lpIo->elementDataType, lpIo->elementDimX, lpIo->elementDimY);

   //THRESHOLD
   ACF_DataDesc THRESHOLD;
   lpIo = (acf_io_info*)((int32_t)pArgs+7*sizeof(acf_io_info));
   THRESHOLD.Init2D(0, (void*)lpIo->pBuf, lpIo->width, lpIo->height, lpIo->span, lpIo->elementDataType, lpIo->elementDimX, lpIo->elementDimY);

   //LANE_DATA
   ACF_DataDesc LANE_DATA;
   lpIo = (acf_io_info*)((int32_t)pArgs+8*sizeof(acf_io_info));
   LANE_DATA.Init2D(0, (void*)lpIo->pBuf, lpIo->width, lpIo->height, lpIo->span, lpIo->elementDataType, lpIo->elementDimX, lpIo->elementDimY);

   ACF_FILTERS lProcess;
   lProcess.Initialize();
   lProcess.ConnectIndirectInput("SRC_IMG", SRC_IMG_src, SRC_IMG_offset);
   lProcess.ConnectIO("LOCAL_OFFSET", LOCAL_OFFSET);
   lProcess.ConnectIO("DELTA", DELTA);
   lProcess.ConnectIO("THRESHOLD_PREVIOUS", THRESHOLD_PREVIOUS);
   lProcess.ConnectIO("VOTE_THRESHOLD", VOTE_THRESHOLD);
   lProcess.ConnectIO("DST_IMG", DST_IMG);
   lProcess.ConnectIO("THRESHOLD", THRESHOLD);
   lProcess.ConnectIO("LANE_DATA", LANE_DATA);
   lProcess.Start();
   lProcess.Wait();
}

#endif //_ACF_PROCESS_APU_ACF_FILTERS_NATIVE_KERNEL_WRAPPER
