#ifndef _ACF_PROCESS_APU_APU_GAUSS_3X3
#define _ACF_PROCESS_APU_APU_GAUSS_3X3

#include <acf_process_apu.h>
#include <APU_GAUSS_3X3_APU_LOAD.h> //APU load associated with this process
#include <stdint.h>


//SCENARIO LIST*************************************************
static acf_scenario_buffer_data gScenarioBufferData0_APU_GAUSS_3X3[] = {{8, 8, 4, 0, {1, 1, 2, 2}}, {8, 8, 2, 1, {0, 0, 0, 0}}};

static acf_scenario_kernel_data gScenarioKernelData0_APU_GAUSS_3X3[] = {{1, 0}};

static acf_scenario gScenarioArray_APU_GAUSS_3X3[] = {
{0, 0, 536, 0, 1, gScenarioBufferData0_APU_GAUSS_3X3, 32, gScenarioKernelData0_APU_GAUSS_3X3, 4}
};

static acf_scenario_list gScenarioList_APU_GAUSS_3X3 = {
1, //number of scenarios
gScenarioArray_APU_GAUSS_3X3};
//**************************************************************

class APU_GAUSS_3X3 : public ACF_Process_APU
{

public:
   APU_GAUSS_3X3(int32_t apex_id = 0) : ACF_Process_APU(apex_id)
   {} 

   int32_t Initialize()
   {
      int32_t lRetVal = 0;

      if (!Initialized()) //initialization steps that only need to occur once
      {
         SetProcessIdentifier("APU_GAUSS_3X3");
         SetApuLoadInfo(APU_GAUSS_3X3_LOAD_SEGMENTS,
                        APU_GAUSS_3X3_LOAD_PMEM, APU_GAUSS_3X3_LOAD_PMEM_SIZE,
                        APU_GAUSS_3X3_LOAD_DMEM, APU_GAUSS_3X3_LOAD_DMEM_SIZE,
                        0, 0); //assuming _LOAD_CMEM does not exist

         lRetVal = NonParametricInitialization();
         if (lRetVal != 0) return lRetVal;

         FlagSpatialDep();

         //***NOTE: the order in which the following ports are added is meaningful; do not change!

         AddPort("Gauss3x3Graph_Input", icp::DATATYPE_08U, 1, 1, 8, 8, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0);
         AddPort("Gauss3x3Graph_Output", icp::DATATYPE_08U, 1, 1, 8, 8, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0);

         CfgScenarios(&gScenarioList_APU_GAUSS_3X3);

         FlagAsInitialized();
      }

      lRetVal = SelectApuConfiguration(mApuCfg, mApexId); //by default mApuCfg = ACF_APU_CFG__APEX0_DEFAULT and mApexId = 0

      return lRetVal;
   }
};

#endif //_ACF_PROCESS_APU_APU_GAUSS_3X3
