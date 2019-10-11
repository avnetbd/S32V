#ifndef _ACF_PROCESS_APU_APU_DISPARITY_NATIVE_KERNEL_WRAPPER
#define _ACF_PROCESS_APU_APU_DISPARITY_NATIVE_KERNEL_WRAPPER

#include <APU_DISPARITY.hpp>

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

acf_io_info MYKERNEL_args[3];

where:
   [0] corresponds to input 'IN0'
   [1] corresponds to input 'IN1'
   [2] corresponds to output 'OUT'

and 'pArgs = (void*)MYKERNEL_args;'
******************************************************/

static void APU_DISPARITY_nativekernel(void* pArgs)
{
   acf_io_info* lpIo = 0;

   //IN0
   ACF_DataDesc IN0;
   lpIo = (acf_io_info*)((int32_t)pArgs+0*sizeof(acf_io_info));
   IN0.Init2D(0, (void*)lpIo->pBuf, lpIo->width, lpIo->height, lpIo->span, lpIo->elementDataType, lpIo->elementDimX, lpIo->elementDimY);

   //IN1
   ACF_DataDesc IN1;
   lpIo = (acf_io_info*)((int32_t)pArgs+1*sizeof(acf_io_info));
   IN1.Init2D(0, (void*)lpIo->pBuf, lpIo->width, lpIo->height, lpIo->span, lpIo->elementDataType, lpIo->elementDimX, lpIo->elementDimY);

   //OUT
   ACF_DataDesc OUT;
   lpIo = (acf_io_info*)((int32_t)pArgs+2*sizeof(acf_io_info));
   OUT.Init2D(0, (void*)lpIo->pBuf, lpIo->width, lpIo->height, lpIo->span, lpIo->elementDataType, lpIo->elementDimX, lpIo->elementDimY);

   APU_DISPARITY lProcess;
   lProcess.Initialize();
   lProcess.ConnectIO("IN0", IN0);
   lProcess.ConnectIO("IN1", IN1);
   lProcess.ConnectIO("OUT", OUT);
   lProcess.Start();
   lProcess.Wait();
}

#endif //_ACF_PROCESS_APU_APU_DISPARITY_NATIVE_KERNEL_WRAPPER
