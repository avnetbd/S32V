#ifndef _ACF_PROCESS_APU_APU_CONVOLUTION_NATIVE_KERNEL_WRAPPER
#define _ACF_PROCESS_APU_APU_CONVOLUTION_NATIVE_KERNEL_WRAPPER

#include <APU_CONVOLUTION.hpp>

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

acf_io_info MYKERNEL_args[5];

where:
   [0] corresponds to input 'ConvGraph_Input'
   [1] corresponds to input 'ConvGr_In_Filt'
   [2] corresponds to input 'ConvGraph_FiltScale'
   [3] corresponds to input 'ConvGr_FltSymmFl'
   [4] corresponds to output 'ConvGraph_Output'

and 'pArgs = (void*)MYKERNEL_args;'
******************************************************/

static void APU_CONVOLUTION_nativekernel(void* pArgs)
{
   acf_io_info* lpIo = 0;

   //ConvGraph_Input
   ACF_DataDesc ConvGraph_Input;
   lpIo = (acf_io_info*)((int32_t)pArgs+0*sizeof(acf_io_info));
   ConvGraph_Input.Init2D(0, (void*)lpIo->pBuf, lpIo->width, lpIo->height, lpIo->span, lpIo->elementDataType, lpIo->elementDimX, lpIo->elementDimY);

   //ConvGr_In_Filt
   ACF_DataDesc ConvGr_In_Filt;
   lpIo = (acf_io_info*)((int32_t)pArgs+1*sizeof(acf_io_info));
   ConvGr_In_Filt.Init2D(0, (void*)lpIo->pBuf, lpIo->width, lpIo->height, lpIo->span, lpIo->elementDataType, lpIo->elementDimX, lpIo->elementDimY);

   //ConvGraph_FiltScale
   ACF_DataDesc ConvGraph_FiltScale;
   lpIo = (acf_io_info*)((int32_t)pArgs+2*sizeof(acf_io_info));
   ConvGraph_FiltScale.Init2D(0, (void*)lpIo->pBuf, lpIo->width, lpIo->height, lpIo->span, lpIo->elementDataType, lpIo->elementDimX, lpIo->elementDimY);

   //ConvGr_FltSymmFl
   ACF_DataDesc ConvGr_FltSymmFl;
   lpIo = (acf_io_info*)((int32_t)pArgs+3*sizeof(acf_io_info));
   ConvGr_FltSymmFl.Init2D(0, (void*)lpIo->pBuf, lpIo->width, lpIo->height, lpIo->span, lpIo->elementDataType, lpIo->elementDimX, lpIo->elementDimY);

   //ConvGraph_Output
   ACF_DataDesc ConvGraph_Output;
   lpIo = (acf_io_info*)((int32_t)pArgs+4*sizeof(acf_io_info));
   ConvGraph_Output.Init2D(0, (void*)lpIo->pBuf, lpIo->width, lpIo->height, lpIo->span, lpIo->elementDataType, lpIo->elementDimX, lpIo->elementDimY);

   APU_CONVOLUTION lProcess;
   lProcess.Initialize();
   lProcess.ConnectIO("ConvGraph_Input", ConvGraph_Input);
   lProcess.ConnectIO("ConvGr_In_Filt", ConvGr_In_Filt);
   lProcess.ConnectIO("ConvGraph_FiltScale", ConvGraph_FiltScale);
   lProcess.ConnectIO("ConvGr_FltSymmFl", ConvGr_FltSymmFl);
   lProcess.ConnectIO("ConvGraph_Output", ConvGraph_Output);
   lProcess.Start();
   lProcess.Wait();
}

#endif //_ACF_PROCESS_APU_APU_CONVOLUTION_NATIVE_KERNEL_WRAPPER
