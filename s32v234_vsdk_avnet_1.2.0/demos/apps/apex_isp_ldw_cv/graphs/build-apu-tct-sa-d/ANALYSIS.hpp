#ifndef _ACF_PROCESS_APU_ANALYSIS
#define _ACF_PROCESS_APU_ANALYSIS

#include <acf_process_apu.h>
#include <ANALYSIS_APU_LOAD.h> //APU load associated with this process
#include <stdint.h>


//SCENARIO LIST*************************************************
static acf_scenario_buffer_data gScenarioBufferData0_ANALYSIS[] = {{4, 10, 2, 0, {0, 0, 0, 0}}, {4, 1, 1, 0, {0, 0, 0, 0}}, {192, 1, 1, 0, {0, 0, 0, 0}}, {192, 1, 1, 0, {0, 0, 0, 0}}, {4, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};

static acf_scenario_kernel_data gScenarioKernelData0_ANALYSIS[] = {{0, 0}, {0, 0}};

static acf_scenario gScenarioArray_ANALYSIS[] = {
{0, 0, 112, 1584, 0, gScenarioBufferData0_ANALYSIS, 128, gScenarioKernelData0_ANALYSIS, 8}
};

static acf_scenario_list gScenarioList_ANALYSIS = {
1, //number of scenarios
gScenarioArray_ANALYSIS};
//**************************************************************

class ANALYSIS : public ACF_Process_APU
{

public:
   ANALYSIS(int32_t apex_id = 0) : ACF_Process_APU(apex_id)
   {} 

   int32_t Initialize()
   {
      int32_t lRetVal = 0;

      if (!Initialized()) //initialization steps that only need to occur once
      {
         SetProcessIdentifier("ANALYSIS");
         SetApuLoadInfo(ANALYSIS_LOAD_SEGMENTS,
                        ANALYSIS_LOAD_PMEM, ANALYSIS_LOAD_PMEM_SIZE,
                        ANALYSIS_LOAD_DMEM, ANALYSIS_LOAD_DMEM_SIZE,
                        0, 0); //assuming _LOAD_CMEM does not exist

         lRetVal = NonParametricInitialization();
         if (lRetVal != 0) return lRetVal;

         //***NOTE: the order in which the following ports are added is meaningful; do not change!

         AddPort("InputBirdView", icp::DATATYPE_08U, 1, 1, 4, 10, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0);
         AddPort("Prior_down", icp::DATATYPE_32U, 1, 1, 1, 1, 0, 1, 1, 0, 1, 0, 5, 0, 0, 0, 0, 0);
         AddPort("Prior_up", icp::DATATYPE_32U, 1, 1, 1, 1, 0, 1, 1, 0, 1, 0, 6, 0, 0, 0, 0, 0);
         AddPort("Start_tile", icp::DATATYPE_32U, 1, 1, 1, 1, 0, 1, 1, 0, 1, 0, 7, 0, 0, 0, 0, 0);
         AddPort("Columns_sum_down", icp::DATATYPE_32U, 1, 1, 192, 1, 1, 1, 1, 0, 1, 0, 2, 0, 0, 0, 0, 0);
         AddPort("Columns_sum_up", icp::DATATYPE_32U, 1, 1, 192, 1, 1, 1, 1, 0, 1, 0, 3, 0, 0, 0, 0, 0);

         CfgScenarios(&gScenarioList_ANALYSIS);

         FlagAsInitialized();
      }

      lRetVal = SelectApuConfiguration(mApuCfg, mApexId); //by default mApuCfg = ACF_APU_CFG__APEX0_DEFAULT and mApexId = 0

      return lRetVal;
   }
};

#endif //_ACF_PROCESS_APU_ANALYSIS
