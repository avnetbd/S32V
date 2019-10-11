#ifndef _ACF_PROCESS_APU_APU_REMAP_BM
#define _ACF_PROCESS_APU_APU_REMAP_BM

#include <acf_process_apu.h>
#include <APU_REMAP_BM_APU_LOAD.h> //APU load associated with this process
#include <stdint.h>


//SCENARIO LIST*************************************************
static acf_scenario_buffer_data gScenarioBufferData0_APU_REMAP_BM[] = {{20, 4, 2, 0, {0, 0, 0, 0}}, {20, 4, 2, 0, {0, 0, 0, 0}}, {20, 4, 2, 0, {0, 0, 0, 0}}};

static acf_scenario_kernel_data gScenarioKernelData0_APU_REMAP_BM[] = {{0, 0}};

static acf_scenario gScenarioArray_APU_REMAP_BM[] = {
{0, 0, 640, 0, 0, gScenarioBufferData0_APU_REMAP_BM, 48, gScenarioKernelData0_APU_REMAP_BM, 4}
};

static acf_scenario_list gScenarioList_APU_REMAP_BM = {
1, //number of scenarios
gScenarioArray_APU_REMAP_BM};
//**************************************************************

class APU_REMAP_BM : public ACF_Process_APU
{

public:
   APU_REMAP_BM(int32_t apex_id = 0) : ACF_Process_APU(apex_id)
   {} 

   int32_t Initialize()
   {
      int32_t lRetVal = 0;

      if (!Initialized()) //initialization steps that only need to occur once
      {
         SetProcessIdentifier("APU_REMAP_BM");
         SetApuLoadInfo(APU_REMAP_BM_LOAD_SEGMENTS,
                        APU_REMAP_BM_LOAD_PMEM, APU_REMAP_BM_LOAD_PMEM_SIZE,
                        APU_REMAP_BM_LOAD_DMEM, APU_REMAP_BM_LOAD_DMEM_SIZE,
                        0, 0); //assuming _LOAD_CMEM does not exist

         lRetVal = NonParametricInitialization();
         if (lRetVal != 0) return lRetVal;

         //***NOTE: the order in which the following ports are added is meaningful; do not change!

         AddPort("INPUT", icp::DATATYPE_08U, 1, 1, 20, 4, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0);
         AddPort("OFFSET", icp::DATATYPE_16U, 1, 1, 20, 4, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0);
         AddPort("OUTPUT", icp::DATATYPE_08U, 1, 1, 20, 4, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0);

         CfgScenarios(&gScenarioList_APU_REMAP_BM);

         FlagAsInitialized();
      }

      lRetVal = SelectApuConfiguration(mApuCfg, mApexId); //by default mApuCfg = ACF_APU_CFG__APEX0_DEFAULT and mApexId = 0

      return lRetVal;
   }
};

#endif //_ACF_PROCESS_APU_APU_REMAP_BM
