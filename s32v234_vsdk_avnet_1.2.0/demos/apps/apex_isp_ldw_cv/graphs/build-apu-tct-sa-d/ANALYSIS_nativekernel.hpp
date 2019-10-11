#ifndef _ACF_PROCESS_APU_ANALYSIS_NATIVE_KERNEL_WRAPPER
#define _ACF_PROCESS_APU_ANALYSIS_NATIVE_KERNEL_WRAPPER

#include <ANALYSIS.hpp>

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

acf_io_info MYKERNEL_args[6];

where:
   [0] corresponds to input 'InputBirdView'
   [1] corresponds to input 'Prior_down'
   [2] corresponds to input 'Prior_up'
   [3] corresponds to input 'Start_tile'
   [4] corresponds to output 'Columns_sum_down'
   [5] corresponds to output 'Columns_sum_up'

and 'pArgs = (void*)MYKERNEL_args;'
******************************************************/

static void ANALYSIS_nativekernel(void* pArgs)
{
   acf_io_info* lpIo = 0;

   //InputBirdView
   ACF_DataDesc InputBirdView;
   lpIo = (acf_io_info*)((int32_t)pArgs+0*sizeof(acf_io_info));
   InputBirdView.Init2D(0, (void*)lpIo->pBuf, lpIo->width, lpIo->height, lpIo->span, lpIo->elementDataType, lpIo->elementDimX, lpIo->elementDimY);

   //Prior_down
   ACF_DataDesc Prior_down;
   lpIo = (acf_io_info*)((int32_t)pArgs+1*sizeof(acf_io_info));
   Prior_down.Init2D(0, (void*)lpIo->pBuf, lpIo->width, lpIo->height, lpIo->span, lpIo->elementDataType, lpIo->elementDimX, lpIo->elementDimY);

   //Prior_up
   ACF_DataDesc Prior_up;
   lpIo = (acf_io_info*)((int32_t)pArgs+2*sizeof(acf_io_info));
   Prior_up.Init2D(0, (void*)lpIo->pBuf, lpIo->width, lpIo->height, lpIo->span, lpIo->elementDataType, lpIo->elementDimX, lpIo->elementDimY);

   //Start_tile
   ACF_DataDesc Start_tile;
   lpIo = (acf_io_info*)((int32_t)pArgs+3*sizeof(acf_io_info));
   Start_tile.Init2D(0, (void*)lpIo->pBuf, lpIo->width, lpIo->height, lpIo->span, lpIo->elementDataType, lpIo->elementDimX, lpIo->elementDimY);

   //Columns_sum_down
   ACF_DataDesc Columns_sum_down;
   lpIo = (acf_io_info*)((int32_t)pArgs+4*sizeof(acf_io_info));
   Columns_sum_down.Init2D(0, (void*)lpIo->pBuf, lpIo->width, lpIo->height, lpIo->span, lpIo->elementDataType, lpIo->elementDimX, lpIo->elementDimY);

   //Columns_sum_up
   ACF_DataDesc Columns_sum_up;
   lpIo = (acf_io_info*)((int32_t)pArgs+5*sizeof(acf_io_info));
   Columns_sum_up.Init2D(0, (void*)lpIo->pBuf, lpIo->width, lpIo->height, lpIo->span, lpIo->elementDataType, lpIo->elementDimX, lpIo->elementDimY);

   ANALYSIS lProcess;
   lProcess.Initialize();
   lProcess.ConnectIO("InputBirdView", InputBirdView);
   lProcess.ConnectIO("Prior_down", Prior_down);
   lProcess.ConnectIO("Prior_up", Prior_up);
   lProcess.ConnectIO("Start_tile", Start_tile);
   lProcess.ConnectIO("Columns_sum_down", Columns_sum_down);
   lProcess.ConnectIO("Columns_sum_up", Columns_sum_up);
   lProcess.Start();
   lProcess.Wait();
}

#endif //_ACF_PROCESS_APU_ANALYSIS_NATIVE_KERNEL_WRAPPER
