#ifndef _ACF_PROCESS_APU_HISTORY_MAXIMUM_NATIVE_KERNEL_WRAPPER
#define _ACF_PROCESS_APU_HISTORY_MAXIMUM_NATIVE_KERNEL_WRAPPER

#include <HISTORY_MAXIMUM.hpp>

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

acf_io_info MYKERNEL_args[11];

where:
   [0] corresponds to input 'SRC_0'
   [1] corresponds to input 'SRC_1'
   [2] corresponds to input 'SRC_2'
   [3] corresponds to input 'SRC_3'
   [4] corresponds to input 'SRC_4'
   [5] corresponds to input 'SRC_5'
   [6] corresponds to input 'SRC_6'
   [7] corresponds to input 'SRC_7'
   [8] corresponds to input 'SRC_8'
   [9] corresponds to input 'SRC_9'
   [10] corresponds to output 'DST_IMG'

and 'pArgs = (void*)MYKERNEL_args;'
******************************************************/

static void HISTORY_MAXIMUM_nativekernel(void* pArgs)
{
   acf_io_info* lpIo = 0;

   //SRC_0
   ACF_DataDesc SRC_0;
   lpIo = (acf_io_info*)((int32_t)pArgs+0*sizeof(acf_io_info));
   SRC_0.Init2D(0, (void*)lpIo->pBuf, lpIo->width, lpIo->height, lpIo->span, lpIo->elementDataType, lpIo->elementDimX, lpIo->elementDimY);

   //SRC_1
   ACF_DataDesc SRC_1;
   lpIo = (acf_io_info*)((int32_t)pArgs+1*sizeof(acf_io_info));
   SRC_1.Init2D(0, (void*)lpIo->pBuf, lpIo->width, lpIo->height, lpIo->span, lpIo->elementDataType, lpIo->elementDimX, lpIo->elementDimY);

   //SRC_2
   ACF_DataDesc SRC_2;
   lpIo = (acf_io_info*)((int32_t)pArgs+2*sizeof(acf_io_info));
   SRC_2.Init2D(0, (void*)lpIo->pBuf, lpIo->width, lpIo->height, lpIo->span, lpIo->elementDataType, lpIo->elementDimX, lpIo->elementDimY);

   //SRC_3
   ACF_DataDesc SRC_3;
   lpIo = (acf_io_info*)((int32_t)pArgs+3*sizeof(acf_io_info));
   SRC_3.Init2D(0, (void*)lpIo->pBuf, lpIo->width, lpIo->height, lpIo->span, lpIo->elementDataType, lpIo->elementDimX, lpIo->elementDimY);

   //SRC_4
   ACF_DataDesc SRC_4;
   lpIo = (acf_io_info*)((int32_t)pArgs+4*sizeof(acf_io_info));
   SRC_4.Init2D(0, (void*)lpIo->pBuf, lpIo->width, lpIo->height, lpIo->span, lpIo->elementDataType, lpIo->elementDimX, lpIo->elementDimY);

   //SRC_5
   ACF_DataDesc SRC_5;
   lpIo = (acf_io_info*)((int32_t)pArgs+5*sizeof(acf_io_info));
   SRC_5.Init2D(0, (void*)lpIo->pBuf, lpIo->width, lpIo->height, lpIo->span, lpIo->elementDataType, lpIo->elementDimX, lpIo->elementDimY);

   //SRC_6
   ACF_DataDesc SRC_6;
   lpIo = (acf_io_info*)((int32_t)pArgs+6*sizeof(acf_io_info));
   SRC_6.Init2D(0, (void*)lpIo->pBuf, lpIo->width, lpIo->height, lpIo->span, lpIo->elementDataType, lpIo->elementDimX, lpIo->elementDimY);

   //SRC_7
   ACF_DataDesc SRC_7;
   lpIo = (acf_io_info*)((int32_t)pArgs+7*sizeof(acf_io_info));
   SRC_7.Init2D(0, (void*)lpIo->pBuf, lpIo->width, lpIo->height, lpIo->span, lpIo->elementDataType, lpIo->elementDimX, lpIo->elementDimY);

   //SRC_8
   ACF_DataDesc SRC_8;
   lpIo = (acf_io_info*)((int32_t)pArgs+8*sizeof(acf_io_info));
   SRC_8.Init2D(0, (void*)lpIo->pBuf, lpIo->width, lpIo->height, lpIo->span, lpIo->elementDataType, lpIo->elementDimX, lpIo->elementDimY);

   //SRC_9
   ACF_DataDesc SRC_9;
   lpIo = (acf_io_info*)((int32_t)pArgs+9*sizeof(acf_io_info));
   SRC_9.Init2D(0, (void*)lpIo->pBuf, lpIo->width, lpIo->height, lpIo->span, lpIo->elementDataType, lpIo->elementDimX, lpIo->elementDimY);

   //DST_IMG
   ACF_DataDesc DST_IMG;
   lpIo = (acf_io_info*)((int32_t)pArgs+10*sizeof(acf_io_info));
   DST_IMG.Init2D(0, (void*)lpIo->pBuf, lpIo->width, lpIo->height, lpIo->span, lpIo->elementDataType, lpIo->elementDimX, lpIo->elementDimY);

   HISTORY_MAXIMUM lProcess;
   lProcess.Initialize();
   lProcess.ConnectIO("SRC_0", SRC_0);
   lProcess.ConnectIO("SRC_1", SRC_1);
   lProcess.ConnectIO("SRC_2", SRC_2);
   lProcess.ConnectIO("SRC_3", SRC_3);
   lProcess.ConnectIO("SRC_4", SRC_4);
   lProcess.ConnectIO("SRC_5", SRC_5);
   lProcess.ConnectIO("SRC_6", SRC_6);
   lProcess.ConnectIO("SRC_7", SRC_7);
   lProcess.ConnectIO("SRC_8", SRC_8);
   lProcess.ConnectIO("SRC_9", SRC_9);
   lProcess.ConnectIO("DST_IMG", DST_IMG);
   lProcess.Start();
   lProcess.Wait();
}

#endif //_ACF_PROCESS_APU_HISTORY_MAXIMUM_NATIVE_KERNEL_WRAPPER
