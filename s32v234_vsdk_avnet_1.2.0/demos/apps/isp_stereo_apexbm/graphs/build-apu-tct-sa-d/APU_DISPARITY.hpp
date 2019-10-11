#ifndef _ACF_PROCESS_APU_APU_DISPARITY
#define _ACF_PROCESS_APU_APU_DISPARITY

#include <acf_process_apu.h>
#include <APU_DISPARITY_APU_LOAD.h> //APU load associated with this process
#include <stdint.h>


//SCENARIO LIST*************************************************
static acf_scenario_buffer_data gScenarioBufferData0_APU_DISPARITY[] = {{64, 2, 4, 0, {1, 1, 2, 2}}, {64, 2, 4, 1, {1, 1, 2, 2}}, {64, 2, 2, 2, {0, 0, 0, 0}}, {64, 2, 4, 0, {1, 1, 2, 2}}, {64, 2, 4, 1, {1, 2, 64, 2}}};

static acf_scenario_kernel_data gScenarioKernelData0_APU_DISPARITY[] = {{1, 0}, {1, 0}, {2, 0}};

static acf_scenario gScenarioArray_APU_DISPARITY[] = {
{0, 0, 3728, 0, 2, gScenarioBufferData0_APU_DISPARITY, 80, gScenarioKernelData0_APU_DISPARITY, 12}
};

static acf_scenario_list gScenarioList_APU_DISPARITY = {
1, //number of scenarios
gScenarioArray_APU_DISPARITY};
//**************************************************************

class APU_DISPARITY : public ACF_Process_APU
{

public:
   APU_DISPARITY(int32_t apex_id = 0) : ACF_Process_APU(apex_id)
   {} 

   int32_t Initialize()
   {
      int32_t lRetVal = 0;

      if (!Initialized()) //initialization steps that only need to occur once
      {
         SetProcessIdentifier("APU_DISPARITY");
         SetApuLoadInfo(APU_DISPARITY_LOAD_SEGMENTS,
                        APU_DISPARITY_LOAD_PMEM, APU_DISPARITY_LOAD_PMEM_SIZE,
                        APU_DISPARITY_LOAD_DMEM, APU_DISPARITY_LOAD_DMEM_SIZE,
                        0, 0); //assuming _LOAD_CMEM does not exist

         lRetVal = NonParametricInitialization();
         if (lRetVal != 0) return lRetVal;

         FlagSpatialDep();

         //***NOTE: the order in which the following ports are added is meaningful; do not change!

         AddPort("IN0", icp::DATATYPE_08U, 1, 1, 64, 2, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2, 2, 0);
         AddPort("IN1", icp::DATATYPE_08U, 1, 1, 64, 2, 0, 0, 0, 0, 0, 0, 3, 2, 3, 65, 2, 0);
         AddPort("OUT", icp::DATATYPE_08U, 1, 1, 64, 2, 1, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0);

         CfgScenarios(&gScenarioList_APU_DISPARITY);

         FlagAsInitialized();
      }

      lRetVal = SelectApuConfiguration(mApuCfg, mApexId); //by default mApuCfg = ACF_APU_CFG__APEX0_DEFAULT and mApexId = 0

      return lRetVal;
   }
};

#endif //_ACF_PROCESS_APU_APU_DISPARITY
