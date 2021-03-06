#ifndef _ACF_PROCESS_APU_E1AP_MODULE
#define _ACF_PROCESS_APU_E1AP_MODULE

#include <acf_process_apu.h>
#include <E1AP_MODULE_APU_LOAD.h> //APU load associated with this process
#include <stdint.h>


//SCENARIO LIST*************************************************
static acf_scenario_buffer_data gScenarioBufferData0_E1AP_MODULE[] = {{2, 1, 1, 0, {0, 0, 0, 0}}, {12, 1, 1, 0, {0, 0, 0, 0}}, {2, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 2, 0, {0, 0, 0, 0}}, {64, 1, 2, 0, {0, 0, 0, 0}}, {1, 1, 2, 0, {0, 0, 0, 0}}, {32, 1, 1, 0, {0, 0, 0, 0}}};

static acf_scenario_kernel_data gScenarioKernelData0_E1AP_MODULE[] = {{0, 0}};

static acf_scenario gScenarioArray_E1AP_MODULE[] = {
{0, 0, 140, 192, 0, gScenarioBufferData0_E1AP_MODULE, 112, gScenarioKernelData0_E1AP_MODULE, 4}
};

static acf_scenario_list gScenarioList_E1AP_MODULE = {
1, //number of scenarios
gScenarioArray_E1AP_MODULE};
//**************************************************************

class E1AP_MODULE : public ACF_Process_APU
{

public:
   E1AP_MODULE(int32_t apex_id = 0) : ACF_Process_APU(apex_id)
   {} 

   int32_t Initialize()
   {
      int32_t lRetVal = 0;

      if (!Initialized()) //initialization steps that only need to occur once
      {
         SetProcessIdentifier("E1AP_MODULE");
         SetApuLoadInfo(E1AP_MODULE_LOAD_SEGMENTS,
                        E1AP_MODULE_LOAD_PMEM, E1AP_MODULE_LOAD_PMEM_SIZE,
                        E1AP_MODULE_LOAD_DMEM, E1AP_MODULE_LOAD_DMEM_SIZE,
                        0, 0); //assuming _LOAD_CMEM does not exist

         lRetVal = NonParametricInitialization();
         if (lRetVal != 0) return lRetVal;

         //***NOTE: the order in which the following ports are added is meaningful; do not change!

         AddPort("E1AP_MODULE_INPUT_IMAGE", icp::DATATYPE_08S, 1, 1, 2, 1, 0, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0);
         AddPort("E1AP_MODULE_INPUT_WEIGHT", icp::DATATYPE_08S, 1, 1, 64, 1, 0, 0, 0, 0, 1, 0, 4, 0, 0, 0, 0, 0);
         AddPort("E1AP_MODULE_INPUT_ACCM", icp::DATATYPE_16S, 1, 1, 1, 1, 0, 0, 0, 0, 1, 0, 5, 0, 0, 0, 0, 0);
         AddPort("E1AP_MODULE_INPUT_PARAMS", icp::DATATYPE_32S, 1, 1, 32, 1, 0, 1, 1, 0, 1, 0, 6, 0, 0, 0, 0, 0);
         AddPort("E1AP_MODULE_OUTPUT", icp::DATATYPE_16S, 1, 1, 1, 1, 1, 0, 0, 0, 1, 0, 3, 0, 0, 0, 0, 0);

         CfgScenarios(&gScenarioList_E1AP_MODULE);

         FlagAsInitialized();
      }

      lRetVal = SelectApuConfiguration(mApuCfg, mApexId); //by default mApuCfg = ACF_APU_CFG__APEX0_DEFAULT and mApexId = 0

      return lRetVal;
   }
};

#endif //_ACF_PROCESS_APU_E1AP_MODULE
