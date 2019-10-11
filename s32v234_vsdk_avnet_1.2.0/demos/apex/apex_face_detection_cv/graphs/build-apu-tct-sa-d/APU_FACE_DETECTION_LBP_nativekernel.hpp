#ifndef _ACF_PROCESS_APU_APU_FACE_DETECTION_LBP_NATIVE_KERNEL_WRAPPER
#define _ACF_PROCESS_APU_APU_FACE_DETECTION_LBP_NATIVE_KERNEL_WRAPPER

#include <APU_FACE_DETECTION_LBP.hpp>

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

acf_io_info MYKERNEL_args[7];

where:
   [0] corresponds to input 'INPUT_IMAGE'
   [1] corresponds to input 'INPUT_CASCADE_SIZES_AND_SKIP'
   [2] corresponds to input 'INPUT_CASCADE_FEATURES'
   [3] corresponds to input 'INPUT_CASCADE_STAGES'
   [4] corresponds to input 'INPUT_PIXEL_SHIFTS'
   [5] corresponds to input 'INPUT_PIXEL_OFFSETS'
   [6] corresponds to output 'OUTPUT'

and 'pArgs = (void*)MYKERNEL_args;'
******************************************************/

static void APU_FACE_DETECTION_LBP_nativekernel(void* pArgs)
{
   acf_io_info* lpIo = 0;

   //INPUT_IMAGE
   ACF_DataDesc INPUT_IMAGE;
   lpIo = (acf_io_info*)((int32_t)pArgs+0*sizeof(acf_io_info));
   INPUT_IMAGE.Init2D(0, (void*)lpIo->pBuf, lpIo->width, lpIo->height, lpIo->span, lpIo->elementDataType, lpIo->elementDimX, lpIo->elementDimY);

   //INPUT_CASCADE_SIZES_AND_SKIP
   ACF_DataDesc INPUT_CASCADE_SIZES_AND_SKIP;
   lpIo = (acf_io_info*)((int32_t)pArgs+1*sizeof(acf_io_info));
   INPUT_CASCADE_SIZES_AND_SKIP.Init2D(0, (void*)lpIo->pBuf, lpIo->width, lpIo->height, lpIo->span, lpIo->elementDataType, lpIo->elementDimX, lpIo->elementDimY);

   //INPUT_CASCADE_FEATURES
   ACF_DataDesc INPUT_CASCADE_FEATURES;
   lpIo = (acf_io_info*)((int32_t)pArgs+2*sizeof(acf_io_info));
   INPUT_CASCADE_FEATURES.Init2D(0, (void*)lpIo->pBuf, lpIo->width, lpIo->height, lpIo->span, lpIo->elementDataType, lpIo->elementDimX, lpIo->elementDimY);

   //INPUT_CASCADE_STAGES
   ACF_DataDesc INPUT_CASCADE_STAGES;
   lpIo = (acf_io_info*)((int32_t)pArgs+3*sizeof(acf_io_info));
   INPUT_CASCADE_STAGES.Init2D(0, (void*)lpIo->pBuf, lpIo->width, lpIo->height, lpIo->span, lpIo->elementDataType, lpIo->elementDimX, lpIo->elementDimY);

   //INPUT_PIXEL_SHIFTS
   ACF_DataDesc INPUT_PIXEL_SHIFTS;
   lpIo = (acf_io_info*)((int32_t)pArgs+4*sizeof(acf_io_info));
   INPUT_PIXEL_SHIFTS.Init2D(0, (void*)lpIo->pBuf, lpIo->width, lpIo->height, lpIo->span, lpIo->elementDataType, lpIo->elementDimX, lpIo->elementDimY);

   //INPUT_PIXEL_OFFSETS
   ACF_DataDesc INPUT_PIXEL_OFFSETS;
   lpIo = (acf_io_info*)((int32_t)pArgs+5*sizeof(acf_io_info));
   INPUT_PIXEL_OFFSETS.Init2D(0, (void*)lpIo->pBuf, lpIo->width, lpIo->height, lpIo->span, lpIo->elementDataType, lpIo->elementDimX, lpIo->elementDimY);

   //OUTPUT
   ACF_DataDesc OUTPUT;
   lpIo = (acf_io_info*)((int32_t)pArgs+6*sizeof(acf_io_info));
   OUTPUT.Init2D(0, (void*)lpIo->pBuf, lpIo->width, lpIo->height, lpIo->span, lpIo->elementDataType, lpIo->elementDimX, lpIo->elementDimY);

   APU_FACE_DETECTION_LBP lProcess;
   lProcess.Initialize();
   lProcess.ConnectIO("INPUT_IMAGE", INPUT_IMAGE);
   lProcess.ConnectIO("INPUT_CASCADE_SIZES_AND_SKIP", INPUT_CASCADE_SIZES_AND_SKIP);
   lProcess.ConnectIO("INPUT_CASCADE_FEATURES", INPUT_CASCADE_FEATURES);
   lProcess.ConnectIO("INPUT_CASCADE_STAGES", INPUT_CASCADE_STAGES);
   lProcess.ConnectIO("INPUT_PIXEL_SHIFTS", INPUT_PIXEL_SHIFTS);
   lProcess.ConnectIO("INPUT_PIXEL_OFFSETS", INPUT_PIXEL_OFFSETS);
   lProcess.ConnectIO("OUTPUT", OUTPUT);
   lProcess.Start();
   lProcess.Wait();
}

#endif //_ACF_PROCESS_APU_APU_FACE_DETECTION_LBP_NATIVE_KERNEL_WRAPPER
