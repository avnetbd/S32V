#ifndef _ACF_PROCESS_APU_APU_CONVOLUTION
#define _ACF_PROCESS_APU_APU_CONVOLUTION

#include <acf_process_apu.h>
#include <APU_CONVOLUTION_APU_LOAD.h> //APU load associated with this process
#include <stdint.h>


//SCENARIO LIST*************************************************
static acf_scenario_buffer_data gScenarioBufferData0_APU_CONVOLUTION[] = {{2, 1, 4, 0, {1, 1, 2, 2}}, {2, 1, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData1_APU_CONVOLUTION[] = {{4, 1, 4, 0, {1, 1, 2, 2}}, {4, 1, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData2_APU_CONVOLUTION[] = {{6, 1, 4, 0, {1, 1, 2, 2}}, {6, 1, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData3_APU_CONVOLUTION[] = {{8, 1, 4, 0, {1, 1, 2, 2}}, {8, 1, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData4_APU_CONVOLUTION[] = {{10, 1, 4, 0, {1, 1, 2, 2}}, {10, 1, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData5_APU_CONVOLUTION[] = {{12, 1, 4, 0, {1, 1, 2, 2}}, {12, 1, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData6_APU_CONVOLUTION[] = {{14, 1, 4, 0, {1, 1, 2, 2}}, {14, 1, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData7_APU_CONVOLUTION[] = {{16, 1, 4, 0, {1, 1, 2, 2}}, {16, 1, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData8_APU_CONVOLUTION[] = {{18, 1, 4, 0, {1, 1, 2, 2}}, {18, 1, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData9_APU_CONVOLUTION[] = {{20, 1, 4, 0, {1, 1, 2, 2}}, {20, 1, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData10_APU_CONVOLUTION[] = {{22, 1, 4, 0, {1, 1, 2, 2}}, {22, 1, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData11_APU_CONVOLUTION[] = {{24, 1, 4, 0, {1, 1, 2, 2}}, {24, 1, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData12_APU_CONVOLUTION[] = {{26, 1, 4, 0, {1, 1, 2, 2}}, {26, 1, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData13_APU_CONVOLUTION[] = {{28, 1, 4, 0, {1, 1, 2, 2}}, {28, 1, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData14_APU_CONVOLUTION[] = {{30, 1, 4, 0, {1, 1, 2, 2}}, {30, 1, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData15_APU_CONVOLUTION[] = {{32, 1, 4, 0, {1, 1, 2, 2}}, {32, 1, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData16_APU_CONVOLUTION[] = {{36, 1, 4, 0, {1, 1, 2, 2}}, {36, 1, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData17_APU_CONVOLUTION[] = {{40, 1, 4, 0, {1, 1, 2, 2}}, {40, 1, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData18_APU_CONVOLUTION[] = {{44, 1, 4, 0, {1, 1, 2, 2}}, {44, 1, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData19_APU_CONVOLUTION[] = {{48, 1, 4, 0, {1, 1, 2, 2}}, {48, 1, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData20_APU_CONVOLUTION[] = {{52, 1, 4, 0, {1, 1, 2, 2}}, {52, 1, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData21_APU_CONVOLUTION[] = {{56, 1, 4, 0, {1, 1, 2, 2}}, {56, 1, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData22_APU_CONVOLUTION[] = {{60, 1, 4, 0, {1, 1, 2, 2}}, {60, 1, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData23_APU_CONVOLUTION[] = {{64, 1, 4, 0, {1, 1, 2, 2}}, {64, 1, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData24_APU_CONVOLUTION[] = {{2, 2, 4, 0, {1, 1, 2, 2}}, {2, 2, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData25_APU_CONVOLUTION[] = {{4, 2, 4, 0, {1, 1, 2, 2}}, {4, 2, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData26_APU_CONVOLUTION[] = {{6, 2, 4, 0, {1, 1, 2, 2}}, {6, 2, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData27_APU_CONVOLUTION[] = {{8, 2, 4, 0, {1, 1, 2, 2}}, {8, 2, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData28_APU_CONVOLUTION[] = {{10, 2, 4, 0, {1, 1, 2, 2}}, {10, 2, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData29_APU_CONVOLUTION[] = {{12, 2, 4, 0, {1, 1, 2, 2}}, {12, 2, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData30_APU_CONVOLUTION[] = {{14, 2, 4, 0, {1, 1, 2, 2}}, {14, 2, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData31_APU_CONVOLUTION[] = {{16, 2, 4, 0, {1, 1, 2, 2}}, {16, 2, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData32_APU_CONVOLUTION[] = {{18, 2, 4, 0, {1, 1, 2, 2}}, {18, 2, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData33_APU_CONVOLUTION[] = {{20, 2, 4, 0, {1, 1, 2, 2}}, {20, 2, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData34_APU_CONVOLUTION[] = {{22, 2, 4, 0, {1, 1, 2, 2}}, {22, 2, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData35_APU_CONVOLUTION[] = {{24, 2, 4, 0, {1, 1, 2, 2}}, {24, 2, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData36_APU_CONVOLUTION[] = {{26, 2, 4, 0, {1, 1, 2, 2}}, {26, 2, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData37_APU_CONVOLUTION[] = {{28, 2, 4, 0, {1, 1, 2, 2}}, {28, 2, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData38_APU_CONVOLUTION[] = {{30, 2, 4, 0, {1, 1, 2, 2}}, {30, 2, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData39_APU_CONVOLUTION[] = {{32, 2, 4, 0, {1, 1, 2, 2}}, {32, 2, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData40_APU_CONVOLUTION[] = {{36, 2, 4, 0, {1, 1, 2, 2}}, {36, 2, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData41_APU_CONVOLUTION[] = {{40, 2, 4, 0, {1, 1, 2, 2}}, {40, 2, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData42_APU_CONVOLUTION[] = {{44, 2, 4, 0, {1, 1, 2, 2}}, {44, 2, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData43_APU_CONVOLUTION[] = {{48, 2, 4, 0, {1, 1, 2, 2}}, {48, 2, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData44_APU_CONVOLUTION[] = {{52, 2, 4, 0, {1, 1, 2, 2}}, {52, 2, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData45_APU_CONVOLUTION[] = {{56, 2, 4, 0, {1, 1, 2, 2}}, {56, 2, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData46_APU_CONVOLUTION[] = {{60, 2, 4, 0, {1, 1, 2, 2}}, {60, 2, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData47_APU_CONVOLUTION[] = {{64, 2, 4, 0, {1, 1, 2, 2}}, {64, 2, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData48_APU_CONVOLUTION[] = {{2, 3, 4, 0, {1, 1, 2, 2}}, {2, 3, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData49_APU_CONVOLUTION[] = {{4, 3, 4, 0, {1, 1, 2, 2}}, {4, 3, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData50_APU_CONVOLUTION[] = {{6, 3, 4, 0, {1, 1, 2, 2}}, {6, 3, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData51_APU_CONVOLUTION[] = {{8, 3, 4, 0, {1, 1, 2, 2}}, {8, 3, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData52_APU_CONVOLUTION[] = {{10, 3, 4, 0, {1, 1, 2, 2}}, {10, 3, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData53_APU_CONVOLUTION[] = {{12, 3, 4, 0, {1, 1, 2, 2}}, {12, 3, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData54_APU_CONVOLUTION[] = {{14, 3, 4, 0, {1, 1, 2, 2}}, {14, 3, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData55_APU_CONVOLUTION[] = {{16, 3, 4, 0, {1, 1, 2, 2}}, {16, 3, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData56_APU_CONVOLUTION[] = {{18, 3, 4, 0, {1, 1, 2, 2}}, {18, 3, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData57_APU_CONVOLUTION[] = {{20, 3, 4, 0, {1, 1, 2, 2}}, {20, 3, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData58_APU_CONVOLUTION[] = {{22, 3, 4, 0, {1, 1, 2, 2}}, {22, 3, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData59_APU_CONVOLUTION[] = {{24, 3, 4, 0, {1, 1, 2, 2}}, {24, 3, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData60_APU_CONVOLUTION[] = {{26, 3, 4, 0, {1, 1, 2, 2}}, {26, 3, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData61_APU_CONVOLUTION[] = {{28, 3, 4, 0, {1, 1, 2, 2}}, {28, 3, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData62_APU_CONVOLUTION[] = {{30, 3, 4, 0, {1, 1, 2, 2}}, {30, 3, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData63_APU_CONVOLUTION[] = {{32, 3, 4, 0, {1, 1, 2, 2}}, {32, 3, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData64_APU_CONVOLUTION[] = {{36, 3, 4, 0, {1, 1, 2, 2}}, {36, 3, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData65_APU_CONVOLUTION[] = {{40, 3, 4, 0, {1, 1, 2, 2}}, {40, 3, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData66_APU_CONVOLUTION[] = {{44, 3, 4, 0, {1, 1, 2, 2}}, {44, 3, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData67_APU_CONVOLUTION[] = {{48, 3, 4, 0, {1, 1, 2, 2}}, {48, 3, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData68_APU_CONVOLUTION[] = {{52, 3, 4, 0, {1, 1, 2, 2}}, {52, 3, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData69_APU_CONVOLUTION[] = {{56, 3, 4, 0, {1, 1, 2, 2}}, {56, 3, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData70_APU_CONVOLUTION[] = {{60, 3, 4, 0, {1, 1, 2, 2}}, {60, 3, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData71_APU_CONVOLUTION[] = {{64, 3, 4, 0, {1, 1, 2, 2}}, {64, 3, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData72_APU_CONVOLUTION[] = {{2, 4, 4, 0, {1, 1, 2, 2}}, {2, 4, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData73_APU_CONVOLUTION[] = {{4, 4, 4, 0, {1, 1, 2, 2}}, {4, 4, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData74_APU_CONVOLUTION[] = {{6, 4, 4, 0, {1, 1, 2, 2}}, {6, 4, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData75_APU_CONVOLUTION[] = {{8, 4, 4, 0, {1, 1, 2, 2}}, {8, 4, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData76_APU_CONVOLUTION[] = {{10, 4, 4, 0, {1, 1, 2, 2}}, {10, 4, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData77_APU_CONVOLUTION[] = {{12, 4, 4, 0, {1, 1, 2, 2}}, {12, 4, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData78_APU_CONVOLUTION[] = {{14, 4, 4, 0, {1, 1, 2, 2}}, {14, 4, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData79_APU_CONVOLUTION[] = {{16, 4, 4, 0, {1, 1, 2, 2}}, {16, 4, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData80_APU_CONVOLUTION[] = {{18, 4, 4, 0, {1, 1, 2, 2}}, {18, 4, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData81_APU_CONVOLUTION[] = {{20, 4, 4, 0, {1, 1, 2, 2}}, {20, 4, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData82_APU_CONVOLUTION[] = {{22, 4, 4, 0, {1, 1, 2, 2}}, {22, 4, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData83_APU_CONVOLUTION[] = {{24, 4, 4, 0, {1, 1, 2, 2}}, {24, 4, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData84_APU_CONVOLUTION[] = {{26, 4, 4, 0, {1, 1, 2, 2}}, {26, 4, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData85_APU_CONVOLUTION[] = {{28, 4, 4, 0, {1, 1, 2, 2}}, {28, 4, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData86_APU_CONVOLUTION[] = {{30, 4, 4, 0, {1, 1, 2, 2}}, {30, 4, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData87_APU_CONVOLUTION[] = {{32, 4, 4, 0, {1, 1, 2, 2}}, {32, 4, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData88_APU_CONVOLUTION[] = {{36, 4, 4, 0, {1, 1, 2, 2}}, {36, 4, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData89_APU_CONVOLUTION[] = {{40, 4, 4, 0, {1, 1, 2, 2}}, {40, 4, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData90_APU_CONVOLUTION[] = {{44, 4, 4, 0, {1, 1, 2, 2}}, {44, 4, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData91_APU_CONVOLUTION[] = {{48, 4, 4, 0, {1, 1, 2, 2}}, {48, 4, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData92_APU_CONVOLUTION[] = {{52, 4, 4, 0, {1, 1, 2, 2}}, {52, 4, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData93_APU_CONVOLUTION[] = {{56, 4, 4, 0, {1, 1, 2, 2}}, {56, 4, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData94_APU_CONVOLUTION[] = {{60, 4, 4, 0, {1, 1, 2, 2}}, {60, 4, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData95_APU_CONVOLUTION[] = {{64, 4, 4, 0, {1, 1, 2, 2}}, {64, 4, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData96_APU_CONVOLUTION[] = {{2, 5, 4, 0, {1, 1, 2, 2}}, {2, 5, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData97_APU_CONVOLUTION[] = {{4, 5, 4, 0, {1, 1, 2, 2}}, {4, 5, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData98_APU_CONVOLUTION[] = {{6, 5, 4, 0, {1, 1, 2, 2}}, {6, 5, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData99_APU_CONVOLUTION[] = {{8, 5, 4, 0, {1, 1, 2, 2}}, {8, 5, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData100_APU_CONVOLUTION[] = {{10, 5, 4, 0, {1, 1, 2, 2}}, {10, 5, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData101_APU_CONVOLUTION[] = {{12, 5, 4, 0, {1, 1, 2, 2}}, {12, 5, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData102_APU_CONVOLUTION[] = {{14, 5, 4, 0, {1, 1, 2, 2}}, {14, 5, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData103_APU_CONVOLUTION[] = {{16, 5, 4, 0, {1, 1, 2, 2}}, {16, 5, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData104_APU_CONVOLUTION[] = {{18, 5, 4, 0, {1, 1, 2, 2}}, {18, 5, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData105_APU_CONVOLUTION[] = {{20, 5, 4, 0, {1, 1, 2, 2}}, {20, 5, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData106_APU_CONVOLUTION[] = {{22, 5, 4, 0, {1, 1, 2, 2}}, {22, 5, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData107_APU_CONVOLUTION[] = {{24, 5, 4, 0, {1, 1, 2, 2}}, {24, 5, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData108_APU_CONVOLUTION[] = {{26, 5, 4, 0, {1, 1, 2, 2}}, {26, 5, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData109_APU_CONVOLUTION[] = {{28, 5, 4, 0, {1, 1, 2, 2}}, {28, 5, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData110_APU_CONVOLUTION[] = {{30, 5, 4, 0, {1, 1, 2, 2}}, {30, 5, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData111_APU_CONVOLUTION[] = {{32, 5, 4, 0, {1, 1, 2, 2}}, {32, 5, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData112_APU_CONVOLUTION[] = {{36, 5, 4, 0, {1, 1, 2, 2}}, {36, 5, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData113_APU_CONVOLUTION[] = {{40, 5, 4, 0, {1, 1, 2, 2}}, {40, 5, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData114_APU_CONVOLUTION[] = {{44, 5, 4, 0, {1, 1, 2, 2}}, {44, 5, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData115_APU_CONVOLUTION[] = {{48, 5, 4, 0, {1, 1, 2, 2}}, {48, 5, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData116_APU_CONVOLUTION[] = {{52, 5, 4, 0, {1, 1, 2, 2}}, {52, 5, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData117_APU_CONVOLUTION[] = {{56, 5, 4, 0, {1, 1, 2, 2}}, {56, 5, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData118_APU_CONVOLUTION[] = {{60, 5, 4, 0, {1, 1, 2, 2}}, {60, 5, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData119_APU_CONVOLUTION[] = {{64, 5, 4, 0, {1, 1, 2, 2}}, {64, 5, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData120_APU_CONVOLUTION[] = {{2, 6, 4, 0, {1, 1, 2, 2}}, {2, 6, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData121_APU_CONVOLUTION[] = {{4, 6, 4, 0, {1, 1, 2, 2}}, {4, 6, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData122_APU_CONVOLUTION[] = {{6, 6, 4, 0, {1, 1, 2, 2}}, {6, 6, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData123_APU_CONVOLUTION[] = {{8, 6, 4, 0, {1, 1, 2, 2}}, {8, 6, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData124_APU_CONVOLUTION[] = {{10, 6, 4, 0, {1, 1, 2, 2}}, {10, 6, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData125_APU_CONVOLUTION[] = {{12, 6, 4, 0, {1, 1, 2, 2}}, {12, 6, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData126_APU_CONVOLUTION[] = {{14, 6, 4, 0, {1, 1, 2, 2}}, {14, 6, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData127_APU_CONVOLUTION[] = {{16, 6, 4, 0, {1, 1, 2, 2}}, {16, 6, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData128_APU_CONVOLUTION[] = {{18, 6, 4, 0, {1, 1, 2, 2}}, {18, 6, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData129_APU_CONVOLUTION[] = {{20, 6, 4, 0, {1, 1, 2, 2}}, {20, 6, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData130_APU_CONVOLUTION[] = {{22, 6, 4, 0, {1, 1, 2, 2}}, {22, 6, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData131_APU_CONVOLUTION[] = {{24, 6, 4, 0, {1, 1, 2, 2}}, {24, 6, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData132_APU_CONVOLUTION[] = {{26, 6, 4, 0, {1, 1, 2, 2}}, {26, 6, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData133_APU_CONVOLUTION[] = {{28, 6, 4, 0, {1, 1, 2, 2}}, {28, 6, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData134_APU_CONVOLUTION[] = {{30, 6, 4, 0, {1, 1, 2, 2}}, {30, 6, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData135_APU_CONVOLUTION[] = {{32, 6, 4, 0, {1, 1, 2, 2}}, {32, 6, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData136_APU_CONVOLUTION[] = {{36, 6, 4, 0, {1, 1, 2, 2}}, {36, 6, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData137_APU_CONVOLUTION[] = {{40, 6, 4, 0, {1, 1, 2, 2}}, {40, 6, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData138_APU_CONVOLUTION[] = {{44, 6, 4, 0, {1, 1, 2, 2}}, {44, 6, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData139_APU_CONVOLUTION[] = {{48, 6, 4, 0, {1, 1, 2, 2}}, {48, 6, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData140_APU_CONVOLUTION[] = {{52, 6, 4, 0, {1, 1, 2, 2}}, {52, 6, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData141_APU_CONVOLUTION[] = {{56, 6, 4, 0, {1, 1, 2, 2}}, {56, 6, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData142_APU_CONVOLUTION[] = {{60, 6, 4, 0, {1, 1, 2, 2}}, {60, 6, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData143_APU_CONVOLUTION[] = {{64, 6, 4, 0, {1, 1, 2, 2}}, {64, 6, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData144_APU_CONVOLUTION[] = {{2, 8, 4, 0, {1, 1, 2, 2}}, {2, 8, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData145_APU_CONVOLUTION[] = {{4, 8, 4, 0, {1, 1, 2, 2}}, {4, 8, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData146_APU_CONVOLUTION[] = {{6, 8, 4, 0, {1, 1, 2, 2}}, {6, 8, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData147_APU_CONVOLUTION[] = {{8, 8, 4, 0, {1, 1, 2, 2}}, {8, 8, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData148_APU_CONVOLUTION[] = {{10, 8, 4, 0, {1, 1, 2, 2}}, {10, 8, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData149_APU_CONVOLUTION[] = {{12, 8, 4, 0, {1, 1, 2, 2}}, {12, 8, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData150_APU_CONVOLUTION[] = {{14, 8, 4, 0, {1, 1, 2, 2}}, {14, 8, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData151_APU_CONVOLUTION[] = {{16, 8, 4, 0, {1, 1, 2, 2}}, {16, 8, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData152_APU_CONVOLUTION[] = {{18, 8, 4, 0, {1, 1, 2, 2}}, {18, 8, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData153_APU_CONVOLUTION[] = {{20, 8, 4, 0, {1, 1, 2, 2}}, {20, 8, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData154_APU_CONVOLUTION[] = {{22, 8, 4, 0, {1, 1, 2, 2}}, {22, 8, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData155_APU_CONVOLUTION[] = {{24, 8, 4, 0, {1, 1, 2, 2}}, {24, 8, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData156_APU_CONVOLUTION[] = {{26, 8, 4, 0, {1, 1, 2, 2}}, {26, 8, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData157_APU_CONVOLUTION[] = {{28, 8, 4, 0, {1, 1, 2, 2}}, {28, 8, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData158_APU_CONVOLUTION[] = {{30, 8, 4, 0, {1, 1, 2, 2}}, {30, 8, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData159_APU_CONVOLUTION[] = {{32, 8, 4, 0, {1, 1, 2, 2}}, {32, 8, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData160_APU_CONVOLUTION[] = {{36, 8, 4, 0, {1, 1, 2, 2}}, {36, 8, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData161_APU_CONVOLUTION[] = {{40, 8, 4, 0, {1, 1, 2, 2}}, {40, 8, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData162_APU_CONVOLUTION[] = {{44, 8, 4, 0, {1, 1, 2, 2}}, {44, 8, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData163_APU_CONVOLUTION[] = {{48, 8, 4, 0, {1, 1, 2, 2}}, {48, 8, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData164_APU_CONVOLUTION[] = {{52, 8, 4, 0, {1, 1, 2, 2}}, {52, 8, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData165_APU_CONVOLUTION[] = {{56, 8, 4, 0, {1, 1, 2, 2}}, {56, 8, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData166_APU_CONVOLUTION[] = {{2, 10, 4, 0, {1, 1, 2, 2}}, {2, 10, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData167_APU_CONVOLUTION[] = {{4, 10, 4, 0, {1, 1, 2, 2}}, {4, 10, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData168_APU_CONVOLUTION[] = {{6, 10, 4, 0, {1, 1, 2, 2}}, {6, 10, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData169_APU_CONVOLUTION[] = {{8, 10, 4, 0, {1, 1, 2, 2}}, {8, 10, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData170_APU_CONVOLUTION[] = {{10, 10, 4, 0, {1, 1, 2, 2}}, {10, 10, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData171_APU_CONVOLUTION[] = {{12, 10, 4, 0, {1, 1, 2, 2}}, {12, 10, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData172_APU_CONVOLUTION[] = {{14, 10, 4, 0, {1, 1, 2, 2}}, {14, 10, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData173_APU_CONVOLUTION[] = {{16, 10, 4, 0, {1, 1, 2, 2}}, {16, 10, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData174_APU_CONVOLUTION[] = {{18, 10, 4, 0, {1, 1, 2, 2}}, {18, 10, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData175_APU_CONVOLUTION[] = {{20, 10, 4, 0, {1, 1, 2, 2}}, {20, 10, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData176_APU_CONVOLUTION[] = {{22, 10, 4, 0, {1, 1, 2, 2}}, {22, 10, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData177_APU_CONVOLUTION[] = {{24, 10, 4, 0, {1, 1, 2, 2}}, {24, 10, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData178_APU_CONVOLUTION[] = {{26, 10, 4, 0, {1, 1, 2, 2}}, {26, 10, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData179_APU_CONVOLUTION[] = {{28, 10, 4, 0, {1, 1, 2, 2}}, {28, 10, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData180_APU_CONVOLUTION[] = {{30, 10, 4, 0, {1, 1, 2, 2}}, {30, 10, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData181_APU_CONVOLUTION[] = {{32, 10, 4, 0, {1, 1, 2, 2}}, {32, 10, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData182_APU_CONVOLUTION[] = {{36, 10, 4, 0, {1, 1, 2, 2}}, {36, 10, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData183_APU_CONVOLUTION[] = {{40, 10, 4, 0, {1, 1, 2, 2}}, {40, 10, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData184_APU_CONVOLUTION[] = {{44, 10, 4, 0, {1, 1, 2, 2}}, {44, 10, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData185_APU_CONVOLUTION[] = {{2, 12, 4, 0, {1, 1, 2, 2}}, {2, 12, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData186_APU_CONVOLUTION[] = {{4, 12, 4, 0, {1, 1, 2, 2}}, {4, 12, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData187_APU_CONVOLUTION[] = {{6, 12, 4, 0, {1, 1, 2, 2}}, {6, 12, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData188_APU_CONVOLUTION[] = {{8, 12, 4, 0, {1, 1, 2, 2}}, {8, 12, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData189_APU_CONVOLUTION[] = {{10, 12, 4, 0, {1, 1, 2, 2}}, {10, 12, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData190_APU_CONVOLUTION[] = {{12, 12, 4, 0, {1, 1, 2, 2}}, {12, 12, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData191_APU_CONVOLUTION[] = {{14, 12, 4, 0, {1, 1, 2, 2}}, {14, 12, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData192_APU_CONVOLUTION[] = {{16, 12, 4, 0, {1, 1, 2, 2}}, {16, 12, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData193_APU_CONVOLUTION[] = {{18, 12, 4, 0, {1, 1, 2, 2}}, {18, 12, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData194_APU_CONVOLUTION[] = {{20, 12, 4, 0, {1, 1, 2, 2}}, {20, 12, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData195_APU_CONVOLUTION[] = {{22, 12, 4, 0, {1, 1, 2, 2}}, {22, 12, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData196_APU_CONVOLUTION[] = {{24, 12, 4, 0, {1, 1, 2, 2}}, {24, 12, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData197_APU_CONVOLUTION[] = {{26, 12, 4, 0, {1, 1, 2, 2}}, {26, 12, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData198_APU_CONVOLUTION[] = {{28, 12, 4, 0, {1, 1, 2, 2}}, {28, 12, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData199_APU_CONVOLUTION[] = {{30, 12, 4, 0, {1, 1, 2, 2}}, {30, 12, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData200_APU_CONVOLUTION[] = {{32, 12, 4, 0, {1, 1, 2, 2}}, {32, 12, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData201_APU_CONVOLUTION[] = {{36, 12, 4, 0, {1, 1, 2, 2}}, {36, 12, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData202_APU_CONVOLUTION[] = {{2, 14, 4, 0, {1, 1, 2, 2}}, {2, 14, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData203_APU_CONVOLUTION[] = {{4, 14, 4, 0, {1, 1, 2, 2}}, {4, 14, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData204_APU_CONVOLUTION[] = {{6, 14, 4, 0, {1, 1, 2, 2}}, {6, 14, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData205_APU_CONVOLUTION[] = {{8, 14, 4, 0, {1, 1, 2, 2}}, {8, 14, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData206_APU_CONVOLUTION[] = {{10, 14, 4, 0, {1, 1, 2, 2}}, {10, 14, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData207_APU_CONVOLUTION[] = {{12, 14, 4, 0, {1, 1, 2, 2}}, {12, 14, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData208_APU_CONVOLUTION[] = {{14, 14, 4, 0, {1, 1, 2, 2}}, {14, 14, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData209_APU_CONVOLUTION[] = {{16, 14, 4, 0, {1, 1, 2, 2}}, {16, 14, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData210_APU_CONVOLUTION[] = {{18, 14, 4, 0, {1, 1, 2, 2}}, {18, 14, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData211_APU_CONVOLUTION[] = {{20, 14, 4, 0, {1, 1, 2, 2}}, {20, 14, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData212_APU_CONVOLUTION[] = {{22, 14, 4, 0, {1, 1, 2, 2}}, {22, 14, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData213_APU_CONVOLUTION[] = {{24, 14, 4, 0, {1, 1, 2, 2}}, {24, 14, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData214_APU_CONVOLUTION[] = {{26, 14, 4, 0, {1, 1, 2, 2}}, {26, 14, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData215_APU_CONVOLUTION[] = {{28, 14, 4, 0, {1, 1, 2, 2}}, {28, 14, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData216_APU_CONVOLUTION[] = {{30, 14, 4, 0, {1, 1, 2, 2}}, {30, 14, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData217_APU_CONVOLUTION[] = {{2, 16, 4, 0, {1, 1, 2, 2}}, {2, 16, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData218_APU_CONVOLUTION[] = {{4, 16, 4, 0, {1, 1, 2, 2}}, {4, 16, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData219_APU_CONVOLUTION[] = {{6, 16, 4, 0, {1, 1, 2, 2}}, {6, 16, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData220_APU_CONVOLUTION[] = {{8, 16, 4, 0, {1, 1, 2, 2}}, {8, 16, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData221_APU_CONVOLUTION[] = {{10, 16, 4, 0, {1, 1, 2, 2}}, {10, 16, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData222_APU_CONVOLUTION[] = {{12, 16, 4, 0, {1, 1, 2, 2}}, {12, 16, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData223_APU_CONVOLUTION[] = {{14, 16, 4, 0, {1, 1, 2, 2}}, {14, 16, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData224_APU_CONVOLUTION[] = {{16, 16, 4, 0, {1, 1, 2, 2}}, {16, 16, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData225_APU_CONVOLUTION[] = {{18, 16, 4, 0, {1, 1, 2, 2}}, {18, 16, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData226_APU_CONVOLUTION[] = {{20, 16, 4, 0, {1, 1, 2, 2}}, {20, 16, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData227_APU_CONVOLUTION[] = {{22, 16, 4, 0, {1, 1, 2, 2}}, {22, 16, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData228_APU_CONVOLUTION[] = {{24, 16, 4, 0, {1, 1, 2, 2}}, {24, 16, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData229_APU_CONVOLUTION[] = {{26, 16, 4, 0, {1, 1, 2, 2}}, {26, 16, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData230_APU_CONVOLUTION[] = {{2, 18, 4, 0, {1, 1, 2, 2}}, {2, 18, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData231_APU_CONVOLUTION[] = {{4, 18, 4, 0, {1, 1, 2, 2}}, {4, 18, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData232_APU_CONVOLUTION[] = {{6, 18, 4, 0, {1, 1, 2, 2}}, {6, 18, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData233_APU_CONVOLUTION[] = {{8, 18, 4, 0, {1, 1, 2, 2}}, {8, 18, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData234_APU_CONVOLUTION[] = {{10, 18, 4, 0, {1, 1, 2, 2}}, {10, 18, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData235_APU_CONVOLUTION[] = {{12, 18, 4, 0, {1, 1, 2, 2}}, {12, 18, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData236_APU_CONVOLUTION[] = {{14, 18, 4, 0, {1, 1, 2, 2}}, {14, 18, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData237_APU_CONVOLUTION[] = {{16, 18, 4, 0, {1, 1, 2, 2}}, {16, 18, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData238_APU_CONVOLUTION[] = {{18, 18, 4, 0, {1, 1, 2, 2}}, {18, 18, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData239_APU_CONVOLUTION[] = {{20, 18, 4, 0, {1, 1, 2, 2}}, {20, 18, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData240_APU_CONVOLUTION[] = {{22, 18, 4, 0, {1, 1, 2, 2}}, {22, 18, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData241_APU_CONVOLUTION[] = {{24, 18, 4, 0, {1, 1, 2, 2}}, {24, 18, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData242_APU_CONVOLUTION[] = {{2, 20, 4, 0, {1, 1, 2, 2}}, {2, 20, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData243_APU_CONVOLUTION[] = {{4, 20, 4, 0, {1, 1, 2, 2}}, {4, 20, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData244_APU_CONVOLUTION[] = {{6, 20, 4, 0, {1, 1, 2, 2}}, {6, 20, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData245_APU_CONVOLUTION[] = {{8, 20, 4, 0, {1, 1, 2, 2}}, {8, 20, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData246_APU_CONVOLUTION[] = {{10, 20, 4, 0, {1, 1, 2, 2}}, {10, 20, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData247_APU_CONVOLUTION[] = {{12, 20, 4, 0, {1, 1, 2, 2}}, {12, 20, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData248_APU_CONVOLUTION[] = {{14, 20, 4, 0, {1, 1, 2, 2}}, {14, 20, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData249_APU_CONVOLUTION[] = {{16, 20, 4, 0, {1, 1, 2, 2}}, {16, 20, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData250_APU_CONVOLUTION[] = {{18, 20, 4, 0, {1, 1, 2, 2}}, {18, 20, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData251_APU_CONVOLUTION[] = {{20, 20, 4, 0, {1, 1, 2, 2}}, {20, 20, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData252_APU_CONVOLUTION[] = {{2, 22, 4, 0, {1, 1, 2, 2}}, {2, 22, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData253_APU_CONVOLUTION[] = {{4, 22, 4, 0, {1, 1, 2, 2}}, {4, 22, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData254_APU_CONVOLUTION[] = {{6, 22, 4, 0, {1, 1, 2, 2}}, {6, 22, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData255_APU_CONVOLUTION[] = {{8, 22, 4, 0, {1, 1, 2, 2}}, {8, 22, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData256_APU_CONVOLUTION[] = {{10, 22, 4, 0, {1, 1, 2, 2}}, {10, 22, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData257_APU_CONVOLUTION[] = {{12, 22, 4, 0, {1, 1, 2, 2}}, {12, 22, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData258_APU_CONVOLUTION[] = {{14, 22, 4, 0, {1, 1, 2, 2}}, {14, 22, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData259_APU_CONVOLUTION[] = {{16, 22, 4, 0, {1, 1, 2, 2}}, {16, 22, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData260_APU_CONVOLUTION[] = {{18, 22, 4, 0, {1, 1, 2, 2}}, {18, 22, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData261_APU_CONVOLUTION[] = {{2, 24, 4, 0, {1, 1, 2, 2}}, {2, 24, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData262_APU_CONVOLUTION[] = {{4, 24, 4, 0, {1, 1, 2, 2}}, {4, 24, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData263_APU_CONVOLUTION[] = {{6, 24, 4, 0, {1, 1, 2, 2}}, {6, 24, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData264_APU_CONVOLUTION[] = {{8, 24, 4, 0, {1, 1, 2, 2}}, {8, 24, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData265_APU_CONVOLUTION[] = {{10, 24, 4, 0, {1, 1, 2, 2}}, {10, 24, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData266_APU_CONVOLUTION[] = {{12, 24, 4, 0, {1, 1, 2, 2}}, {12, 24, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData267_APU_CONVOLUTION[] = {{14, 24, 4, 0, {1, 1, 2, 2}}, {14, 24, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData268_APU_CONVOLUTION[] = {{16, 24, 4, 0, {1, 1, 2, 2}}, {16, 24, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData269_APU_CONVOLUTION[] = {{2, 26, 4, 0, {1, 1, 2, 2}}, {2, 26, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData270_APU_CONVOLUTION[] = {{4, 26, 4, 0, {1, 1, 2, 2}}, {4, 26, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData271_APU_CONVOLUTION[] = {{6, 26, 4, 0, {1, 1, 2, 2}}, {6, 26, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData272_APU_CONVOLUTION[] = {{8, 26, 4, 0, {1, 1, 2, 2}}, {8, 26, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData273_APU_CONVOLUTION[] = {{10, 26, 4, 0, {1, 1, 2, 2}}, {10, 26, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData274_APU_CONVOLUTION[] = {{12, 26, 4, 0, {1, 1, 2, 2}}, {12, 26, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData275_APU_CONVOLUTION[] = {{14, 26, 4, 0, {1, 1, 2, 2}}, {14, 26, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData276_APU_CONVOLUTION[] = {{16, 26, 4, 0, {1, 1, 2, 2}}, {16, 26, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData277_APU_CONVOLUTION[] = {{2, 28, 4, 0, {1, 1, 2, 2}}, {2, 28, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData278_APU_CONVOLUTION[] = {{4, 28, 4, 0, {1, 1, 2, 2}}, {4, 28, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData279_APU_CONVOLUTION[] = {{6, 28, 4, 0, {1, 1, 2, 2}}, {6, 28, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData280_APU_CONVOLUTION[] = {{8, 28, 4, 0, {1, 1, 2, 2}}, {8, 28, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData281_APU_CONVOLUTION[] = {{10, 28, 4, 0, {1, 1, 2, 2}}, {10, 28, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData282_APU_CONVOLUTION[] = {{12, 28, 4, 0, {1, 1, 2, 2}}, {12, 28, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData283_APU_CONVOLUTION[] = {{14, 28, 4, 0, {1, 1, 2, 2}}, {14, 28, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData284_APU_CONVOLUTION[] = {{2, 30, 4, 0, {1, 1, 2, 2}}, {2, 30, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData285_APU_CONVOLUTION[] = {{4, 30, 4, 0, {1, 1, 2, 2}}, {4, 30, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData286_APU_CONVOLUTION[] = {{6, 30, 4, 0, {1, 1, 2, 2}}, {6, 30, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData287_APU_CONVOLUTION[] = {{8, 30, 4, 0, {1, 1, 2, 2}}, {8, 30, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData288_APU_CONVOLUTION[] = {{10, 30, 4, 0, {1, 1, 2, 2}}, {10, 30, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData289_APU_CONVOLUTION[] = {{12, 30, 4, 0, {1, 1, 2, 2}}, {12, 30, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData290_APU_CONVOLUTION[] = {{2, 32, 4, 0, {1, 1, 2, 2}}, {2, 32, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData291_APU_CONVOLUTION[] = {{4, 32, 4, 0, {1, 1, 2, 2}}, {4, 32, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData292_APU_CONVOLUTION[] = {{6, 32, 4, 0, {1, 1, 2, 2}}, {6, 32, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData293_APU_CONVOLUTION[] = {{8, 32, 4, 0, {1, 1, 2, 2}}, {8, 32, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData294_APU_CONVOLUTION[] = {{10, 32, 4, 0, {1, 1, 2, 2}}, {10, 32, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData295_APU_CONVOLUTION[] = {{12, 32, 4, 0, {1, 1, 2, 2}}, {12, 32, 2, 1, {0, 0, 0, 0}}, {3, 3, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};

static acf_scenario_kernel_data gScenarioKernelData0_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData1_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData2_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData3_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData4_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData5_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData6_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData7_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData8_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData9_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData10_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData11_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData12_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData13_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData14_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData15_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData16_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData17_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData18_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData19_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData20_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData21_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData22_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData23_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData24_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData25_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData26_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData27_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData28_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData29_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData30_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData31_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData32_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData33_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData34_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData35_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData36_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData37_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData38_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData39_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData40_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData41_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData42_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData43_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData44_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData45_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData46_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData47_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData48_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData49_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData50_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData51_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData52_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData53_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData54_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData55_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData56_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData57_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData58_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData59_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData60_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData61_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData62_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData63_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData64_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData65_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData66_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData67_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData68_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData69_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData70_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData71_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData72_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData73_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData74_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData75_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData76_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData77_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData78_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData79_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData80_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData81_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData82_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData83_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData84_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData85_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData86_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData87_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData88_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData89_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData90_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData91_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData92_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData93_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData94_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData95_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData96_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData97_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData98_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData99_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData100_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData101_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData102_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData103_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData104_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData105_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData106_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData107_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData108_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData109_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData110_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData111_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData112_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData113_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData114_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData115_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData116_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData117_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData118_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData119_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData120_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData121_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData122_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData123_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData124_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData125_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData126_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData127_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData128_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData129_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData130_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData131_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData132_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData133_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData134_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData135_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData136_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData137_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData138_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData139_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData140_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData141_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData142_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData143_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData144_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData145_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData146_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData147_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData148_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData149_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData150_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData151_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData152_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData153_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData154_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData155_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData156_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData157_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData158_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData159_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData160_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData161_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData162_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData163_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData164_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData165_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData166_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData167_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData168_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData169_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData170_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData171_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData172_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData173_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData174_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData175_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData176_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData177_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData178_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData179_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData180_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData181_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData182_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData183_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData184_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData185_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData186_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData187_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData188_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData189_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData190_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData191_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData192_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData193_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData194_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData195_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData196_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData197_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData198_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData199_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData200_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData201_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData202_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData203_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData204_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData205_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData206_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData207_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData208_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData209_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData210_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData211_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData212_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData213_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData214_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData215_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData216_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData217_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData218_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData219_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData220_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData221_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData222_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData223_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData224_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData225_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData226_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData227_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData228_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData229_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData230_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData231_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData232_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData233_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData234_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData235_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData236_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData237_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData238_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData239_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData240_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData241_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData242_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData243_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData244_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData245_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData246_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData247_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData248_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData249_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData250_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData251_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData252_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData253_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData254_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData255_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData256_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData257_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData258_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData259_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData260_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData261_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData262_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData263_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData264_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData265_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData266_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData267_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData268_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData269_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData270_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData271_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData272_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData273_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData274_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData275_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData276_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData277_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData278_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData279_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData280_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData281_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData282_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData283_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData284_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData285_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData286_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData287_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData288_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData289_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData290_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData291_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData292_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData293_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData294_APU_CONVOLUTION[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData295_APU_CONVOLUTION[] = {{1, 0}};

static acf_scenario gScenarioArray_APU_CONVOLUTION[] = {
{2, 1, 44, 80, 1, gScenarioBufferData0_APU_CONVOLUTION, 80, gScenarioKernelData0_APU_CONVOLUTION, 4},
{4, 1, 64, 80, 1, gScenarioBufferData1_APU_CONVOLUTION, 80, gScenarioKernelData1_APU_CONVOLUTION, 4},
{6, 1, 84, 80, 1, gScenarioBufferData2_APU_CONVOLUTION, 80, gScenarioKernelData2_APU_CONVOLUTION, 4},
{8, 1, 104, 80, 1, gScenarioBufferData3_APU_CONVOLUTION, 80, gScenarioKernelData3_APU_CONVOLUTION, 4},
{10, 1, 124, 80, 1, gScenarioBufferData4_APU_CONVOLUTION, 80, gScenarioKernelData4_APU_CONVOLUTION, 4},
{12, 1, 144, 80, 1, gScenarioBufferData5_APU_CONVOLUTION, 80, gScenarioKernelData5_APU_CONVOLUTION, 4},
{14, 1, 164, 80, 1, gScenarioBufferData6_APU_CONVOLUTION, 80, gScenarioKernelData6_APU_CONVOLUTION, 4},
{16, 1, 184, 80, 1, gScenarioBufferData7_APU_CONVOLUTION, 80, gScenarioKernelData7_APU_CONVOLUTION, 4},
{18, 1, 204, 80, 1, gScenarioBufferData8_APU_CONVOLUTION, 80, gScenarioKernelData8_APU_CONVOLUTION, 4},
{20, 1, 224, 80, 1, gScenarioBufferData9_APU_CONVOLUTION, 80, gScenarioKernelData9_APU_CONVOLUTION, 4},
{22, 1, 244, 80, 1, gScenarioBufferData10_APU_CONVOLUTION, 80, gScenarioKernelData10_APU_CONVOLUTION, 4},
{24, 1, 264, 80, 1, gScenarioBufferData11_APU_CONVOLUTION, 80, gScenarioKernelData11_APU_CONVOLUTION, 4},
{26, 1, 284, 80, 1, gScenarioBufferData12_APU_CONVOLUTION, 80, gScenarioKernelData12_APU_CONVOLUTION, 4},
{28, 1, 304, 80, 1, gScenarioBufferData13_APU_CONVOLUTION, 80, gScenarioKernelData13_APU_CONVOLUTION, 4},
{30, 1, 324, 80, 1, gScenarioBufferData14_APU_CONVOLUTION, 80, gScenarioKernelData14_APU_CONVOLUTION, 4},
{32, 1, 344, 80, 1, gScenarioBufferData15_APU_CONVOLUTION, 80, gScenarioKernelData15_APU_CONVOLUTION, 4},
{36, 1, 384, 80, 1, gScenarioBufferData16_APU_CONVOLUTION, 80, gScenarioKernelData16_APU_CONVOLUTION, 4},
{40, 1, 424, 80, 1, gScenarioBufferData17_APU_CONVOLUTION, 80, gScenarioKernelData17_APU_CONVOLUTION, 4},
{44, 1, 464, 80, 1, gScenarioBufferData18_APU_CONVOLUTION, 80, gScenarioKernelData18_APU_CONVOLUTION, 4},
{48, 1, 504, 80, 1, gScenarioBufferData19_APU_CONVOLUTION, 80, gScenarioKernelData19_APU_CONVOLUTION, 4},
{52, 1, 544, 80, 1, gScenarioBufferData20_APU_CONVOLUTION, 80, gScenarioKernelData20_APU_CONVOLUTION, 4},
{56, 1, 584, 80, 1, gScenarioBufferData21_APU_CONVOLUTION, 80, gScenarioKernelData21_APU_CONVOLUTION, 4},
{60, 1, 624, 80, 1, gScenarioBufferData22_APU_CONVOLUTION, 80, gScenarioKernelData22_APU_CONVOLUTION, 4},
{64, 1, 664, 80, 1, gScenarioBufferData23_APU_CONVOLUTION, 80, gScenarioKernelData23_APU_CONVOLUTION, 4},
{2, 2, 76, 80, 1, gScenarioBufferData24_APU_CONVOLUTION, 80, gScenarioKernelData24_APU_CONVOLUTION, 4},
{4, 2, 112, 80, 1, gScenarioBufferData25_APU_CONVOLUTION, 80, gScenarioKernelData25_APU_CONVOLUTION, 4},
{6, 2, 148, 80, 1, gScenarioBufferData26_APU_CONVOLUTION, 80, gScenarioKernelData26_APU_CONVOLUTION, 4},
{8, 2, 184, 80, 1, gScenarioBufferData27_APU_CONVOLUTION, 80, gScenarioKernelData27_APU_CONVOLUTION, 4},
{10, 2, 220, 80, 1, gScenarioBufferData28_APU_CONVOLUTION, 80, gScenarioKernelData28_APU_CONVOLUTION, 4},
{12, 2, 256, 80, 1, gScenarioBufferData29_APU_CONVOLUTION, 80, gScenarioKernelData29_APU_CONVOLUTION, 4},
{14, 2, 292, 80, 1, gScenarioBufferData30_APU_CONVOLUTION, 80, gScenarioKernelData30_APU_CONVOLUTION, 4},
{16, 2, 328, 80, 1, gScenarioBufferData31_APU_CONVOLUTION, 80, gScenarioKernelData31_APU_CONVOLUTION, 4},
{18, 2, 364, 80, 1, gScenarioBufferData32_APU_CONVOLUTION, 80, gScenarioKernelData32_APU_CONVOLUTION, 4},
{20, 2, 400, 80, 1, gScenarioBufferData33_APU_CONVOLUTION, 80, gScenarioKernelData33_APU_CONVOLUTION, 4},
{22, 2, 436, 80, 1, gScenarioBufferData34_APU_CONVOLUTION, 80, gScenarioKernelData34_APU_CONVOLUTION, 4},
{24, 2, 472, 80, 1, gScenarioBufferData35_APU_CONVOLUTION, 80, gScenarioKernelData35_APU_CONVOLUTION, 4},
{26, 2, 508, 80, 1, gScenarioBufferData36_APU_CONVOLUTION, 80, gScenarioKernelData36_APU_CONVOLUTION, 4},
{28, 2, 544, 80, 1, gScenarioBufferData37_APU_CONVOLUTION, 80, gScenarioKernelData37_APU_CONVOLUTION, 4},
{30, 2, 580, 80, 1, gScenarioBufferData38_APU_CONVOLUTION, 80, gScenarioKernelData38_APU_CONVOLUTION, 4},
{32, 2, 616, 80, 1, gScenarioBufferData39_APU_CONVOLUTION, 80, gScenarioKernelData39_APU_CONVOLUTION, 4},
{36, 2, 688, 80, 1, gScenarioBufferData40_APU_CONVOLUTION, 80, gScenarioKernelData40_APU_CONVOLUTION, 4},
{40, 2, 760, 80, 1, gScenarioBufferData41_APU_CONVOLUTION, 80, gScenarioKernelData41_APU_CONVOLUTION, 4},
{44, 2, 832, 80, 1, gScenarioBufferData42_APU_CONVOLUTION, 80, gScenarioKernelData42_APU_CONVOLUTION, 4},
{48, 2, 904, 80, 1, gScenarioBufferData43_APU_CONVOLUTION, 80, gScenarioKernelData43_APU_CONVOLUTION, 4},
{52, 2, 976, 80, 1, gScenarioBufferData44_APU_CONVOLUTION, 80, gScenarioKernelData44_APU_CONVOLUTION, 4},
{56, 2, 1048, 80, 1, gScenarioBufferData45_APU_CONVOLUTION, 80, gScenarioKernelData45_APU_CONVOLUTION, 4},
{60, 2, 1120, 80, 1, gScenarioBufferData46_APU_CONVOLUTION, 80, gScenarioKernelData46_APU_CONVOLUTION, 4},
{64, 2, 1192, 80, 1, gScenarioBufferData47_APU_CONVOLUTION, 80, gScenarioKernelData47_APU_CONVOLUTION, 4},
{2, 3, 108, 80, 1, gScenarioBufferData48_APU_CONVOLUTION, 80, gScenarioKernelData48_APU_CONVOLUTION, 4},
{4, 3, 160, 80, 1, gScenarioBufferData49_APU_CONVOLUTION, 80, gScenarioKernelData49_APU_CONVOLUTION, 4},
{6, 3, 212, 80, 1, gScenarioBufferData50_APU_CONVOLUTION, 80, gScenarioKernelData50_APU_CONVOLUTION, 4},
{8, 3, 264, 80, 1, gScenarioBufferData51_APU_CONVOLUTION, 80, gScenarioKernelData51_APU_CONVOLUTION, 4},
{10, 3, 316, 80, 1, gScenarioBufferData52_APU_CONVOLUTION, 80, gScenarioKernelData52_APU_CONVOLUTION, 4},
{12, 3, 368, 80, 1, gScenarioBufferData53_APU_CONVOLUTION, 80, gScenarioKernelData53_APU_CONVOLUTION, 4},
{14, 3, 420, 80, 1, gScenarioBufferData54_APU_CONVOLUTION, 80, gScenarioKernelData54_APU_CONVOLUTION, 4},
{16, 3, 472, 80, 1, gScenarioBufferData55_APU_CONVOLUTION, 80, gScenarioKernelData55_APU_CONVOLUTION, 4},
{18, 3, 524, 80, 1, gScenarioBufferData56_APU_CONVOLUTION, 80, gScenarioKernelData56_APU_CONVOLUTION, 4},
{20, 3, 576, 80, 1, gScenarioBufferData57_APU_CONVOLUTION, 80, gScenarioKernelData57_APU_CONVOLUTION, 4},
{22, 3, 628, 80, 1, gScenarioBufferData58_APU_CONVOLUTION, 80, gScenarioKernelData58_APU_CONVOLUTION, 4},
{24, 3, 680, 80, 1, gScenarioBufferData59_APU_CONVOLUTION, 80, gScenarioKernelData59_APU_CONVOLUTION, 4},
{26, 3, 732, 80, 1, gScenarioBufferData60_APU_CONVOLUTION, 80, gScenarioKernelData60_APU_CONVOLUTION, 4},
{28, 3, 784, 80, 1, gScenarioBufferData61_APU_CONVOLUTION, 80, gScenarioKernelData61_APU_CONVOLUTION, 4},
{30, 3, 836, 80, 1, gScenarioBufferData62_APU_CONVOLUTION, 80, gScenarioKernelData62_APU_CONVOLUTION, 4},
{32, 3, 888, 80, 1, gScenarioBufferData63_APU_CONVOLUTION, 80, gScenarioKernelData63_APU_CONVOLUTION, 4},
{36, 3, 992, 80, 1, gScenarioBufferData64_APU_CONVOLUTION, 80, gScenarioKernelData64_APU_CONVOLUTION, 4},
{40, 3, 1096, 80, 1, gScenarioBufferData65_APU_CONVOLUTION, 80, gScenarioKernelData65_APU_CONVOLUTION, 4},
{44, 3, 1200, 80, 1, gScenarioBufferData66_APU_CONVOLUTION, 80, gScenarioKernelData66_APU_CONVOLUTION, 4},
{48, 3, 1304, 80, 1, gScenarioBufferData67_APU_CONVOLUTION, 80, gScenarioKernelData67_APU_CONVOLUTION, 4},
{52, 3, 1408, 80, 1, gScenarioBufferData68_APU_CONVOLUTION, 80, gScenarioKernelData68_APU_CONVOLUTION, 4},
{56, 3, 1512, 80, 1, gScenarioBufferData69_APU_CONVOLUTION, 80, gScenarioKernelData69_APU_CONVOLUTION, 4},
{60, 3, 1616, 80, 1, gScenarioBufferData70_APU_CONVOLUTION, 80, gScenarioKernelData70_APU_CONVOLUTION, 4},
{64, 3, 1720, 80, 1, gScenarioBufferData71_APU_CONVOLUTION, 80, gScenarioKernelData71_APU_CONVOLUTION, 4},
{2, 4, 140, 80, 1, gScenarioBufferData72_APU_CONVOLUTION, 80, gScenarioKernelData72_APU_CONVOLUTION, 4},
{4, 4, 208, 80, 1, gScenarioBufferData73_APU_CONVOLUTION, 80, gScenarioKernelData73_APU_CONVOLUTION, 4},
{6, 4, 276, 80, 1, gScenarioBufferData74_APU_CONVOLUTION, 80, gScenarioKernelData74_APU_CONVOLUTION, 4},
{8, 4, 344, 80, 1, gScenarioBufferData75_APU_CONVOLUTION, 80, gScenarioKernelData75_APU_CONVOLUTION, 4},
{10, 4, 412, 80, 1, gScenarioBufferData76_APU_CONVOLUTION, 80, gScenarioKernelData76_APU_CONVOLUTION, 4},
{12, 4, 480, 80, 1, gScenarioBufferData77_APU_CONVOLUTION, 80, gScenarioKernelData77_APU_CONVOLUTION, 4},
{14, 4, 548, 80, 1, gScenarioBufferData78_APU_CONVOLUTION, 80, gScenarioKernelData78_APU_CONVOLUTION, 4},
{16, 4, 616, 80, 1, gScenarioBufferData79_APU_CONVOLUTION, 80, gScenarioKernelData79_APU_CONVOLUTION, 4},
{18, 4, 684, 80, 1, gScenarioBufferData80_APU_CONVOLUTION, 80, gScenarioKernelData80_APU_CONVOLUTION, 4},
{20, 4, 752, 80, 1, gScenarioBufferData81_APU_CONVOLUTION, 80, gScenarioKernelData81_APU_CONVOLUTION, 4},
{22, 4, 820, 80, 1, gScenarioBufferData82_APU_CONVOLUTION, 80, gScenarioKernelData82_APU_CONVOLUTION, 4},
{24, 4, 888, 80, 1, gScenarioBufferData83_APU_CONVOLUTION, 80, gScenarioKernelData83_APU_CONVOLUTION, 4},
{26, 4, 956, 80, 1, gScenarioBufferData84_APU_CONVOLUTION, 80, gScenarioKernelData84_APU_CONVOLUTION, 4},
{28, 4, 1024, 80, 1, gScenarioBufferData85_APU_CONVOLUTION, 80, gScenarioKernelData85_APU_CONVOLUTION, 4},
{30, 4, 1092, 80, 1, gScenarioBufferData86_APU_CONVOLUTION, 80, gScenarioKernelData86_APU_CONVOLUTION, 4},
{32, 4, 1160, 80, 1, gScenarioBufferData87_APU_CONVOLUTION, 80, gScenarioKernelData87_APU_CONVOLUTION, 4},
{36, 4, 1296, 80, 1, gScenarioBufferData88_APU_CONVOLUTION, 80, gScenarioKernelData88_APU_CONVOLUTION, 4},
{40, 4, 1432, 80, 1, gScenarioBufferData89_APU_CONVOLUTION, 80, gScenarioKernelData89_APU_CONVOLUTION, 4},
{44, 4, 1568, 80, 1, gScenarioBufferData90_APU_CONVOLUTION, 80, gScenarioKernelData90_APU_CONVOLUTION, 4},
{48, 4, 1704, 80, 1, gScenarioBufferData91_APU_CONVOLUTION, 80, gScenarioKernelData91_APU_CONVOLUTION, 4},
{52, 4, 1840, 80, 1, gScenarioBufferData92_APU_CONVOLUTION, 80, gScenarioKernelData92_APU_CONVOLUTION, 4},
{56, 4, 1976, 80, 1, gScenarioBufferData93_APU_CONVOLUTION, 80, gScenarioKernelData93_APU_CONVOLUTION, 4},
{60, 4, 2112, 80, 1, gScenarioBufferData94_APU_CONVOLUTION, 80, gScenarioKernelData94_APU_CONVOLUTION, 4},
{64, 4, 2248, 80, 1, gScenarioBufferData95_APU_CONVOLUTION, 80, gScenarioKernelData95_APU_CONVOLUTION, 4},
{2, 5, 172, 80, 1, gScenarioBufferData96_APU_CONVOLUTION, 80, gScenarioKernelData96_APU_CONVOLUTION, 4},
{4, 5, 256, 80, 1, gScenarioBufferData97_APU_CONVOLUTION, 80, gScenarioKernelData97_APU_CONVOLUTION, 4},
{6, 5, 340, 80, 1, gScenarioBufferData98_APU_CONVOLUTION, 80, gScenarioKernelData98_APU_CONVOLUTION, 4},
{8, 5, 424, 80, 1, gScenarioBufferData99_APU_CONVOLUTION, 80, gScenarioKernelData99_APU_CONVOLUTION, 4},
{10, 5, 508, 80, 1, gScenarioBufferData100_APU_CONVOLUTION, 80, gScenarioKernelData100_APU_CONVOLUTION, 4},
{12, 5, 592, 80, 1, gScenarioBufferData101_APU_CONVOLUTION, 80, gScenarioKernelData101_APU_CONVOLUTION, 4},
{14, 5, 676, 80, 1, gScenarioBufferData102_APU_CONVOLUTION, 80, gScenarioKernelData102_APU_CONVOLUTION, 4},
{16, 5, 760, 80, 1, gScenarioBufferData103_APU_CONVOLUTION, 80, gScenarioKernelData103_APU_CONVOLUTION, 4},
{18, 5, 844, 80, 1, gScenarioBufferData104_APU_CONVOLUTION, 80, gScenarioKernelData104_APU_CONVOLUTION, 4},
{20, 5, 928, 80, 1, gScenarioBufferData105_APU_CONVOLUTION, 80, gScenarioKernelData105_APU_CONVOLUTION, 4},
{22, 5, 1012, 80, 1, gScenarioBufferData106_APU_CONVOLUTION, 80, gScenarioKernelData106_APU_CONVOLUTION, 4},
{24, 5, 1096, 80, 1, gScenarioBufferData107_APU_CONVOLUTION, 80, gScenarioKernelData107_APU_CONVOLUTION, 4},
{26, 5, 1180, 80, 1, gScenarioBufferData108_APU_CONVOLUTION, 80, gScenarioKernelData108_APU_CONVOLUTION, 4},
{28, 5, 1264, 80, 1, gScenarioBufferData109_APU_CONVOLUTION, 80, gScenarioKernelData109_APU_CONVOLUTION, 4},
{30, 5, 1348, 80, 1, gScenarioBufferData110_APU_CONVOLUTION, 80, gScenarioKernelData110_APU_CONVOLUTION, 4},
{32, 5, 1432, 80, 1, gScenarioBufferData111_APU_CONVOLUTION, 80, gScenarioKernelData111_APU_CONVOLUTION, 4},
{36, 5, 1600, 80, 1, gScenarioBufferData112_APU_CONVOLUTION, 80, gScenarioKernelData112_APU_CONVOLUTION, 4},
{40, 5, 1768, 80, 1, gScenarioBufferData113_APU_CONVOLUTION, 80, gScenarioKernelData113_APU_CONVOLUTION, 4},
{44, 5, 1936, 80, 1, gScenarioBufferData114_APU_CONVOLUTION, 80, gScenarioKernelData114_APU_CONVOLUTION, 4},
{48, 5, 2104, 80, 1, gScenarioBufferData115_APU_CONVOLUTION, 80, gScenarioKernelData115_APU_CONVOLUTION, 4},
{52, 5, 2272, 80, 1, gScenarioBufferData116_APU_CONVOLUTION, 80, gScenarioKernelData116_APU_CONVOLUTION, 4},
{56, 5, 2440, 80, 1, gScenarioBufferData117_APU_CONVOLUTION, 80, gScenarioKernelData117_APU_CONVOLUTION, 4},
{60, 5, 2608, 80, 1, gScenarioBufferData118_APU_CONVOLUTION, 80, gScenarioKernelData118_APU_CONVOLUTION, 4},
{64, 5, 2776, 80, 1, gScenarioBufferData119_APU_CONVOLUTION, 80, gScenarioKernelData119_APU_CONVOLUTION, 4},
{2, 6, 204, 80, 1, gScenarioBufferData120_APU_CONVOLUTION, 80, gScenarioKernelData120_APU_CONVOLUTION, 4},
{4, 6, 304, 80, 1, gScenarioBufferData121_APU_CONVOLUTION, 80, gScenarioKernelData121_APU_CONVOLUTION, 4},
{6, 6, 404, 80, 1, gScenarioBufferData122_APU_CONVOLUTION, 80, gScenarioKernelData122_APU_CONVOLUTION, 4},
{8, 6, 504, 80, 1, gScenarioBufferData123_APU_CONVOLUTION, 80, gScenarioKernelData123_APU_CONVOLUTION, 4},
{10, 6, 604, 80, 1, gScenarioBufferData124_APU_CONVOLUTION, 80, gScenarioKernelData124_APU_CONVOLUTION, 4},
{12, 6, 704, 80, 1, gScenarioBufferData125_APU_CONVOLUTION, 80, gScenarioKernelData125_APU_CONVOLUTION, 4},
{14, 6, 804, 80, 1, gScenarioBufferData126_APU_CONVOLUTION, 80, gScenarioKernelData126_APU_CONVOLUTION, 4},
{16, 6, 904, 80, 1, gScenarioBufferData127_APU_CONVOLUTION, 80, gScenarioKernelData127_APU_CONVOLUTION, 4},
{18, 6, 1004, 80, 1, gScenarioBufferData128_APU_CONVOLUTION, 80, gScenarioKernelData128_APU_CONVOLUTION, 4},
{20, 6, 1104, 80, 1, gScenarioBufferData129_APU_CONVOLUTION, 80, gScenarioKernelData129_APU_CONVOLUTION, 4},
{22, 6, 1204, 80, 1, gScenarioBufferData130_APU_CONVOLUTION, 80, gScenarioKernelData130_APU_CONVOLUTION, 4},
{24, 6, 1304, 80, 1, gScenarioBufferData131_APU_CONVOLUTION, 80, gScenarioKernelData131_APU_CONVOLUTION, 4},
{26, 6, 1404, 80, 1, gScenarioBufferData132_APU_CONVOLUTION, 80, gScenarioKernelData132_APU_CONVOLUTION, 4},
{28, 6, 1504, 80, 1, gScenarioBufferData133_APU_CONVOLUTION, 80, gScenarioKernelData133_APU_CONVOLUTION, 4},
{30, 6, 1604, 80, 1, gScenarioBufferData134_APU_CONVOLUTION, 80, gScenarioKernelData134_APU_CONVOLUTION, 4},
{32, 6, 1704, 80, 1, gScenarioBufferData135_APU_CONVOLUTION, 80, gScenarioKernelData135_APU_CONVOLUTION, 4},
{36, 6, 1904, 80, 1, gScenarioBufferData136_APU_CONVOLUTION, 80, gScenarioKernelData136_APU_CONVOLUTION, 4},
{40, 6, 2104, 80, 1, gScenarioBufferData137_APU_CONVOLUTION, 80, gScenarioKernelData137_APU_CONVOLUTION, 4},
{44, 6, 2304, 80, 1, gScenarioBufferData138_APU_CONVOLUTION, 80, gScenarioKernelData138_APU_CONVOLUTION, 4},
{48, 6, 2504, 80, 1, gScenarioBufferData139_APU_CONVOLUTION, 80, gScenarioKernelData139_APU_CONVOLUTION, 4},
{52, 6, 2704, 80, 1, gScenarioBufferData140_APU_CONVOLUTION, 80, gScenarioKernelData140_APU_CONVOLUTION, 4},
{56, 6, 2904, 80, 1, gScenarioBufferData141_APU_CONVOLUTION, 80, gScenarioKernelData141_APU_CONVOLUTION, 4},
{60, 6, 3104, 80, 1, gScenarioBufferData142_APU_CONVOLUTION, 80, gScenarioKernelData142_APU_CONVOLUTION, 4},
{64, 6, 3304, 80, 1, gScenarioBufferData143_APU_CONVOLUTION, 80, gScenarioKernelData143_APU_CONVOLUTION, 4},
{2, 8, 268, 80, 1, gScenarioBufferData144_APU_CONVOLUTION, 80, gScenarioKernelData144_APU_CONVOLUTION, 4},
{4, 8, 400, 80, 1, gScenarioBufferData145_APU_CONVOLUTION, 80, gScenarioKernelData145_APU_CONVOLUTION, 4},
{6, 8, 532, 80, 1, gScenarioBufferData146_APU_CONVOLUTION, 80, gScenarioKernelData146_APU_CONVOLUTION, 4},
{8, 8, 664, 80, 1, gScenarioBufferData147_APU_CONVOLUTION, 80, gScenarioKernelData147_APU_CONVOLUTION, 4},
{10, 8, 796, 80, 1, gScenarioBufferData148_APU_CONVOLUTION, 80, gScenarioKernelData148_APU_CONVOLUTION, 4},
{12, 8, 928, 80, 1, gScenarioBufferData149_APU_CONVOLUTION, 80, gScenarioKernelData149_APU_CONVOLUTION, 4},
{14, 8, 1060, 80, 1, gScenarioBufferData150_APU_CONVOLUTION, 80, gScenarioKernelData150_APU_CONVOLUTION, 4},
{16, 8, 1192, 80, 1, gScenarioBufferData151_APU_CONVOLUTION, 80, gScenarioKernelData151_APU_CONVOLUTION, 4},
{18, 8, 1324, 80, 1, gScenarioBufferData152_APU_CONVOLUTION, 80, gScenarioKernelData152_APU_CONVOLUTION, 4},
{20, 8, 1456, 80, 1, gScenarioBufferData153_APU_CONVOLUTION, 80, gScenarioKernelData153_APU_CONVOLUTION, 4},
{22, 8, 1588, 80, 1, gScenarioBufferData154_APU_CONVOLUTION, 80, gScenarioKernelData154_APU_CONVOLUTION, 4},
{24, 8, 1720, 80, 1, gScenarioBufferData155_APU_CONVOLUTION, 80, gScenarioKernelData155_APU_CONVOLUTION, 4},
{26, 8, 1852, 80, 1, gScenarioBufferData156_APU_CONVOLUTION, 80, gScenarioKernelData156_APU_CONVOLUTION, 4},
{28, 8, 1984, 80, 1, gScenarioBufferData157_APU_CONVOLUTION, 80, gScenarioKernelData157_APU_CONVOLUTION, 4},
{30, 8, 2116, 80, 1, gScenarioBufferData158_APU_CONVOLUTION, 80, gScenarioKernelData158_APU_CONVOLUTION, 4},
{32, 8, 2248, 80, 1, gScenarioBufferData159_APU_CONVOLUTION, 80, gScenarioKernelData159_APU_CONVOLUTION, 4},
{36, 8, 2512, 80, 1, gScenarioBufferData160_APU_CONVOLUTION, 80, gScenarioKernelData160_APU_CONVOLUTION, 4},
{40, 8, 2776, 80, 1, gScenarioBufferData161_APU_CONVOLUTION, 80, gScenarioKernelData161_APU_CONVOLUTION, 4},
{44, 8, 3040, 80, 1, gScenarioBufferData162_APU_CONVOLUTION, 80, gScenarioKernelData162_APU_CONVOLUTION, 4},
{48, 8, 3304, 80, 1, gScenarioBufferData163_APU_CONVOLUTION, 80, gScenarioKernelData163_APU_CONVOLUTION, 4},
{52, 8, 3568, 80, 1, gScenarioBufferData164_APU_CONVOLUTION, 80, gScenarioKernelData164_APU_CONVOLUTION, 4},
{56, 8, 3832, 80, 1, gScenarioBufferData165_APU_CONVOLUTION, 80, gScenarioKernelData165_APU_CONVOLUTION, 4},
{2, 10, 332, 80, 1, gScenarioBufferData166_APU_CONVOLUTION, 80, gScenarioKernelData166_APU_CONVOLUTION, 4},
{4, 10, 496, 80, 1, gScenarioBufferData167_APU_CONVOLUTION, 80, gScenarioKernelData167_APU_CONVOLUTION, 4},
{6, 10, 660, 80, 1, gScenarioBufferData168_APU_CONVOLUTION, 80, gScenarioKernelData168_APU_CONVOLUTION, 4},
{8, 10, 824, 80, 1, gScenarioBufferData169_APU_CONVOLUTION, 80, gScenarioKernelData169_APU_CONVOLUTION, 4},
{10, 10, 988, 80, 1, gScenarioBufferData170_APU_CONVOLUTION, 80, gScenarioKernelData170_APU_CONVOLUTION, 4},
{12, 10, 1152, 80, 1, gScenarioBufferData171_APU_CONVOLUTION, 80, gScenarioKernelData171_APU_CONVOLUTION, 4},
{14, 10, 1316, 80, 1, gScenarioBufferData172_APU_CONVOLUTION, 80, gScenarioKernelData172_APU_CONVOLUTION, 4},
{16, 10, 1480, 80, 1, gScenarioBufferData173_APU_CONVOLUTION, 80, gScenarioKernelData173_APU_CONVOLUTION, 4},
{18, 10, 1644, 80, 1, gScenarioBufferData174_APU_CONVOLUTION, 80, gScenarioKernelData174_APU_CONVOLUTION, 4},
{20, 10, 1808, 80, 1, gScenarioBufferData175_APU_CONVOLUTION, 80, gScenarioKernelData175_APU_CONVOLUTION, 4},
{22, 10, 1972, 80, 1, gScenarioBufferData176_APU_CONVOLUTION, 80, gScenarioKernelData176_APU_CONVOLUTION, 4},
{24, 10, 2136, 80, 1, gScenarioBufferData177_APU_CONVOLUTION, 80, gScenarioKernelData177_APU_CONVOLUTION, 4},
{26, 10, 2300, 80, 1, gScenarioBufferData178_APU_CONVOLUTION, 80, gScenarioKernelData178_APU_CONVOLUTION, 4},
{28, 10, 2464, 80, 1, gScenarioBufferData179_APU_CONVOLUTION, 80, gScenarioKernelData179_APU_CONVOLUTION, 4},
{30, 10, 2628, 80, 1, gScenarioBufferData180_APU_CONVOLUTION, 80, gScenarioKernelData180_APU_CONVOLUTION, 4},
{32, 10, 2792, 80, 1, gScenarioBufferData181_APU_CONVOLUTION, 80, gScenarioKernelData181_APU_CONVOLUTION, 4},
{36, 10, 3120, 80, 1, gScenarioBufferData182_APU_CONVOLUTION, 80, gScenarioKernelData182_APU_CONVOLUTION, 4},
{40, 10, 3448, 80, 1, gScenarioBufferData183_APU_CONVOLUTION, 80, gScenarioKernelData183_APU_CONVOLUTION, 4},
{44, 10, 3776, 80, 1, gScenarioBufferData184_APU_CONVOLUTION, 80, gScenarioKernelData184_APU_CONVOLUTION, 4},
{2, 12, 396, 80, 1, gScenarioBufferData185_APU_CONVOLUTION, 80, gScenarioKernelData185_APU_CONVOLUTION, 4},
{4, 12, 592, 80, 1, gScenarioBufferData186_APU_CONVOLUTION, 80, gScenarioKernelData186_APU_CONVOLUTION, 4},
{6, 12, 788, 80, 1, gScenarioBufferData187_APU_CONVOLUTION, 80, gScenarioKernelData187_APU_CONVOLUTION, 4},
{8, 12, 984, 80, 1, gScenarioBufferData188_APU_CONVOLUTION, 80, gScenarioKernelData188_APU_CONVOLUTION, 4},
{10, 12, 1180, 80, 1, gScenarioBufferData189_APU_CONVOLUTION, 80, gScenarioKernelData189_APU_CONVOLUTION, 4},
{12, 12, 1376, 80, 1, gScenarioBufferData190_APU_CONVOLUTION, 80, gScenarioKernelData190_APU_CONVOLUTION, 4},
{14, 12, 1572, 80, 1, gScenarioBufferData191_APU_CONVOLUTION, 80, gScenarioKernelData191_APU_CONVOLUTION, 4},
{16, 12, 1768, 80, 1, gScenarioBufferData192_APU_CONVOLUTION, 80, gScenarioKernelData192_APU_CONVOLUTION, 4},
{18, 12, 1964, 80, 1, gScenarioBufferData193_APU_CONVOLUTION, 80, gScenarioKernelData193_APU_CONVOLUTION, 4},
{20, 12, 2160, 80, 1, gScenarioBufferData194_APU_CONVOLUTION, 80, gScenarioKernelData194_APU_CONVOLUTION, 4},
{22, 12, 2356, 80, 1, gScenarioBufferData195_APU_CONVOLUTION, 80, gScenarioKernelData195_APU_CONVOLUTION, 4},
{24, 12, 2552, 80, 1, gScenarioBufferData196_APU_CONVOLUTION, 80, gScenarioKernelData196_APU_CONVOLUTION, 4},
{26, 12, 2748, 80, 1, gScenarioBufferData197_APU_CONVOLUTION, 80, gScenarioKernelData197_APU_CONVOLUTION, 4},
{28, 12, 2944, 80, 1, gScenarioBufferData198_APU_CONVOLUTION, 80, gScenarioKernelData198_APU_CONVOLUTION, 4},
{30, 12, 3140, 80, 1, gScenarioBufferData199_APU_CONVOLUTION, 80, gScenarioKernelData199_APU_CONVOLUTION, 4},
{32, 12, 3336, 80, 1, gScenarioBufferData200_APU_CONVOLUTION, 80, gScenarioKernelData200_APU_CONVOLUTION, 4},
{36, 12, 3728, 80, 1, gScenarioBufferData201_APU_CONVOLUTION, 80, gScenarioKernelData201_APU_CONVOLUTION, 4},
{2, 14, 460, 80, 1, gScenarioBufferData202_APU_CONVOLUTION, 80, gScenarioKernelData202_APU_CONVOLUTION, 4},
{4, 14, 688, 80, 1, gScenarioBufferData203_APU_CONVOLUTION, 80, gScenarioKernelData203_APU_CONVOLUTION, 4},
{6, 14, 916, 80, 1, gScenarioBufferData204_APU_CONVOLUTION, 80, gScenarioKernelData204_APU_CONVOLUTION, 4},
{8, 14, 1144, 80, 1, gScenarioBufferData205_APU_CONVOLUTION, 80, gScenarioKernelData205_APU_CONVOLUTION, 4},
{10, 14, 1372, 80, 1, gScenarioBufferData206_APU_CONVOLUTION, 80, gScenarioKernelData206_APU_CONVOLUTION, 4},
{12, 14, 1600, 80, 1, gScenarioBufferData207_APU_CONVOLUTION, 80, gScenarioKernelData207_APU_CONVOLUTION, 4},
{14, 14, 1828, 80, 1, gScenarioBufferData208_APU_CONVOLUTION, 80, gScenarioKernelData208_APU_CONVOLUTION, 4},
{16, 14, 2056, 80, 1, gScenarioBufferData209_APU_CONVOLUTION, 80, gScenarioKernelData209_APU_CONVOLUTION, 4},
{18, 14, 2284, 80, 1, gScenarioBufferData210_APU_CONVOLUTION, 80, gScenarioKernelData210_APU_CONVOLUTION, 4},
{20, 14, 2512, 80, 1, gScenarioBufferData211_APU_CONVOLUTION, 80, gScenarioKernelData211_APU_CONVOLUTION, 4},
{22, 14, 2740, 80, 1, gScenarioBufferData212_APU_CONVOLUTION, 80, gScenarioKernelData212_APU_CONVOLUTION, 4},
{24, 14, 2968, 80, 1, gScenarioBufferData213_APU_CONVOLUTION, 80, gScenarioKernelData213_APU_CONVOLUTION, 4},
{26, 14, 3196, 80, 1, gScenarioBufferData214_APU_CONVOLUTION, 80, gScenarioKernelData214_APU_CONVOLUTION, 4},
{28, 14, 3424, 80, 1, gScenarioBufferData215_APU_CONVOLUTION, 80, gScenarioKernelData215_APU_CONVOLUTION, 4},
{30, 14, 3652, 80, 1, gScenarioBufferData216_APU_CONVOLUTION, 80, gScenarioKernelData216_APU_CONVOLUTION, 4},
{2, 16, 524, 80, 1, gScenarioBufferData217_APU_CONVOLUTION, 80, gScenarioKernelData217_APU_CONVOLUTION, 4},
{4, 16, 784, 80, 1, gScenarioBufferData218_APU_CONVOLUTION, 80, gScenarioKernelData218_APU_CONVOLUTION, 4},
{6, 16, 1044, 80, 1, gScenarioBufferData219_APU_CONVOLUTION, 80, gScenarioKernelData219_APU_CONVOLUTION, 4},
{8, 16, 1304, 80, 1, gScenarioBufferData220_APU_CONVOLUTION, 80, gScenarioKernelData220_APU_CONVOLUTION, 4},
{10, 16, 1564, 80, 1, gScenarioBufferData221_APU_CONVOLUTION, 80, gScenarioKernelData221_APU_CONVOLUTION, 4},
{12, 16, 1824, 80, 1, gScenarioBufferData222_APU_CONVOLUTION, 80, gScenarioKernelData222_APU_CONVOLUTION, 4},
{14, 16, 2084, 80, 1, gScenarioBufferData223_APU_CONVOLUTION, 80, gScenarioKernelData223_APU_CONVOLUTION, 4},
{16, 16, 2344, 80, 1, gScenarioBufferData224_APU_CONVOLUTION, 80, gScenarioKernelData224_APU_CONVOLUTION, 4},
{18, 16, 2604, 80, 1, gScenarioBufferData225_APU_CONVOLUTION, 80, gScenarioKernelData225_APU_CONVOLUTION, 4},
{20, 16, 2864, 80, 1, gScenarioBufferData226_APU_CONVOLUTION, 80, gScenarioKernelData226_APU_CONVOLUTION, 4},
{22, 16, 3124, 80, 1, gScenarioBufferData227_APU_CONVOLUTION, 80, gScenarioKernelData227_APU_CONVOLUTION, 4},
{24, 16, 3384, 80, 1, gScenarioBufferData228_APU_CONVOLUTION, 80, gScenarioKernelData228_APU_CONVOLUTION, 4},
{26, 16, 3644, 80, 1, gScenarioBufferData229_APU_CONVOLUTION, 80, gScenarioKernelData229_APU_CONVOLUTION, 4},
{2, 18, 588, 80, 1, gScenarioBufferData230_APU_CONVOLUTION, 80, gScenarioKernelData230_APU_CONVOLUTION, 4},
{4, 18, 880, 80, 1, gScenarioBufferData231_APU_CONVOLUTION, 80, gScenarioKernelData231_APU_CONVOLUTION, 4},
{6, 18, 1172, 80, 1, gScenarioBufferData232_APU_CONVOLUTION, 80, gScenarioKernelData232_APU_CONVOLUTION, 4},
{8, 18, 1464, 80, 1, gScenarioBufferData233_APU_CONVOLUTION, 80, gScenarioKernelData233_APU_CONVOLUTION, 4},
{10, 18, 1756, 80, 1, gScenarioBufferData234_APU_CONVOLUTION, 80, gScenarioKernelData234_APU_CONVOLUTION, 4},
{12, 18, 2048, 80, 1, gScenarioBufferData235_APU_CONVOLUTION, 80, gScenarioKernelData235_APU_CONVOLUTION, 4},
{14, 18, 2340, 80, 1, gScenarioBufferData236_APU_CONVOLUTION, 80, gScenarioKernelData236_APU_CONVOLUTION, 4},
{16, 18, 2632, 80, 1, gScenarioBufferData237_APU_CONVOLUTION, 80, gScenarioKernelData237_APU_CONVOLUTION, 4},
{18, 18, 2924, 80, 1, gScenarioBufferData238_APU_CONVOLUTION, 80, gScenarioKernelData238_APU_CONVOLUTION, 4},
{20, 18, 3216, 80, 1, gScenarioBufferData239_APU_CONVOLUTION, 80, gScenarioKernelData239_APU_CONVOLUTION, 4},
{22, 18, 3508, 80, 1, gScenarioBufferData240_APU_CONVOLUTION, 80, gScenarioKernelData240_APU_CONVOLUTION, 4},
{24, 18, 3800, 80, 1, gScenarioBufferData241_APU_CONVOLUTION, 80, gScenarioKernelData241_APU_CONVOLUTION, 4},
{2, 20, 652, 80, 1, gScenarioBufferData242_APU_CONVOLUTION, 80, gScenarioKernelData242_APU_CONVOLUTION, 4},
{4, 20, 976, 80, 1, gScenarioBufferData243_APU_CONVOLUTION, 80, gScenarioKernelData243_APU_CONVOLUTION, 4},
{6, 20, 1300, 80, 1, gScenarioBufferData244_APU_CONVOLUTION, 80, gScenarioKernelData244_APU_CONVOLUTION, 4},
{8, 20, 1624, 80, 1, gScenarioBufferData245_APU_CONVOLUTION, 80, gScenarioKernelData245_APU_CONVOLUTION, 4},
{10, 20, 1948, 80, 1, gScenarioBufferData246_APU_CONVOLUTION, 80, gScenarioKernelData246_APU_CONVOLUTION, 4},
{12, 20, 2272, 80, 1, gScenarioBufferData247_APU_CONVOLUTION, 80, gScenarioKernelData247_APU_CONVOLUTION, 4},
{14, 20, 2596, 80, 1, gScenarioBufferData248_APU_CONVOLUTION, 80, gScenarioKernelData248_APU_CONVOLUTION, 4},
{16, 20, 2920, 80, 1, gScenarioBufferData249_APU_CONVOLUTION, 80, gScenarioKernelData249_APU_CONVOLUTION, 4},
{18, 20, 3244, 80, 1, gScenarioBufferData250_APU_CONVOLUTION, 80, gScenarioKernelData250_APU_CONVOLUTION, 4},
{20, 20, 3568, 80, 1, gScenarioBufferData251_APU_CONVOLUTION, 80, gScenarioKernelData251_APU_CONVOLUTION, 4},
{2, 22, 716, 80, 1, gScenarioBufferData252_APU_CONVOLUTION, 80, gScenarioKernelData252_APU_CONVOLUTION, 4},
{4, 22, 1072, 80, 1, gScenarioBufferData253_APU_CONVOLUTION, 80, gScenarioKernelData253_APU_CONVOLUTION, 4},
{6, 22, 1428, 80, 1, gScenarioBufferData254_APU_CONVOLUTION, 80, gScenarioKernelData254_APU_CONVOLUTION, 4},
{8, 22, 1784, 80, 1, gScenarioBufferData255_APU_CONVOLUTION, 80, gScenarioKernelData255_APU_CONVOLUTION, 4},
{10, 22, 2140, 80, 1, gScenarioBufferData256_APU_CONVOLUTION, 80, gScenarioKernelData256_APU_CONVOLUTION, 4},
{12, 22, 2496, 80, 1, gScenarioBufferData257_APU_CONVOLUTION, 80, gScenarioKernelData257_APU_CONVOLUTION, 4},
{14, 22, 2852, 80, 1, gScenarioBufferData258_APU_CONVOLUTION, 80, gScenarioKernelData258_APU_CONVOLUTION, 4},
{16, 22, 3208, 80, 1, gScenarioBufferData259_APU_CONVOLUTION, 80, gScenarioKernelData259_APU_CONVOLUTION, 4},
{18, 22, 3564, 80, 1, gScenarioBufferData260_APU_CONVOLUTION, 80, gScenarioKernelData260_APU_CONVOLUTION, 4},
{2, 24, 780, 80, 1, gScenarioBufferData261_APU_CONVOLUTION, 80, gScenarioKernelData261_APU_CONVOLUTION, 4},
{4, 24, 1168, 80, 1, gScenarioBufferData262_APU_CONVOLUTION, 80, gScenarioKernelData262_APU_CONVOLUTION, 4},
{6, 24, 1556, 80, 1, gScenarioBufferData263_APU_CONVOLUTION, 80, gScenarioKernelData263_APU_CONVOLUTION, 4},
{8, 24, 1944, 80, 1, gScenarioBufferData264_APU_CONVOLUTION, 80, gScenarioKernelData264_APU_CONVOLUTION, 4},
{10, 24, 2332, 80, 1, gScenarioBufferData265_APU_CONVOLUTION, 80, gScenarioKernelData265_APU_CONVOLUTION, 4},
{12, 24, 2720, 80, 1, gScenarioBufferData266_APU_CONVOLUTION, 80, gScenarioKernelData266_APU_CONVOLUTION, 4},
{14, 24, 3108, 80, 1, gScenarioBufferData267_APU_CONVOLUTION, 80, gScenarioKernelData267_APU_CONVOLUTION, 4},
{16, 24, 3496, 80, 1, gScenarioBufferData268_APU_CONVOLUTION, 80, gScenarioKernelData268_APU_CONVOLUTION, 4},
{2, 26, 844, 80, 1, gScenarioBufferData269_APU_CONVOLUTION, 80, gScenarioKernelData269_APU_CONVOLUTION, 4},
{4, 26, 1264, 80, 1, gScenarioBufferData270_APU_CONVOLUTION, 80, gScenarioKernelData270_APU_CONVOLUTION, 4},
{6, 26, 1684, 80, 1, gScenarioBufferData271_APU_CONVOLUTION, 80, gScenarioKernelData271_APU_CONVOLUTION, 4},
{8, 26, 2104, 80, 1, gScenarioBufferData272_APU_CONVOLUTION, 80, gScenarioKernelData272_APU_CONVOLUTION, 4},
{10, 26, 2524, 80, 1, gScenarioBufferData273_APU_CONVOLUTION, 80, gScenarioKernelData273_APU_CONVOLUTION, 4},
{12, 26, 2944, 80, 1, gScenarioBufferData274_APU_CONVOLUTION, 80, gScenarioKernelData274_APU_CONVOLUTION, 4},
{14, 26, 3364, 80, 1, gScenarioBufferData275_APU_CONVOLUTION, 80, gScenarioKernelData275_APU_CONVOLUTION, 4},
{16, 26, 3784, 80, 1, gScenarioBufferData276_APU_CONVOLUTION, 80, gScenarioKernelData276_APU_CONVOLUTION, 4},
{2, 28, 908, 80, 1, gScenarioBufferData277_APU_CONVOLUTION, 80, gScenarioKernelData277_APU_CONVOLUTION, 4},
{4, 28, 1360, 80, 1, gScenarioBufferData278_APU_CONVOLUTION, 80, gScenarioKernelData278_APU_CONVOLUTION, 4},
{6, 28, 1812, 80, 1, gScenarioBufferData279_APU_CONVOLUTION, 80, gScenarioKernelData279_APU_CONVOLUTION, 4},
{8, 28, 2264, 80, 1, gScenarioBufferData280_APU_CONVOLUTION, 80, gScenarioKernelData280_APU_CONVOLUTION, 4},
{10, 28, 2716, 80, 1, gScenarioBufferData281_APU_CONVOLUTION, 80, gScenarioKernelData281_APU_CONVOLUTION, 4},
{12, 28, 3168, 80, 1, gScenarioBufferData282_APU_CONVOLUTION, 80, gScenarioKernelData282_APU_CONVOLUTION, 4},
{14, 28, 3620, 80, 1, gScenarioBufferData283_APU_CONVOLUTION, 80, gScenarioKernelData283_APU_CONVOLUTION, 4},
{2, 30, 972, 80, 1, gScenarioBufferData284_APU_CONVOLUTION, 80, gScenarioKernelData284_APU_CONVOLUTION, 4},
{4, 30, 1456, 80, 1, gScenarioBufferData285_APU_CONVOLUTION, 80, gScenarioKernelData285_APU_CONVOLUTION, 4},
{6, 30, 1940, 80, 1, gScenarioBufferData286_APU_CONVOLUTION, 80, gScenarioKernelData286_APU_CONVOLUTION, 4},
{8, 30, 2424, 80, 1, gScenarioBufferData287_APU_CONVOLUTION, 80, gScenarioKernelData287_APU_CONVOLUTION, 4},
{10, 30, 2908, 80, 1, gScenarioBufferData288_APU_CONVOLUTION, 80, gScenarioKernelData288_APU_CONVOLUTION, 4},
{12, 30, 3392, 80, 1, gScenarioBufferData289_APU_CONVOLUTION, 80, gScenarioKernelData289_APU_CONVOLUTION, 4},
{2, 32, 1036, 80, 1, gScenarioBufferData290_APU_CONVOLUTION, 80, gScenarioKernelData290_APU_CONVOLUTION, 4},
{4, 32, 1552, 80, 1, gScenarioBufferData291_APU_CONVOLUTION, 80, gScenarioKernelData291_APU_CONVOLUTION, 4},
{6, 32, 2068, 80, 1, gScenarioBufferData292_APU_CONVOLUTION, 80, gScenarioKernelData292_APU_CONVOLUTION, 4},
{8, 32, 2584, 80, 1, gScenarioBufferData293_APU_CONVOLUTION, 80, gScenarioKernelData293_APU_CONVOLUTION, 4},
{10, 32, 3100, 80, 1, gScenarioBufferData294_APU_CONVOLUTION, 80, gScenarioKernelData294_APU_CONVOLUTION, 4},
{12, 32, 3616, 80, 1, gScenarioBufferData295_APU_CONVOLUTION, 80, gScenarioKernelData295_APU_CONVOLUTION, 4}
};

static acf_scenario_list gScenarioList_APU_CONVOLUTION = {
296, //number of scenarios
gScenarioArray_APU_CONVOLUTION};
//**************************************************************

class APU_CONVOLUTION : public ACF_Process_APU
{

public:
   APU_CONVOLUTION(int32_t apex_id = 0) : ACF_Process_APU(apex_id)
   {} 

   int32_t Initialize()
   {
      int32_t lRetVal = 0;

      if (!Initialized()) //initialization steps that only need to occur once
      {
         SetProcessIdentifier("APU_CONVOLUTION");
         SetApuLoadInfo(APU_CONVOLUTION_LOAD_SEGMENTS,
                        APU_CONVOLUTION_LOAD_PMEM, APU_CONVOLUTION_LOAD_PMEM_SIZE,
                        APU_CONVOLUTION_LOAD_DMEM, APU_CONVOLUTION_LOAD_DMEM_SIZE,
                        0, 0); //assuming _LOAD_CMEM does not exist

         lRetVal = NonParametricInitialization();
         if (lRetVal != 0) return lRetVal;

         FlagSpatialDep();

         //***NOTE: the order in which the following ports are added is meaningful; do not change!

         AddPort("ConvGraph_Input", icp::DATATYPE_08U, 1, 1, 2, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0);
         AddPort("ConvGr_In_Filt", icp::DATATYPE_16S, 1, 1, 3, 3, 0, 1, 1, 0, 1, 0, 2, 0, 0, 0, 0, 0);
         AddPort("ConvGraph_FiltScale", icp::DATATYPE_16S, 1, 1, 1, 1, 0, 1, 1, 0, 1, 0, 3, 0, 0, 0, 0, 0);
         AddPort("ConvGr_FltSymmFl", icp::DATATYPE_16U, 1, 1, 1, 1, 0, 1, 1, 0, 1, 0, 4, 0, 0, 0, 0, 0);
         AddPort("ConvGraph_Output", icp::DATATYPE_16S, 1, 1, 2, 1, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0);

         CfgScenarios(&gScenarioList_APU_CONVOLUTION);

         FlagAsInitialized();
      }

      lRetVal = SelectApuConfiguration(mApuCfg, mApexId); //by default mApuCfg = ACF_APU_CFG__APEX0_DEFAULT and mApexId = 0

      return lRetVal;
   }
};

#endif //_ACF_PROCESS_APU_APU_CONVOLUTION
