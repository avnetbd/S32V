#ifndef _ACF_PROCESS_APU_APU_GAUSS_5X5_NATIVE_KERNEL_WRAPPER
#define _ACF_PROCESS_APU_APU_GAUSS_5X5_NATIVE_KERNEL_WRAPPER

#include <APU_GAUSS_5X5.hpp>

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

acf_io_info MYKERNEL_args[2];

where:
   [0] corresponds to input 'Gauss5x5Graph_Input'
   [1] corresponds to output 'Gauss5x5Graph_Output'

and 'pArgs = (void*)MYKERNEL_args;'
******************************************************/

static void APU_GAUSS_5X5_nativekernel(void* pArgs)
{
   acf_io_info* lpIo = 0;

   //Gauss5x5Graph_Input
   ACF_DataDesc Gauss5x5Graph_Input;
   lpIo = (acf_io_info*)((int32_t)pArgs+0*sizeof(acf_io_info));
   Gauss5x5Graph_Input.Init2D(0, (void*)lpIo->pBuf, lpIo->width, lpIo->height, lpIo->span, lpIo->elementDataType, lpIo->elementDimX, lpIo->elementDimY);

   //Gauss5x5Graph_Output
   ACF_DataDesc Gauss5x5Graph_Output;
   lpIo = (acf_io_info*)((int32_t)pArgs+1*sizeof(acf_io_info));
   Gauss5x5Graph_Output.Init2D(0, (void*)lpIo->pBuf, lpIo->width, lpIo->height, lpIo->span, lpIo->elementDataType, lpIo->elementDimX, lpIo->elementDimY);

   APU_GAUSS_5X5 lProcess;
   lProcess.Initialize();
   lProcess.ConnectIO("Gauss5x5Graph_Input", Gauss5x5Graph_Input);
   lProcess.ConnectIO("Gauss5x5Graph_Output", Gauss5x5Graph_Output);
   lProcess.Start();
   lProcess.Wait();
}

#endif //_ACF_PROCESS_APU_APU_GAUSS_5X5_NATIVE_KERNEL_WRAPPER
