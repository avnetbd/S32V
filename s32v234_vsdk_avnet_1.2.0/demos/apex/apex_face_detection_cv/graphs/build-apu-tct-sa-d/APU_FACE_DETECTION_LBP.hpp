#ifndef _ACF_PROCESS_APU_APU_FACE_DETECTION_LBP
#define _ACF_PROCESS_APU_APU_FACE_DETECTION_LBP

#include <acf_process_apu.h>
#include <APU_FACE_DETECTION_LBP_APU_LOAD.h> //APU load associated with this process
#include <stdint.h>


//SCENARIO LIST*************************************************
static acf_scenario_buffer_data gScenarioBufferData0_APU_FACE_DETECTION_LBP[] = {{6, 1, 2, 0, {0, 0, 0, 0}}, {6, 1, 3, 0, {1, 0, 0, 1}}, {1, 1, 1, 1, {0, 0, 0, 0}}, {21, 32, 1, 1, {0, 0, 0, 0}}, {6, 1, 2, 1, {0, 0, 0, 0}}, {6, 1, 1, 0, {0, 0, 0, 0}}, {3, 1, 1, 0, {0, 0, 0, 0}}, {10000, 1, 1, 0, {0, 0, 0, 0}}, {200, 1, 1, 0, {0, 0, 0, 0}}, {64, 1, 1, 0, {0, 0, 0, 0}}, {64, 1, 1, 0, {0, 0, 0, 0}}};

static acf_scenario_kernel_data gScenarioKernelData0_APU_FACE_DETECTION_LBP[] = {{0, 0}, {1, 0}};

static acf_scenario gScenarioArray_APU_FACE_DETECTION_LBP[] = {
{0, 0, 2848, 10368, 1, gScenarioBufferData0_APU_FACE_DETECTION_LBP, 176, gScenarioKernelData0_APU_FACE_DETECTION_LBP, 8}
};

static acf_scenario_list gScenarioList_APU_FACE_DETECTION_LBP = {
1, //number of scenarios
gScenarioArray_APU_FACE_DETECTION_LBP};
//**************************************************************

class APU_FACE_DETECTION_LBP : public ACF_Process_APU
{

public:
   APU_FACE_DETECTION_LBP(int32_t apex_id = 0) : ACF_Process_APU(apex_id)
   {} 

   int32_t Initialize()
   {
      int32_t lRetVal = 0;

      if (!Initialized()) //initialization steps that only need to occur once
      {
         SetProcessIdentifier("APU_FACE_DETECTION_LBP");
         SetApuLoadInfo(APU_FACE_DETECTION_LBP_LOAD_SEGMENTS,
                        APU_FACE_DETECTION_LBP_LOAD_PMEM, APU_FACE_DETECTION_LBP_LOAD_PMEM_SIZE,
                        APU_FACE_DETECTION_LBP_LOAD_DMEM, APU_FACE_DETECTION_LBP_LOAD_DMEM_SIZE,
                        0, 0); //assuming _LOAD_CMEM does not exist

         lRetVal = NonParametricInitialization();
         if (lRetVal != 0) return lRetVal;

         FlagSpatialDep();

         //***NOTE: the order in which the following ports are added is meaningful; do not change!

         AddPort("INPUT_IMAGE", icp::DATATYPE_08U, 1, 1, 6, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0);
         AddPort("INPUT_CASCADE_SIZES_AND_SKIP", icp::DATATYPE_16U, 1, 1, 3, 1, 0, 1, 1, 0, 1, 0, 6, 0, 0, 0, 0, 0);
         AddPort("INPUT_CASCADE_FEATURES", icp::DATATYPE_08U, 1, 1, 10000, 1, 0, 1, 1, 0, 1, 0, 7, 0, 0, 0, 0, 0);
         AddPort("INPUT_CASCADE_STAGES", icp::DATATYPE_08U, 1, 1, 200, 1, 0, 1, 1, 0, 1, 0, 8, 0, 0, 0, 0, 0);
         AddPort("INPUT_PIXEL_SHIFTS", icp::DATATYPE_08U, 1, 1, 64, 1, 0, 1, 1, 0, 1, 0, 9, 0, 0, 0, 0, 0);
         AddPort("INPUT_PIXEL_OFFSETS", icp::DATATYPE_08U, 1, 1, 64, 1, 0, 1, 1, 0, 1, 0, 10, 0, 0, 0, 0, 0);
         AddPort("OUTPUT", icp::DATATYPE_08U, 1, 1, 6, 1, 1, 0, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0);

         CfgScenarios(&gScenarioList_APU_FACE_DETECTION_LBP);

         FlagAsInitialized();
      }

      lRetVal = SelectApuConfiguration(mApuCfg, mApexId); //by default mApuCfg = ACF_APU_CFG__APEX0_DEFAULT and mApexId = 0

      return lRetVal;
   }
};

#endif //_ACF_PROCESS_APU_APU_FACE_DETECTION_LBP
