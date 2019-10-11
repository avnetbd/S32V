#ifndef _ACF_PROCESS_APU_APU_K_MEANS
#define _ACF_PROCESS_APU_APU_K_MEANS

#include <acf_process_apu.h>
#include <APU_K_MEANS_APU_LOAD.h> //APU load associated with this process
#include <stdint.h>


//SCENARIO LIST*************************************************
static acf_scenario_buffer_data gScenarioBufferData0_APU_K_MEANS[] = {{20, 20, 2, 0, {0, 0, 0, 0}}, {20, 20, 2, 0, {0, 0, 0, 0}}, {3, 3, 2, 0, {0, 0, 0, 0}}, {3, 3, 2, 0, {0, 0, 0, 0}}};

static acf_scenario_kernel_data gScenarioKernelData0_APU_K_MEANS[] = {{0, 0}};

static acf_scenario gScenarioArray_APU_K_MEANS[] = {
{0, 0, 2472, 0, 0, gScenarioBufferData0_APU_K_MEANS, 64, gScenarioKernelData0_APU_K_MEANS, 4}
};

static acf_scenario_list gScenarioList_APU_K_MEANS = {
1, //number of scenarios
gScenarioArray_APU_K_MEANS};
//**************************************************************

class APU_K_MEANS : public ACF_Process_APU
{

public:
   APU_K_MEANS(int32_t apex_id = 0) : ACF_Process_APU(apex_id)
   {} 

   int32_t Initialize()
   {
      int32_t lRetVal = 0;

      if (!Initialized()) //initialization steps that only need to occur once
      {
         SetProcessIdentifier("APU_K_MEANS");
         SetApuLoadInfo(APU_K_MEANS_LOAD_SEGMENTS,
                        APU_K_MEANS_LOAD_PMEM, APU_K_MEANS_LOAD_PMEM_SIZE,
                        APU_K_MEANS_LOAD_DMEM, APU_K_MEANS_LOAD_DMEM_SIZE,
                        0, 0); //assuming _LOAD_CMEM does not exist

         lRetVal = NonParametricInitialization();
         if (lRetVal != 0) return lRetVal;

         //***NOTE: the order in which the following ports are added is meaningful; do not change!

         AddPort("INPUT_0", icp::DATATYPE_08U, 1, 1, 20, 20, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0);
         AddPort("INPUT_1", icp::DATATYPE_16U, 1, 1, 3, 3, 0, 0, 0, 0, 1, 0, 2, 0, 0, 0, 0, 0);
         AddPort("INPUT_2", icp::DATATYPE_16U, 1, 1, 3, 3, 0, 0, 0, 0, 1, 0, 3, 0, 0, 0, 0, 0);
         AddPort("OUTPUT_0", icp::DATATYPE_16U, 1, 1, 20, 20, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0);

         CfgScenarios(&gScenarioList_APU_K_MEANS);

         FlagAsInitialized();
      }

      lRetVal = SelectApuConfiguration(mApuCfg, mApexId); //by default mApuCfg = ACF_APU_CFG__APEX0_DEFAULT and mApexId = 0

      return lRetVal;
   }
};

#endif //_ACF_PROCESS_APU_APU_K_MEANS
