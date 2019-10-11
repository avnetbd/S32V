#ifndef _ACF_PROCESS_APU_LDWS_REMAP_SOBEL_MEDIAN
#define _ACF_PROCESS_APU_LDWS_REMAP_SOBEL_MEDIAN

#include <acf_process_apu.h>
#include <LDWS_REMAP_SOBEL_MEDIAN_APU_LOAD.h> //APU load associated with this process
#include <stdint.h>


//SCENARIO LIST*************************************************
static acf_scenario_buffer_data gScenarioBufferData0_LDWS_REMAP_SOBEL_MEDIAN[] = {{36, 12, 2, 0, {0, 0, 0, 0}}, {6, 8, 4, 0, {1, 1, 2, 2}}, {6, 8, 4, 1, {1, 1, 2, 2}}, {6, 8, 2, 2, {0, 0, 0, 0}}, {6, 8, 2, 0, {0, 0, 0, 0}}, {6, 8, 2, 0, {0, 0, 0, 0}}};

static acf_scenario_kernel_data gScenarioKernelData0_LDWS_REMAP_SOBEL_MEDIAN[] = {{0, 0}, {1, 0}, {2, 0}};

static acf_scenario gScenarioArray_LDWS_REMAP_SOBEL_MEDIAN[] = {
{0, 0, 2024, 0, 2, gScenarioBufferData0_LDWS_REMAP_SOBEL_MEDIAN, 96, gScenarioKernelData0_LDWS_REMAP_SOBEL_MEDIAN, 12}
};

static acf_scenario_list gScenarioList_LDWS_REMAP_SOBEL_MEDIAN = {
1, //number of scenarios
gScenarioArray_LDWS_REMAP_SOBEL_MEDIAN};
//**************************************************************

class LDWS_REMAP_SOBEL_MEDIAN : public ACF_Process_APU
{

public:
   LDWS_REMAP_SOBEL_MEDIAN(int32_t apex_id = 0) : ACF_Process_APU(apex_id)
   {} 

   int32_t Initialize()
   {
      int32_t lRetVal = 0;

      if (!Initialized()) //initialization steps that only need to occur once
      {
         SetProcessIdentifier("LDWS_REMAP_SOBEL_MEDIAN");
         SetApuLoadInfo(LDWS_REMAP_SOBEL_MEDIAN_LOAD_SEGMENTS,
                        LDWS_REMAP_SOBEL_MEDIAN_LOAD_PMEM, LDWS_REMAP_SOBEL_MEDIAN_LOAD_PMEM_SIZE,
                        LDWS_REMAP_SOBEL_MEDIAN_LOAD_DMEM, LDWS_REMAP_SOBEL_MEDIAN_LOAD_DMEM_SIZE,
                        0, 0); //assuming _LOAD_CMEM does not exist

         lRetVal = NonParametricInitialization();
         if (lRetVal != 0) return lRetVal;

         FlagSpatialDep();

         //***NOTE: the order in which the following ports are added is meaningful; do not change!

         AddPort("SRC_IMG", icp::DATATYPE_08U, 1, 1, 36, 12, 0, 0, 0, 1, 1, 0, 0, 72, 72, 24, 24, 0);
         AddPort("LOCAL_OFFSET", icp::DATATYPE_16U, 1, 1, 6, 8, 0, 0, 0, 0, 0, 0, 4, 2, 2, 2, 2, 0);
         AddPort("DELTA", icp::DATATYPE_08U, 2, 1, 6, 8, 0, 0, 0, 0, 0, 0, 5, 2, 2, 2, 2, 0);
         AddPort("DST_IMG", icp::DATATYPE_08U, 1, 1, 6, 8, 1, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0);

         CfgScenarios(&gScenarioList_LDWS_REMAP_SOBEL_MEDIAN);

         FlagAsInitialized();
      }

      lRetVal = SelectApuConfiguration(mApuCfg, mApexId); //by default mApuCfg = ACF_APU_CFG__APEX0_DEFAULT and mApexId = 0

      return lRetVal;
   }
};

#endif //_ACF_PROCESS_APU_LDWS_REMAP_SOBEL_MEDIAN
