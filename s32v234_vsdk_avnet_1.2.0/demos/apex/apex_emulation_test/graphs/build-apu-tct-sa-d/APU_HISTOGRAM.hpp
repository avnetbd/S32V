#ifndef _ACF_PROCESS_APU_APU_HISTOGRAM
#define _ACF_PROCESS_APU_APU_HISTOGRAM

#include <acf_process_apu.h>
#include <APU_HISTOGRAM_APU_LOAD.h> //APU load associated with this process
#include <stdint.h>


//SCENARIO LIST*************************************************
static acf_scenario_buffer_data gScenarioBufferData0_APU_HISTOGRAM[] = {{8, 8, 2, 0, {0, 0, 0, 0}}, {256, 1, 1, 0, {0, 0, 0, 0}}, {256, 1, 1, 0, {0, 0, 0, 0}}};

static acf_scenario_kernel_data gScenarioKernelData0_APU_HISTOGRAM[] = {{0, 0}, {0, 0}};

static acf_scenario gScenarioArray_APU_HISTOGRAM[] = {
{0, 0, 1152, 1024, 0, gScenarioBufferData0_APU_HISTOGRAM, 48, gScenarioKernelData0_APU_HISTOGRAM, 8}
};

static acf_scenario_list gScenarioList_APU_HISTOGRAM = {
1, //number of scenarios
gScenarioArray_APU_HISTOGRAM};
//**************************************************************

class APU_HISTOGRAM : public ACF_Process_APU
{

public:
   APU_HISTOGRAM(int32_t apex_id = 0) : ACF_Process_APU(apex_id)
   {} 

   int32_t Initialize()
   {
      int32_t lRetVal = 0;

      if (!Initialized()) //initialization steps that only need to occur once
      {
         SetProcessIdentifier("APU_HISTOGRAM");
         SetApuLoadInfo(APU_HISTOGRAM_LOAD_SEGMENTS,
                        APU_HISTOGRAM_LOAD_PMEM, APU_HISTOGRAM_LOAD_PMEM_SIZE,
                        APU_HISTOGRAM_LOAD_DMEM, APU_HISTOGRAM_LOAD_DMEM_SIZE,
                        0, 0); //assuming _LOAD_CMEM does not exist

         lRetVal = NonParametricInitialization();
         if (lRetVal != 0) return lRetVal;

         //***NOTE: the order in which the following ports are added is meaningful; do not change!

         AddPort("INPUT_0", icp::DATATYPE_08U, 1, 1, 8, 8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
         AddPort("OUTPUT_0", icp::DATATYPE_32U, 1, 1, 256, 1, 1, 1, 1, 0, 1, 0, 2, 0, 0, 0, 0, 0);

         CfgScenarios(&gScenarioList_APU_HISTOGRAM);

         FlagAsInitialized();
      }

      lRetVal = SelectApuConfiguration(mApuCfg, mApexId); //by default mApuCfg = ACF_APU_CFG__APEX0_DEFAULT and mApexId = 0

      return lRetVal;
   }
};

#endif //_ACF_PROCESS_APU_APU_HISTOGRAM
