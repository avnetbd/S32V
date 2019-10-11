#ifndef _ACF_PROCESS_APU_HT_BOX_3X3_08U
#define _ACF_PROCESS_APU_HT_BOX_3X3_08U

#include <acf_process_apu.h>
#include <HT_BOX_3X3_08U_APU_LOAD.h> //APU load associated with this process
#include <stdint.h>


//SCENARIO LIST*************************************************
static acf_scenario_buffer_data gScenarioBufferData0_HT_BOX_3X3_08U[] = {{2, 1, 4, 0, {1, 1, 2, 2}}, {2, 1, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData1_HT_BOX_3X3_08U[] = {{4, 1, 4, 0, {1, 1, 2, 2}}, {4, 1, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData2_HT_BOX_3X3_08U[] = {{6, 1, 4, 0, {1, 1, 2, 2}}, {6, 1, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData3_HT_BOX_3X3_08U[] = {{8, 1, 4, 0, {1, 1, 2, 2}}, {8, 1, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData4_HT_BOX_3X3_08U[] = {{10, 1, 4, 0, {1, 1, 2, 2}}, {10, 1, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData5_HT_BOX_3X3_08U[] = {{12, 1, 4, 0, {1, 1, 2, 2}}, {12, 1, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData6_HT_BOX_3X3_08U[] = {{14, 1, 4, 0, {1, 1, 2, 2}}, {14, 1, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData7_HT_BOX_3X3_08U[] = {{16, 1, 4, 0, {1, 1, 2, 2}}, {16, 1, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData8_HT_BOX_3X3_08U[] = {{18, 1, 4, 0, {1, 1, 2, 2}}, {18, 1, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData9_HT_BOX_3X3_08U[] = {{20, 1, 4, 0, {1, 1, 2, 2}}, {20, 1, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData10_HT_BOX_3X3_08U[] = {{22, 1, 4, 0, {1, 1, 2, 2}}, {22, 1, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData11_HT_BOX_3X3_08U[] = {{24, 1, 4, 0, {1, 1, 2, 2}}, {24, 1, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData12_HT_BOX_3X3_08U[] = {{26, 1, 4, 0, {1, 1, 2, 2}}, {26, 1, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData13_HT_BOX_3X3_08U[] = {{28, 1, 4, 0, {1, 1, 2, 2}}, {28, 1, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData14_HT_BOX_3X3_08U[] = {{30, 1, 4, 0, {1, 1, 2, 2}}, {30, 1, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData15_HT_BOX_3X3_08U[] = {{32, 1, 4, 0, {1, 1, 2, 2}}, {32, 1, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData16_HT_BOX_3X3_08U[] = {{36, 1, 4, 0, {1, 1, 2, 2}}, {36, 1, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData17_HT_BOX_3X3_08U[] = {{40, 1, 4, 0, {1, 1, 2, 2}}, {40, 1, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData18_HT_BOX_3X3_08U[] = {{44, 1, 4, 0, {1, 1, 2, 2}}, {44, 1, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData19_HT_BOX_3X3_08U[] = {{48, 1, 4, 0, {1, 1, 2, 2}}, {48, 1, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData20_HT_BOX_3X3_08U[] = {{52, 1, 4, 0, {1, 1, 2, 2}}, {52, 1, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData21_HT_BOX_3X3_08U[] = {{56, 1, 4, 0, {1, 1, 2, 2}}, {56, 1, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData22_HT_BOX_3X3_08U[] = {{60, 1, 4, 0, {1, 1, 2, 2}}, {60, 1, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData23_HT_BOX_3X3_08U[] = {{64, 1, 4, 0, {1, 1, 2, 2}}, {64, 1, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData24_HT_BOX_3X3_08U[] = {{72, 1, 4, 0, {1, 1, 2, 2}}, {72, 1, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData25_HT_BOX_3X3_08U[] = {{80, 1, 4, 0, {1, 1, 2, 2}}, {80, 1, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData26_HT_BOX_3X3_08U[] = {{88, 1, 4, 0, {1, 1, 2, 2}}, {88, 1, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData27_HT_BOX_3X3_08U[] = {{96, 1, 4, 0, {1, 1, 2, 2}}, {96, 1, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData28_HT_BOX_3X3_08U[] = {{104, 1, 4, 0, {1, 1, 2, 2}}, {104, 1, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData29_HT_BOX_3X3_08U[] = {{112, 1, 4, 0, {1, 1, 2, 2}}, {112, 1, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData30_HT_BOX_3X3_08U[] = {{120, 1, 4, 0, {1, 1, 2, 2}}, {120, 1, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData31_HT_BOX_3X3_08U[] = {{128, 1, 4, 0, {1, 1, 2, 2}}, {128, 1, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData32_HT_BOX_3X3_08U[] = {{2, 2, 4, 0, {1, 1, 2, 2}}, {2, 2, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData33_HT_BOX_3X3_08U[] = {{4, 2, 4, 0, {1, 1, 2, 2}}, {4, 2, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData34_HT_BOX_3X3_08U[] = {{6, 2, 4, 0, {1, 1, 2, 2}}, {6, 2, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData35_HT_BOX_3X3_08U[] = {{8, 2, 4, 0, {1, 1, 2, 2}}, {8, 2, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData36_HT_BOX_3X3_08U[] = {{10, 2, 4, 0, {1, 1, 2, 2}}, {10, 2, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData37_HT_BOX_3X3_08U[] = {{12, 2, 4, 0, {1, 1, 2, 2}}, {12, 2, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData38_HT_BOX_3X3_08U[] = {{14, 2, 4, 0, {1, 1, 2, 2}}, {14, 2, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData39_HT_BOX_3X3_08U[] = {{16, 2, 4, 0, {1, 1, 2, 2}}, {16, 2, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData40_HT_BOX_3X3_08U[] = {{18, 2, 4, 0, {1, 1, 2, 2}}, {18, 2, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData41_HT_BOX_3X3_08U[] = {{20, 2, 4, 0, {1, 1, 2, 2}}, {20, 2, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData42_HT_BOX_3X3_08U[] = {{22, 2, 4, 0, {1, 1, 2, 2}}, {22, 2, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData43_HT_BOX_3X3_08U[] = {{24, 2, 4, 0, {1, 1, 2, 2}}, {24, 2, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData44_HT_BOX_3X3_08U[] = {{26, 2, 4, 0, {1, 1, 2, 2}}, {26, 2, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData45_HT_BOX_3X3_08U[] = {{28, 2, 4, 0, {1, 1, 2, 2}}, {28, 2, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData46_HT_BOX_3X3_08U[] = {{30, 2, 4, 0, {1, 1, 2, 2}}, {30, 2, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData47_HT_BOX_3X3_08U[] = {{32, 2, 4, 0, {1, 1, 2, 2}}, {32, 2, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData48_HT_BOX_3X3_08U[] = {{36, 2, 4, 0, {1, 1, 2, 2}}, {36, 2, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData49_HT_BOX_3X3_08U[] = {{40, 2, 4, 0, {1, 1, 2, 2}}, {40, 2, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData50_HT_BOX_3X3_08U[] = {{44, 2, 4, 0, {1, 1, 2, 2}}, {44, 2, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData51_HT_BOX_3X3_08U[] = {{48, 2, 4, 0, {1, 1, 2, 2}}, {48, 2, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData52_HT_BOX_3X3_08U[] = {{52, 2, 4, 0, {1, 1, 2, 2}}, {52, 2, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData53_HT_BOX_3X3_08U[] = {{56, 2, 4, 0, {1, 1, 2, 2}}, {56, 2, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData54_HT_BOX_3X3_08U[] = {{60, 2, 4, 0, {1, 1, 2, 2}}, {60, 2, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData55_HT_BOX_3X3_08U[] = {{64, 2, 4, 0, {1, 1, 2, 2}}, {64, 2, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData56_HT_BOX_3X3_08U[] = {{72, 2, 4, 0, {1, 1, 2, 2}}, {72, 2, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData57_HT_BOX_3X3_08U[] = {{80, 2, 4, 0, {1, 1, 2, 2}}, {80, 2, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData58_HT_BOX_3X3_08U[] = {{88, 2, 4, 0, {1, 1, 2, 2}}, {88, 2, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData59_HT_BOX_3X3_08U[] = {{96, 2, 4, 0, {1, 1, 2, 2}}, {96, 2, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData60_HT_BOX_3X3_08U[] = {{104, 2, 4, 0, {1, 1, 2, 2}}, {104, 2, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData61_HT_BOX_3X3_08U[] = {{112, 2, 4, 0, {1, 1, 2, 2}}, {112, 2, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData62_HT_BOX_3X3_08U[] = {{120, 2, 4, 0, {1, 1, 2, 2}}, {120, 2, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData63_HT_BOX_3X3_08U[] = {{128, 2, 4, 0, {1, 1, 2, 2}}, {128, 2, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData64_HT_BOX_3X3_08U[] = {{2, 3, 4, 0, {1, 1, 2, 2}}, {2, 3, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData65_HT_BOX_3X3_08U[] = {{4, 3, 4, 0, {1, 1, 2, 2}}, {4, 3, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData66_HT_BOX_3X3_08U[] = {{6, 3, 4, 0, {1, 1, 2, 2}}, {6, 3, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData67_HT_BOX_3X3_08U[] = {{8, 3, 4, 0, {1, 1, 2, 2}}, {8, 3, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData68_HT_BOX_3X3_08U[] = {{10, 3, 4, 0, {1, 1, 2, 2}}, {10, 3, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData69_HT_BOX_3X3_08U[] = {{12, 3, 4, 0, {1, 1, 2, 2}}, {12, 3, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData70_HT_BOX_3X3_08U[] = {{14, 3, 4, 0, {1, 1, 2, 2}}, {14, 3, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData71_HT_BOX_3X3_08U[] = {{16, 3, 4, 0, {1, 1, 2, 2}}, {16, 3, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData72_HT_BOX_3X3_08U[] = {{18, 3, 4, 0, {1, 1, 2, 2}}, {18, 3, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData73_HT_BOX_3X3_08U[] = {{20, 3, 4, 0, {1, 1, 2, 2}}, {20, 3, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData74_HT_BOX_3X3_08U[] = {{22, 3, 4, 0, {1, 1, 2, 2}}, {22, 3, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData75_HT_BOX_3X3_08U[] = {{24, 3, 4, 0, {1, 1, 2, 2}}, {24, 3, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData76_HT_BOX_3X3_08U[] = {{26, 3, 4, 0, {1, 1, 2, 2}}, {26, 3, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData77_HT_BOX_3X3_08U[] = {{28, 3, 4, 0, {1, 1, 2, 2}}, {28, 3, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData78_HT_BOX_3X3_08U[] = {{30, 3, 4, 0, {1, 1, 2, 2}}, {30, 3, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData79_HT_BOX_3X3_08U[] = {{32, 3, 4, 0, {1, 1, 2, 2}}, {32, 3, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData80_HT_BOX_3X3_08U[] = {{36, 3, 4, 0, {1, 1, 2, 2}}, {36, 3, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData81_HT_BOX_3X3_08U[] = {{40, 3, 4, 0, {1, 1, 2, 2}}, {40, 3, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData82_HT_BOX_3X3_08U[] = {{44, 3, 4, 0, {1, 1, 2, 2}}, {44, 3, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData83_HT_BOX_3X3_08U[] = {{48, 3, 4, 0, {1, 1, 2, 2}}, {48, 3, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData84_HT_BOX_3X3_08U[] = {{52, 3, 4, 0, {1, 1, 2, 2}}, {52, 3, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData85_HT_BOX_3X3_08U[] = {{56, 3, 4, 0, {1, 1, 2, 2}}, {56, 3, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData86_HT_BOX_3X3_08U[] = {{60, 3, 4, 0, {1, 1, 2, 2}}, {60, 3, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData87_HT_BOX_3X3_08U[] = {{64, 3, 4, 0, {1, 1, 2, 2}}, {64, 3, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData88_HT_BOX_3X3_08U[] = {{72, 3, 4, 0, {1, 1, 2, 2}}, {72, 3, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData89_HT_BOX_3X3_08U[] = {{80, 3, 4, 0, {1, 1, 2, 2}}, {80, 3, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData90_HT_BOX_3X3_08U[] = {{88, 3, 4, 0, {1, 1, 2, 2}}, {88, 3, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData91_HT_BOX_3X3_08U[] = {{96, 3, 4, 0, {1, 1, 2, 2}}, {96, 3, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData92_HT_BOX_3X3_08U[] = {{104, 3, 4, 0, {1, 1, 2, 2}}, {104, 3, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData93_HT_BOX_3X3_08U[] = {{112, 3, 4, 0, {1, 1, 2, 2}}, {112, 3, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData94_HT_BOX_3X3_08U[] = {{120, 3, 4, 0, {1, 1, 2, 2}}, {120, 3, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData95_HT_BOX_3X3_08U[] = {{128, 3, 4, 0, {1, 1, 2, 2}}, {128, 3, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData96_HT_BOX_3X3_08U[] = {{2, 4, 4, 0, {1, 1, 2, 2}}, {2, 4, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData97_HT_BOX_3X3_08U[] = {{4, 4, 4, 0, {1, 1, 2, 2}}, {4, 4, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData98_HT_BOX_3X3_08U[] = {{6, 4, 4, 0, {1, 1, 2, 2}}, {6, 4, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData99_HT_BOX_3X3_08U[] = {{8, 4, 4, 0, {1, 1, 2, 2}}, {8, 4, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData100_HT_BOX_3X3_08U[] = {{10, 4, 4, 0, {1, 1, 2, 2}}, {10, 4, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData101_HT_BOX_3X3_08U[] = {{12, 4, 4, 0, {1, 1, 2, 2}}, {12, 4, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData102_HT_BOX_3X3_08U[] = {{14, 4, 4, 0, {1, 1, 2, 2}}, {14, 4, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData103_HT_BOX_3X3_08U[] = {{16, 4, 4, 0, {1, 1, 2, 2}}, {16, 4, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData104_HT_BOX_3X3_08U[] = {{18, 4, 4, 0, {1, 1, 2, 2}}, {18, 4, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData105_HT_BOX_3X3_08U[] = {{20, 4, 4, 0, {1, 1, 2, 2}}, {20, 4, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData106_HT_BOX_3X3_08U[] = {{22, 4, 4, 0, {1, 1, 2, 2}}, {22, 4, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData107_HT_BOX_3X3_08U[] = {{24, 4, 4, 0, {1, 1, 2, 2}}, {24, 4, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData108_HT_BOX_3X3_08U[] = {{26, 4, 4, 0, {1, 1, 2, 2}}, {26, 4, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData109_HT_BOX_3X3_08U[] = {{28, 4, 4, 0, {1, 1, 2, 2}}, {28, 4, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData110_HT_BOX_3X3_08U[] = {{30, 4, 4, 0, {1, 1, 2, 2}}, {30, 4, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData111_HT_BOX_3X3_08U[] = {{32, 4, 4, 0, {1, 1, 2, 2}}, {32, 4, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData112_HT_BOX_3X3_08U[] = {{36, 4, 4, 0, {1, 1, 2, 2}}, {36, 4, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData113_HT_BOX_3X3_08U[] = {{40, 4, 4, 0, {1, 1, 2, 2}}, {40, 4, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData114_HT_BOX_3X3_08U[] = {{44, 4, 4, 0, {1, 1, 2, 2}}, {44, 4, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData115_HT_BOX_3X3_08U[] = {{48, 4, 4, 0, {1, 1, 2, 2}}, {48, 4, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData116_HT_BOX_3X3_08U[] = {{52, 4, 4, 0, {1, 1, 2, 2}}, {52, 4, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData117_HT_BOX_3X3_08U[] = {{56, 4, 4, 0, {1, 1, 2, 2}}, {56, 4, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData118_HT_BOX_3X3_08U[] = {{60, 4, 4, 0, {1, 1, 2, 2}}, {60, 4, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData119_HT_BOX_3X3_08U[] = {{64, 4, 4, 0, {1, 1, 2, 2}}, {64, 4, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData120_HT_BOX_3X3_08U[] = {{72, 4, 4, 0, {1, 1, 2, 2}}, {72, 4, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData121_HT_BOX_3X3_08U[] = {{80, 4, 4, 0, {1, 1, 2, 2}}, {80, 4, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData122_HT_BOX_3X3_08U[] = {{88, 4, 4, 0, {1, 1, 2, 2}}, {88, 4, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData123_HT_BOX_3X3_08U[] = {{96, 4, 4, 0, {1, 1, 2, 2}}, {96, 4, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData124_HT_BOX_3X3_08U[] = {{104, 4, 4, 0, {1, 1, 2, 2}}, {104, 4, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData125_HT_BOX_3X3_08U[] = {{112, 4, 4, 0, {1, 1, 2, 2}}, {112, 4, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData126_HT_BOX_3X3_08U[] = {{120, 4, 4, 0, {1, 1, 2, 2}}, {120, 4, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData127_HT_BOX_3X3_08U[] = {{128, 4, 4, 0, {1, 1, 2, 2}}, {128, 4, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData128_HT_BOX_3X3_08U[] = {{2, 5, 4, 0, {1, 1, 2, 2}}, {2, 5, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData129_HT_BOX_3X3_08U[] = {{4, 5, 4, 0, {1, 1, 2, 2}}, {4, 5, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData130_HT_BOX_3X3_08U[] = {{6, 5, 4, 0, {1, 1, 2, 2}}, {6, 5, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData131_HT_BOX_3X3_08U[] = {{8, 5, 4, 0, {1, 1, 2, 2}}, {8, 5, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData132_HT_BOX_3X3_08U[] = {{10, 5, 4, 0, {1, 1, 2, 2}}, {10, 5, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData133_HT_BOX_3X3_08U[] = {{12, 5, 4, 0, {1, 1, 2, 2}}, {12, 5, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData134_HT_BOX_3X3_08U[] = {{14, 5, 4, 0, {1, 1, 2, 2}}, {14, 5, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData135_HT_BOX_3X3_08U[] = {{16, 5, 4, 0, {1, 1, 2, 2}}, {16, 5, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData136_HT_BOX_3X3_08U[] = {{18, 5, 4, 0, {1, 1, 2, 2}}, {18, 5, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData137_HT_BOX_3X3_08U[] = {{20, 5, 4, 0, {1, 1, 2, 2}}, {20, 5, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData138_HT_BOX_3X3_08U[] = {{22, 5, 4, 0, {1, 1, 2, 2}}, {22, 5, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData139_HT_BOX_3X3_08U[] = {{24, 5, 4, 0, {1, 1, 2, 2}}, {24, 5, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData140_HT_BOX_3X3_08U[] = {{26, 5, 4, 0, {1, 1, 2, 2}}, {26, 5, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData141_HT_BOX_3X3_08U[] = {{28, 5, 4, 0, {1, 1, 2, 2}}, {28, 5, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData142_HT_BOX_3X3_08U[] = {{30, 5, 4, 0, {1, 1, 2, 2}}, {30, 5, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData143_HT_BOX_3X3_08U[] = {{32, 5, 4, 0, {1, 1, 2, 2}}, {32, 5, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData144_HT_BOX_3X3_08U[] = {{36, 5, 4, 0, {1, 1, 2, 2}}, {36, 5, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData145_HT_BOX_3X3_08U[] = {{40, 5, 4, 0, {1, 1, 2, 2}}, {40, 5, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData146_HT_BOX_3X3_08U[] = {{44, 5, 4, 0, {1, 1, 2, 2}}, {44, 5, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData147_HT_BOX_3X3_08U[] = {{48, 5, 4, 0, {1, 1, 2, 2}}, {48, 5, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData148_HT_BOX_3X3_08U[] = {{52, 5, 4, 0, {1, 1, 2, 2}}, {52, 5, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData149_HT_BOX_3X3_08U[] = {{56, 5, 4, 0, {1, 1, 2, 2}}, {56, 5, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData150_HT_BOX_3X3_08U[] = {{60, 5, 4, 0, {1, 1, 2, 2}}, {60, 5, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData151_HT_BOX_3X3_08U[] = {{64, 5, 4, 0, {1, 1, 2, 2}}, {64, 5, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData152_HT_BOX_3X3_08U[] = {{72, 5, 4, 0, {1, 1, 2, 2}}, {72, 5, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData153_HT_BOX_3X3_08U[] = {{80, 5, 4, 0, {1, 1, 2, 2}}, {80, 5, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData154_HT_BOX_3X3_08U[] = {{88, 5, 4, 0, {1, 1, 2, 2}}, {88, 5, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData155_HT_BOX_3X3_08U[] = {{96, 5, 4, 0, {1, 1, 2, 2}}, {96, 5, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData156_HT_BOX_3X3_08U[] = {{104, 5, 4, 0, {1, 1, 2, 2}}, {104, 5, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData157_HT_BOX_3X3_08U[] = {{112, 5, 4, 0, {1, 1, 2, 2}}, {112, 5, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData158_HT_BOX_3X3_08U[] = {{2, 6, 4, 0, {1, 1, 2, 2}}, {2, 6, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData159_HT_BOX_3X3_08U[] = {{4, 6, 4, 0, {1, 1, 2, 2}}, {4, 6, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData160_HT_BOX_3X3_08U[] = {{6, 6, 4, 0, {1, 1, 2, 2}}, {6, 6, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData161_HT_BOX_3X3_08U[] = {{8, 6, 4, 0, {1, 1, 2, 2}}, {8, 6, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData162_HT_BOX_3X3_08U[] = {{10, 6, 4, 0, {1, 1, 2, 2}}, {10, 6, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData163_HT_BOX_3X3_08U[] = {{12, 6, 4, 0, {1, 1, 2, 2}}, {12, 6, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData164_HT_BOX_3X3_08U[] = {{14, 6, 4, 0, {1, 1, 2, 2}}, {14, 6, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData165_HT_BOX_3X3_08U[] = {{16, 6, 4, 0, {1, 1, 2, 2}}, {16, 6, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData166_HT_BOX_3X3_08U[] = {{18, 6, 4, 0, {1, 1, 2, 2}}, {18, 6, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData167_HT_BOX_3X3_08U[] = {{20, 6, 4, 0, {1, 1, 2, 2}}, {20, 6, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData168_HT_BOX_3X3_08U[] = {{22, 6, 4, 0, {1, 1, 2, 2}}, {22, 6, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData169_HT_BOX_3X3_08U[] = {{24, 6, 4, 0, {1, 1, 2, 2}}, {24, 6, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData170_HT_BOX_3X3_08U[] = {{26, 6, 4, 0, {1, 1, 2, 2}}, {26, 6, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData171_HT_BOX_3X3_08U[] = {{28, 6, 4, 0, {1, 1, 2, 2}}, {28, 6, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData172_HT_BOX_3X3_08U[] = {{30, 6, 4, 0, {1, 1, 2, 2}}, {30, 6, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData173_HT_BOX_3X3_08U[] = {{32, 6, 4, 0, {1, 1, 2, 2}}, {32, 6, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData174_HT_BOX_3X3_08U[] = {{36, 6, 4, 0, {1, 1, 2, 2}}, {36, 6, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData175_HT_BOX_3X3_08U[] = {{40, 6, 4, 0, {1, 1, 2, 2}}, {40, 6, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData176_HT_BOX_3X3_08U[] = {{44, 6, 4, 0, {1, 1, 2, 2}}, {44, 6, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData177_HT_BOX_3X3_08U[] = {{48, 6, 4, 0, {1, 1, 2, 2}}, {48, 6, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData178_HT_BOX_3X3_08U[] = {{52, 6, 4, 0, {1, 1, 2, 2}}, {52, 6, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData179_HT_BOX_3X3_08U[] = {{56, 6, 4, 0, {1, 1, 2, 2}}, {56, 6, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData180_HT_BOX_3X3_08U[] = {{60, 6, 4, 0, {1, 1, 2, 2}}, {60, 6, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData181_HT_BOX_3X3_08U[] = {{64, 6, 4, 0, {1, 1, 2, 2}}, {64, 6, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData182_HT_BOX_3X3_08U[] = {{72, 6, 4, 0, {1, 1, 2, 2}}, {72, 6, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData183_HT_BOX_3X3_08U[] = {{80, 6, 4, 0, {1, 1, 2, 2}}, {80, 6, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData184_HT_BOX_3X3_08U[] = {{88, 6, 4, 0, {1, 1, 2, 2}}, {88, 6, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData185_HT_BOX_3X3_08U[] = {{96, 6, 4, 0, {1, 1, 2, 2}}, {96, 6, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData186_HT_BOX_3X3_08U[] = {{2, 8, 4, 0, {1, 1, 2, 2}}, {2, 8, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData187_HT_BOX_3X3_08U[] = {{4, 8, 4, 0, {1, 1, 2, 2}}, {4, 8, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData188_HT_BOX_3X3_08U[] = {{6, 8, 4, 0, {1, 1, 2, 2}}, {6, 8, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData189_HT_BOX_3X3_08U[] = {{8, 8, 4, 0, {1, 1, 2, 2}}, {8, 8, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData190_HT_BOX_3X3_08U[] = {{10, 8, 4, 0, {1, 1, 2, 2}}, {10, 8, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData191_HT_BOX_3X3_08U[] = {{12, 8, 4, 0, {1, 1, 2, 2}}, {12, 8, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData192_HT_BOX_3X3_08U[] = {{14, 8, 4, 0, {1, 1, 2, 2}}, {14, 8, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData193_HT_BOX_3X3_08U[] = {{16, 8, 4, 0, {1, 1, 2, 2}}, {16, 8, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData194_HT_BOX_3X3_08U[] = {{18, 8, 4, 0, {1, 1, 2, 2}}, {18, 8, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData195_HT_BOX_3X3_08U[] = {{20, 8, 4, 0, {1, 1, 2, 2}}, {20, 8, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData196_HT_BOX_3X3_08U[] = {{22, 8, 4, 0, {1, 1, 2, 2}}, {22, 8, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData197_HT_BOX_3X3_08U[] = {{24, 8, 4, 0, {1, 1, 2, 2}}, {24, 8, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData198_HT_BOX_3X3_08U[] = {{26, 8, 4, 0, {1, 1, 2, 2}}, {26, 8, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData199_HT_BOX_3X3_08U[] = {{28, 8, 4, 0, {1, 1, 2, 2}}, {28, 8, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData200_HT_BOX_3X3_08U[] = {{30, 8, 4, 0, {1, 1, 2, 2}}, {30, 8, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData201_HT_BOX_3X3_08U[] = {{32, 8, 4, 0, {1, 1, 2, 2}}, {32, 8, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData202_HT_BOX_3X3_08U[] = {{36, 8, 4, 0, {1, 1, 2, 2}}, {36, 8, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData203_HT_BOX_3X3_08U[] = {{40, 8, 4, 0, {1, 1, 2, 2}}, {40, 8, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData204_HT_BOX_3X3_08U[] = {{44, 8, 4, 0, {1, 1, 2, 2}}, {44, 8, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData205_HT_BOX_3X3_08U[] = {{48, 8, 4, 0, {1, 1, 2, 2}}, {48, 8, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData206_HT_BOX_3X3_08U[] = {{52, 8, 4, 0, {1, 1, 2, 2}}, {52, 8, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData207_HT_BOX_3X3_08U[] = {{56, 8, 4, 0, {1, 1, 2, 2}}, {56, 8, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData208_HT_BOX_3X3_08U[] = {{60, 8, 4, 0, {1, 1, 2, 2}}, {60, 8, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData209_HT_BOX_3X3_08U[] = {{64, 8, 4, 0, {1, 1, 2, 2}}, {64, 8, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData210_HT_BOX_3X3_08U[] = {{72, 8, 4, 0, {1, 1, 2, 2}}, {72, 8, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData211_HT_BOX_3X3_08U[] = {{2, 10, 4, 0, {1, 1, 2, 2}}, {2, 10, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData212_HT_BOX_3X3_08U[] = {{4, 10, 4, 0, {1, 1, 2, 2}}, {4, 10, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData213_HT_BOX_3X3_08U[] = {{6, 10, 4, 0, {1, 1, 2, 2}}, {6, 10, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData214_HT_BOX_3X3_08U[] = {{8, 10, 4, 0, {1, 1, 2, 2}}, {8, 10, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData215_HT_BOX_3X3_08U[] = {{10, 10, 4, 0, {1, 1, 2, 2}}, {10, 10, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData216_HT_BOX_3X3_08U[] = {{12, 10, 4, 0, {1, 1, 2, 2}}, {12, 10, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData217_HT_BOX_3X3_08U[] = {{14, 10, 4, 0, {1, 1, 2, 2}}, {14, 10, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData218_HT_BOX_3X3_08U[] = {{16, 10, 4, 0, {1, 1, 2, 2}}, {16, 10, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData219_HT_BOX_3X3_08U[] = {{18, 10, 4, 0, {1, 1, 2, 2}}, {18, 10, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData220_HT_BOX_3X3_08U[] = {{20, 10, 4, 0, {1, 1, 2, 2}}, {20, 10, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData221_HT_BOX_3X3_08U[] = {{22, 10, 4, 0, {1, 1, 2, 2}}, {22, 10, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData222_HT_BOX_3X3_08U[] = {{24, 10, 4, 0, {1, 1, 2, 2}}, {24, 10, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData223_HT_BOX_3X3_08U[] = {{26, 10, 4, 0, {1, 1, 2, 2}}, {26, 10, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData224_HT_BOX_3X3_08U[] = {{28, 10, 4, 0, {1, 1, 2, 2}}, {28, 10, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData225_HT_BOX_3X3_08U[] = {{30, 10, 4, 0, {1, 1, 2, 2}}, {30, 10, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData226_HT_BOX_3X3_08U[] = {{32, 10, 4, 0, {1, 1, 2, 2}}, {32, 10, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData227_HT_BOX_3X3_08U[] = {{36, 10, 4, 0, {1, 1, 2, 2}}, {36, 10, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData228_HT_BOX_3X3_08U[] = {{40, 10, 4, 0, {1, 1, 2, 2}}, {40, 10, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData229_HT_BOX_3X3_08U[] = {{44, 10, 4, 0, {1, 1, 2, 2}}, {44, 10, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData230_HT_BOX_3X3_08U[] = {{48, 10, 4, 0, {1, 1, 2, 2}}, {48, 10, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData231_HT_BOX_3X3_08U[] = {{52, 10, 4, 0, {1, 1, 2, 2}}, {52, 10, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData232_HT_BOX_3X3_08U[] = {{56, 10, 4, 0, {1, 1, 2, 2}}, {56, 10, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData233_HT_BOX_3X3_08U[] = {{2, 12, 4, 0, {1, 1, 2, 2}}, {2, 12, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData234_HT_BOX_3X3_08U[] = {{4, 12, 4, 0, {1, 1, 2, 2}}, {4, 12, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData235_HT_BOX_3X3_08U[] = {{6, 12, 4, 0, {1, 1, 2, 2}}, {6, 12, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData236_HT_BOX_3X3_08U[] = {{8, 12, 4, 0, {1, 1, 2, 2}}, {8, 12, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData237_HT_BOX_3X3_08U[] = {{10, 12, 4, 0, {1, 1, 2, 2}}, {10, 12, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData238_HT_BOX_3X3_08U[] = {{12, 12, 4, 0, {1, 1, 2, 2}}, {12, 12, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData239_HT_BOX_3X3_08U[] = {{14, 12, 4, 0, {1, 1, 2, 2}}, {14, 12, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData240_HT_BOX_3X3_08U[] = {{16, 12, 4, 0, {1, 1, 2, 2}}, {16, 12, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData241_HT_BOX_3X3_08U[] = {{18, 12, 4, 0, {1, 1, 2, 2}}, {18, 12, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData242_HT_BOX_3X3_08U[] = {{20, 12, 4, 0, {1, 1, 2, 2}}, {20, 12, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData243_HT_BOX_3X3_08U[] = {{22, 12, 4, 0, {1, 1, 2, 2}}, {22, 12, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData244_HT_BOX_3X3_08U[] = {{24, 12, 4, 0, {1, 1, 2, 2}}, {24, 12, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData245_HT_BOX_3X3_08U[] = {{26, 12, 4, 0, {1, 1, 2, 2}}, {26, 12, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData246_HT_BOX_3X3_08U[] = {{28, 12, 4, 0, {1, 1, 2, 2}}, {28, 12, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData247_HT_BOX_3X3_08U[] = {{30, 12, 4, 0, {1, 1, 2, 2}}, {30, 12, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData248_HT_BOX_3X3_08U[] = {{32, 12, 4, 0, {1, 1, 2, 2}}, {32, 12, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData249_HT_BOX_3X3_08U[] = {{36, 12, 4, 0, {1, 1, 2, 2}}, {36, 12, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData250_HT_BOX_3X3_08U[] = {{40, 12, 4, 0, {1, 1, 2, 2}}, {40, 12, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData251_HT_BOX_3X3_08U[] = {{44, 12, 4, 0, {1, 1, 2, 2}}, {44, 12, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData252_HT_BOX_3X3_08U[] = {{48, 12, 4, 0, {1, 1, 2, 2}}, {48, 12, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData253_HT_BOX_3X3_08U[] = {{2, 14, 4, 0, {1, 1, 2, 2}}, {2, 14, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData254_HT_BOX_3X3_08U[] = {{4, 14, 4, 0, {1, 1, 2, 2}}, {4, 14, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData255_HT_BOX_3X3_08U[] = {{6, 14, 4, 0, {1, 1, 2, 2}}, {6, 14, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData256_HT_BOX_3X3_08U[] = {{8, 14, 4, 0, {1, 1, 2, 2}}, {8, 14, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData257_HT_BOX_3X3_08U[] = {{10, 14, 4, 0, {1, 1, 2, 2}}, {10, 14, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData258_HT_BOX_3X3_08U[] = {{12, 14, 4, 0, {1, 1, 2, 2}}, {12, 14, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData259_HT_BOX_3X3_08U[] = {{14, 14, 4, 0, {1, 1, 2, 2}}, {14, 14, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData260_HT_BOX_3X3_08U[] = {{16, 14, 4, 0, {1, 1, 2, 2}}, {16, 14, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData261_HT_BOX_3X3_08U[] = {{18, 14, 4, 0, {1, 1, 2, 2}}, {18, 14, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData262_HT_BOX_3X3_08U[] = {{20, 14, 4, 0, {1, 1, 2, 2}}, {20, 14, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData263_HT_BOX_3X3_08U[] = {{22, 14, 4, 0, {1, 1, 2, 2}}, {22, 14, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData264_HT_BOX_3X3_08U[] = {{24, 14, 4, 0, {1, 1, 2, 2}}, {24, 14, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData265_HT_BOX_3X3_08U[] = {{26, 14, 4, 0, {1, 1, 2, 2}}, {26, 14, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData266_HT_BOX_3X3_08U[] = {{28, 14, 4, 0, {1, 1, 2, 2}}, {28, 14, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData267_HT_BOX_3X3_08U[] = {{30, 14, 4, 0, {1, 1, 2, 2}}, {30, 14, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData268_HT_BOX_3X3_08U[] = {{32, 14, 4, 0, {1, 1, 2, 2}}, {32, 14, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData269_HT_BOX_3X3_08U[] = {{36, 14, 4, 0, {1, 1, 2, 2}}, {36, 14, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData270_HT_BOX_3X3_08U[] = {{40, 14, 4, 0, {1, 1, 2, 2}}, {40, 14, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData271_HT_BOX_3X3_08U[] = {{2, 16, 4, 0, {1, 1, 2, 2}}, {2, 16, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData272_HT_BOX_3X3_08U[] = {{4, 16, 4, 0, {1, 1, 2, 2}}, {4, 16, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData273_HT_BOX_3X3_08U[] = {{6, 16, 4, 0, {1, 1, 2, 2}}, {6, 16, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData274_HT_BOX_3X3_08U[] = {{8, 16, 4, 0, {1, 1, 2, 2}}, {8, 16, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData275_HT_BOX_3X3_08U[] = {{10, 16, 4, 0, {1, 1, 2, 2}}, {10, 16, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData276_HT_BOX_3X3_08U[] = {{12, 16, 4, 0, {1, 1, 2, 2}}, {12, 16, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData277_HT_BOX_3X3_08U[] = {{14, 16, 4, 0, {1, 1, 2, 2}}, {14, 16, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData278_HT_BOX_3X3_08U[] = {{16, 16, 4, 0, {1, 1, 2, 2}}, {16, 16, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData279_HT_BOX_3X3_08U[] = {{18, 16, 4, 0, {1, 1, 2, 2}}, {18, 16, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData280_HT_BOX_3X3_08U[] = {{20, 16, 4, 0, {1, 1, 2, 2}}, {20, 16, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData281_HT_BOX_3X3_08U[] = {{22, 16, 4, 0, {1, 1, 2, 2}}, {22, 16, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData282_HT_BOX_3X3_08U[] = {{24, 16, 4, 0, {1, 1, 2, 2}}, {24, 16, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData283_HT_BOX_3X3_08U[] = {{26, 16, 4, 0, {1, 1, 2, 2}}, {26, 16, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData284_HT_BOX_3X3_08U[] = {{28, 16, 4, 0, {1, 1, 2, 2}}, {28, 16, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData285_HT_BOX_3X3_08U[] = {{30, 16, 4, 0, {1, 1, 2, 2}}, {30, 16, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData286_HT_BOX_3X3_08U[] = {{32, 16, 4, 0, {1, 1, 2, 2}}, {32, 16, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData287_HT_BOX_3X3_08U[] = {{36, 16, 4, 0, {1, 1, 2, 2}}, {36, 16, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData288_HT_BOX_3X3_08U[] = {{2, 18, 4, 0, {1, 1, 2, 2}}, {2, 18, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData289_HT_BOX_3X3_08U[] = {{4, 18, 4, 0, {1, 1, 2, 2}}, {4, 18, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData290_HT_BOX_3X3_08U[] = {{6, 18, 4, 0, {1, 1, 2, 2}}, {6, 18, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData291_HT_BOX_3X3_08U[] = {{8, 18, 4, 0, {1, 1, 2, 2}}, {8, 18, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData292_HT_BOX_3X3_08U[] = {{10, 18, 4, 0, {1, 1, 2, 2}}, {10, 18, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData293_HT_BOX_3X3_08U[] = {{12, 18, 4, 0, {1, 1, 2, 2}}, {12, 18, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData294_HT_BOX_3X3_08U[] = {{14, 18, 4, 0, {1, 1, 2, 2}}, {14, 18, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData295_HT_BOX_3X3_08U[] = {{16, 18, 4, 0, {1, 1, 2, 2}}, {16, 18, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData296_HT_BOX_3X3_08U[] = {{18, 18, 4, 0, {1, 1, 2, 2}}, {18, 18, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData297_HT_BOX_3X3_08U[] = {{20, 18, 4, 0, {1, 1, 2, 2}}, {20, 18, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData298_HT_BOX_3X3_08U[] = {{22, 18, 4, 0, {1, 1, 2, 2}}, {22, 18, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData299_HT_BOX_3X3_08U[] = {{24, 18, 4, 0, {1, 1, 2, 2}}, {24, 18, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData300_HT_BOX_3X3_08U[] = {{26, 18, 4, 0, {1, 1, 2, 2}}, {26, 18, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData301_HT_BOX_3X3_08U[] = {{28, 18, 4, 0, {1, 1, 2, 2}}, {28, 18, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData302_HT_BOX_3X3_08U[] = {{30, 18, 4, 0, {1, 1, 2, 2}}, {30, 18, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData303_HT_BOX_3X3_08U[] = {{32, 18, 4, 0, {1, 1, 2, 2}}, {32, 18, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData304_HT_BOX_3X3_08U[] = {{2, 20, 4, 0, {1, 1, 2, 2}}, {2, 20, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData305_HT_BOX_3X3_08U[] = {{4, 20, 4, 0, {1, 1, 2, 2}}, {4, 20, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData306_HT_BOX_3X3_08U[] = {{6, 20, 4, 0, {1, 1, 2, 2}}, {6, 20, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData307_HT_BOX_3X3_08U[] = {{8, 20, 4, 0, {1, 1, 2, 2}}, {8, 20, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData308_HT_BOX_3X3_08U[] = {{10, 20, 4, 0, {1, 1, 2, 2}}, {10, 20, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData309_HT_BOX_3X3_08U[] = {{12, 20, 4, 0, {1, 1, 2, 2}}, {12, 20, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData310_HT_BOX_3X3_08U[] = {{14, 20, 4, 0, {1, 1, 2, 2}}, {14, 20, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData311_HT_BOX_3X3_08U[] = {{16, 20, 4, 0, {1, 1, 2, 2}}, {16, 20, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData312_HT_BOX_3X3_08U[] = {{18, 20, 4, 0, {1, 1, 2, 2}}, {18, 20, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData313_HT_BOX_3X3_08U[] = {{20, 20, 4, 0, {1, 1, 2, 2}}, {20, 20, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData314_HT_BOX_3X3_08U[] = {{22, 20, 4, 0, {1, 1, 2, 2}}, {22, 20, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData315_HT_BOX_3X3_08U[] = {{24, 20, 4, 0, {1, 1, 2, 2}}, {24, 20, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData316_HT_BOX_3X3_08U[] = {{26, 20, 4, 0, {1, 1, 2, 2}}, {26, 20, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData317_HT_BOX_3X3_08U[] = {{28, 20, 4, 0, {1, 1, 2, 2}}, {28, 20, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData318_HT_BOX_3X3_08U[] = {{2, 22, 4, 0, {1, 1, 2, 2}}, {2, 22, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData319_HT_BOX_3X3_08U[] = {{4, 22, 4, 0, {1, 1, 2, 2}}, {4, 22, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData320_HT_BOX_3X3_08U[] = {{6, 22, 4, 0, {1, 1, 2, 2}}, {6, 22, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData321_HT_BOX_3X3_08U[] = {{8, 22, 4, 0, {1, 1, 2, 2}}, {8, 22, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData322_HT_BOX_3X3_08U[] = {{10, 22, 4, 0, {1, 1, 2, 2}}, {10, 22, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData323_HT_BOX_3X3_08U[] = {{12, 22, 4, 0, {1, 1, 2, 2}}, {12, 22, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData324_HT_BOX_3X3_08U[] = {{14, 22, 4, 0, {1, 1, 2, 2}}, {14, 22, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData325_HT_BOX_3X3_08U[] = {{16, 22, 4, 0, {1, 1, 2, 2}}, {16, 22, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData326_HT_BOX_3X3_08U[] = {{18, 22, 4, 0, {1, 1, 2, 2}}, {18, 22, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData327_HT_BOX_3X3_08U[] = {{20, 22, 4, 0, {1, 1, 2, 2}}, {20, 22, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData328_HT_BOX_3X3_08U[] = {{22, 22, 4, 0, {1, 1, 2, 2}}, {22, 22, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData329_HT_BOX_3X3_08U[] = {{24, 22, 4, 0, {1, 1, 2, 2}}, {24, 22, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData330_HT_BOX_3X3_08U[] = {{2, 24, 4, 0, {1, 1, 2, 2}}, {2, 24, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData331_HT_BOX_3X3_08U[] = {{4, 24, 4, 0, {1, 1, 2, 2}}, {4, 24, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData332_HT_BOX_3X3_08U[] = {{6, 24, 4, 0, {1, 1, 2, 2}}, {6, 24, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData333_HT_BOX_3X3_08U[] = {{8, 24, 4, 0, {1, 1, 2, 2}}, {8, 24, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData334_HT_BOX_3X3_08U[] = {{10, 24, 4, 0, {1, 1, 2, 2}}, {10, 24, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData335_HT_BOX_3X3_08U[] = {{12, 24, 4, 0, {1, 1, 2, 2}}, {12, 24, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData336_HT_BOX_3X3_08U[] = {{14, 24, 4, 0, {1, 1, 2, 2}}, {14, 24, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData337_HT_BOX_3X3_08U[] = {{16, 24, 4, 0, {1, 1, 2, 2}}, {16, 24, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData338_HT_BOX_3X3_08U[] = {{18, 24, 4, 0, {1, 1, 2, 2}}, {18, 24, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData339_HT_BOX_3X3_08U[] = {{20, 24, 4, 0, {1, 1, 2, 2}}, {20, 24, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData340_HT_BOX_3X3_08U[] = {{22, 24, 4, 0, {1, 1, 2, 2}}, {22, 24, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData341_HT_BOX_3X3_08U[] = {{2, 26, 4, 0, {1, 1, 2, 2}}, {2, 26, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData342_HT_BOX_3X3_08U[] = {{4, 26, 4, 0, {1, 1, 2, 2}}, {4, 26, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData343_HT_BOX_3X3_08U[] = {{6, 26, 4, 0, {1, 1, 2, 2}}, {6, 26, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData344_HT_BOX_3X3_08U[] = {{8, 26, 4, 0, {1, 1, 2, 2}}, {8, 26, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData345_HT_BOX_3X3_08U[] = {{10, 26, 4, 0, {1, 1, 2, 2}}, {10, 26, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData346_HT_BOX_3X3_08U[] = {{12, 26, 4, 0, {1, 1, 2, 2}}, {12, 26, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData347_HT_BOX_3X3_08U[] = {{14, 26, 4, 0, {1, 1, 2, 2}}, {14, 26, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData348_HT_BOX_3X3_08U[] = {{16, 26, 4, 0, {1, 1, 2, 2}}, {16, 26, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData349_HT_BOX_3X3_08U[] = {{18, 26, 4, 0, {1, 1, 2, 2}}, {18, 26, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData350_HT_BOX_3X3_08U[] = {{20, 26, 4, 0, {1, 1, 2, 2}}, {20, 26, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData351_HT_BOX_3X3_08U[] = {{2, 28, 4, 0, {1, 1, 2, 2}}, {2, 28, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData352_HT_BOX_3X3_08U[] = {{4, 28, 4, 0, {1, 1, 2, 2}}, {4, 28, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData353_HT_BOX_3X3_08U[] = {{6, 28, 4, 0, {1, 1, 2, 2}}, {6, 28, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData354_HT_BOX_3X3_08U[] = {{8, 28, 4, 0, {1, 1, 2, 2}}, {8, 28, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData355_HT_BOX_3X3_08U[] = {{10, 28, 4, 0, {1, 1, 2, 2}}, {10, 28, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData356_HT_BOX_3X3_08U[] = {{12, 28, 4, 0, {1, 1, 2, 2}}, {12, 28, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData357_HT_BOX_3X3_08U[] = {{14, 28, 4, 0, {1, 1, 2, 2}}, {14, 28, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData358_HT_BOX_3X3_08U[] = {{16, 28, 4, 0, {1, 1, 2, 2}}, {16, 28, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData359_HT_BOX_3X3_08U[] = {{18, 28, 4, 0, {1, 1, 2, 2}}, {18, 28, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData360_HT_BOX_3X3_08U[] = {{2, 30, 4, 0, {1, 1, 2, 2}}, {2, 30, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData361_HT_BOX_3X3_08U[] = {{4, 30, 4, 0, {1, 1, 2, 2}}, {4, 30, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData362_HT_BOX_3X3_08U[] = {{6, 30, 4, 0, {1, 1, 2, 2}}, {6, 30, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData363_HT_BOX_3X3_08U[] = {{8, 30, 4, 0, {1, 1, 2, 2}}, {8, 30, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData364_HT_BOX_3X3_08U[] = {{10, 30, 4, 0, {1, 1, 2, 2}}, {10, 30, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData365_HT_BOX_3X3_08U[] = {{12, 30, 4, 0, {1, 1, 2, 2}}, {12, 30, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData366_HT_BOX_3X3_08U[] = {{14, 30, 4, 0, {1, 1, 2, 2}}, {14, 30, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData367_HT_BOX_3X3_08U[] = {{16, 30, 4, 0, {1, 1, 2, 2}}, {16, 30, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData368_HT_BOX_3X3_08U[] = {{18, 30, 4, 0, {1, 1, 2, 2}}, {18, 30, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData369_HT_BOX_3X3_08U[] = {{2, 32, 4, 0, {1, 1, 2, 2}}, {2, 32, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData370_HT_BOX_3X3_08U[] = {{4, 32, 4, 0, {1, 1, 2, 2}}, {4, 32, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData371_HT_BOX_3X3_08U[] = {{6, 32, 4, 0, {1, 1, 2, 2}}, {6, 32, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData372_HT_BOX_3X3_08U[] = {{8, 32, 4, 0, {1, 1, 2, 2}}, {8, 32, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData373_HT_BOX_3X3_08U[] = {{10, 32, 4, 0, {1, 1, 2, 2}}, {10, 32, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData374_HT_BOX_3X3_08U[] = {{12, 32, 4, 0, {1, 1, 2, 2}}, {12, 32, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData375_HT_BOX_3X3_08U[] = {{14, 32, 4, 0, {1, 1, 2, 2}}, {14, 32, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData376_HT_BOX_3X3_08U[] = {{16, 32, 4, 0, {1, 1, 2, 2}}, {16, 32, 2, 1, {0, 0, 0, 0}}};

static acf_scenario_kernel_data gScenarioKernelData0_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData1_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData2_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData3_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData4_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData5_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData6_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData7_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData8_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData9_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData10_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData11_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData12_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData13_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData14_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData15_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData16_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData17_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData18_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData19_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData20_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData21_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData22_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData23_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData24_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData25_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData26_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData27_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData28_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData29_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData30_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData31_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData32_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData33_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData34_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData35_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData36_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData37_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData38_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData39_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData40_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData41_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData42_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData43_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData44_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData45_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData46_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData47_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData48_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData49_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData50_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData51_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData52_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData53_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData54_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData55_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData56_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData57_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData58_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData59_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData60_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData61_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData62_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData63_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData64_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData65_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData66_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData67_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData68_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData69_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData70_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData71_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData72_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData73_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData74_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData75_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData76_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData77_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData78_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData79_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData80_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData81_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData82_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData83_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData84_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData85_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData86_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData87_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData88_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData89_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData90_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData91_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData92_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData93_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData94_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData95_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData96_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData97_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData98_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData99_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData100_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData101_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData102_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData103_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData104_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData105_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData106_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData107_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData108_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData109_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData110_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData111_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData112_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData113_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData114_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData115_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData116_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData117_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData118_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData119_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData120_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData121_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData122_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData123_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData124_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData125_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData126_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData127_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData128_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData129_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData130_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData131_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData132_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData133_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData134_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData135_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData136_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData137_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData138_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData139_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData140_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData141_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData142_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData143_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData144_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData145_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData146_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData147_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData148_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData149_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData150_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData151_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData152_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData153_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData154_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData155_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData156_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData157_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData158_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData159_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData160_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData161_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData162_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData163_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData164_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData165_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData166_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData167_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData168_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData169_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData170_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData171_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData172_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData173_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData174_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData175_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData176_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData177_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData178_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData179_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData180_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData181_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData182_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData183_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData184_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData185_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData186_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData187_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData188_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData189_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData190_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData191_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData192_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData193_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData194_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData195_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData196_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData197_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData198_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData199_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData200_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData201_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData202_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData203_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData204_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData205_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData206_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData207_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData208_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData209_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData210_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData211_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData212_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData213_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData214_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData215_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData216_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData217_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData218_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData219_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData220_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData221_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData222_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData223_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData224_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData225_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData226_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData227_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData228_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData229_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData230_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData231_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData232_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData233_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData234_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData235_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData236_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData237_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData238_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData239_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData240_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData241_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData242_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData243_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData244_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData245_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData246_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData247_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData248_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData249_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData250_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData251_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData252_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData253_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData254_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData255_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData256_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData257_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData258_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData259_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData260_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData261_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData262_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData263_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData264_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData265_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData266_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData267_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData268_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData269_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData270_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData271_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData272_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData273_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData274_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData275_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData276_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData277_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData278_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData279_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData280_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData281_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData282_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData283_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData284_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData285_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData286_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData287_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData288_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData289_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData290_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData291_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData292_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData293_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData294_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData295_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData296_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData297_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData298_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData299_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData300_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData301_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData302_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData303_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData304_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData305_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData306_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData307_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData308_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData309_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData310_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData311_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData312_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData313_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData314_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData315_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData316_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData317_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData318_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData319_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData320_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData321_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData322_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData323_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData324_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData325_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData326_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData327_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData328_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData329_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData330_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData331_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData332_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData333_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData334_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData335_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData336_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData337_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData338_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData339_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData340_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData341_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData342_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData343_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData344_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData345_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData346_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData347_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData348_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData349_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData350_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData351_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData352_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData353_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData354_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData355_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData356_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData357_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData358_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData359_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData360_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData361_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData362_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData363_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData364_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData365_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData366_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData367_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData368_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData369_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData370_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData371_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData372_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData373_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData374_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData375_HT_BOX_3X3_08U[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData376_HT_BOX_3X3_08U[] = {{1, 0}};

static acf_scenario gScenarioArray_HT_BOX_3X3_08U[] = {
{1, 1, 40, 0, 1, gScenarioBufferData0_HT_BOX_3X3_08U, 32, gScenarioKernelData0_HT_BOX_3X3_08U, 4},
{2, 1, 56, 0, 1, gScenarioBufferData1_HT_BOX_3X3_08U, 32, gScenarioKernelData1_HT_BOX_3X3_08U, 4},
{3, 1, 72, 0, 1, gScenarioBufferData2_HT_BOX_3X3_08U, 32, gScenarioKernelData2_HT_BOX_3X3_08U, 4},
{4, 1, 88, 0, 1, gScenarioBufferData3_HT_BOX_3X3_08U, 32, gScenarioKernelData3_HT_BOX_3X3_08U, 4},
{5, 1, 104, 0, 1, gScenarioBufferData4_HT_BOX_3X3_08U, 32, gScenarioKernelData4_HT_BOX_3X3_08U, 4},
{6, 1, 120, 0, 1, gScenarioBufferData5_HT_BOX_3X3_08U, 32, gScenarioKernelData5_HT_BOX_3X3_08U, 4},
{7, 1, 136, 0, 1, gScenarioBufferData6_HT_BOX_3X3_08U, 32, gScenarioKernelData6_HT_BOX_3X3_08U, 4},
{8, 1, 152, 0, 1, gScenarioBufferData7_HT_BOX_3X3_08U, 32, gScenarioKernelData7_HT_BOX_3X3_08U, 4},
{9, 1, 168, 0, 1, gScenarioBufferData8_HT_BOX_3X3_08U, 32, gScenarioKernelData8_HT_BOX_3X3_08U, 4},
{10, 1, 184, 0, 1, gScenarioBufferData9_HT_BOX_3X3_08U, 32, gScenarioKernelData9_HT_BOX_3X3_08U, 4},
{11, 1, 200, 0, 1, gScenarioBufferData10_HT_BOX_3X3_08U, 32, gScenarioKernelData10_HT_BOX_3X3_08U, 4},
{12, 1, 216, 0, 1, gScenarioBufferData11_HT_BOX_3X3_08U, 32, gScenarioKernelData11_HT_BOX_3X3_08U, 4},
{13, 1, 232, 0, 1, gScenarioBufferData12_HT_BOX_3X3_08U, 32, gScenarioKernelData12_HT_BOX_3X3_08U, 4},
{14, 1, 248, 0, 1, gScenarioBufferData13_HT_BOX_3X3_08U, 32, gScenarioKernelData13_HT_BOX_3X3_08U, 4},
{15, 1, 264, 0, 1, gScenarioBufferData14_HT_BOX_3X3_08U, 32, gScenarioKernelData14_HT_BOX_3X3_08U, 4},
{16, 1, 280, 0, 1, gScenarioBufferData15_HT_BOX_3X3_08U, 32, gScenarioKernelData15_HT_BOX_3X3_08U, 4},
{18, 1, 312, 0, 1, gScenarioBufferData16_HT_BOX_3X3_08U, 32, gScenarioKernelData16_HT_BOX_3X3_08U, 4},
{20, 1, 344, 0, 1, gScenarioBufferData17_HT_BOX_3X3_08U, 32, gScenarioKernelData17_HT_BOX_3X3_08U, 4},
{22, 1, 376, 0, 1, gScenarioBufferData18_HT_BOX_3X3_08U, 32, gScenarioKernelData18_HT_BOX_3X3_08U, 4},
{24, 1, 408, 0, 1, gScenarioBufferData19_HT_BOX_3X3_08U, 32, gScenarioKernelData19_HT_BOX_3X3_08U, 4},
{26, 1, 440, 0, 1, gScenarioBufferData20_HT_BOX_3X3_08U, 32, gScenarioKernelData20_HT_BOX_3X3_08U, 4},
{28, 1, 472, 0, 1, gScenarioBufferData21_HT_BOX_3X3_08U, 32, gScenarioKernelData21_HT_BOX_3X3_08U, 4},
{30, 1, 504, 0, 1, gScenarioBufferData22_HT_BOX_3X3_08U, 32, gScenarioKernelData22_HT_BOX_3X3_08U, 4},
{32, 1, 536, 0, 1, gScenarioBufferData23_HT_BOX_3X3_08U, 32, gScenarioKernelData23_HT_BOX_3X3_08U, 4},
{36, 1, 600, 0, 1, gScenarioBufferData24_HT_BOX_3X3_08U, 32, gScenarioKernelData24_HT_BOX_3X3_08U, 4},
{40, 1, 664, 0, 1, gScenarioBufferData25_HT_BOX_3X3_08U, 32, gScenarioKernelData25_HT_BOX_3X3_08U, 4},
{44, 1, 728, 0, 1, gScenarioBufferData26_HT_BOX_3X3_08U, 32, gScenarioKernelData26_HT_BOX_3X3_08U, 4},
{48, 1, 792, 0, 1, gScenarioBufferData27_HT_BOX_3X3_08U, 32, gScenarioKernelData27_HT_BOX_3X3_08U, 4},
{52, 1, 856, 0, 1, gScenarioBufferData28_HT_BOX_3X3_08U, 32, gScenarioKernelData28_HT_BOX_3X3_08U, 4},
{56, 1, 920, 0, 1, gScenarioBufferData29_HT_BOX_3X3_08U, 32, gScenarioKernelData29_HT_BOX_3X3_08U, 4},
{60, 1, 984, 0, 1, gScenarioBufferData30_HT_BOX_3X3_08U, 32, gScenarioKernelData30_HT_BOX_3X3_08U, 4},
{64, 1, 1048, 0, 1, gScenarioBufferData31_HT_BOX_3X3_08U, 32, gScenarioKernelData31_HT_BOX_3X3_08U, 4},
{1, 2, 68, 0, 1, gScenarioBufferData32_HT_BOX_3X3_08U, 32, gScenarioKernelData32_HT_BOX_3X3_08U, 4},
{2, 2, 96, 0, 1, gScenarioBufferData33_HT_BOX_3X3_08U, 32, gScenarioKernelData33_HT_BOX_3X3_08U, 4},
{3, 2, 124, 0, 1, gScenarioBufferData34_HT_BOX_3X3_08U, 32, gScenarioKernelData34_HT_BOX_3X3_08U, 4},
{4, 2, 152, 0, 1, gScenarioBufferData35_HT_BOX_3X3_08U, 32, gScenarioKernelData35_HT_BOX_3X3_08U, 4},
{5, 2, 180, 0, 1, gScenarioBufferData36_HT_BOX_3X3_08U, 32, gScenarioKernelData36_HT_BOX_3X3_08U, 4},
{6, 2, 208, 0, 1, gScenarioBufferData37_HT_BOX_3X3_08U, 32, gScenarioKernelData37_HT_BOX_3X3_08U, 4},
{7, 2, 236, 0, 1, gScenarioBufferData38_HT_BOX_3X3_08U, 32, gScenarioKernelData38_HT_BOX_3X3_08U, 4},
{8, 2, 264, 0, 1, gScenarioBufferData39_HT_BOX_3X3_08U, 32, gScenarioKernelData39_HT_BOX_3X3_08U, 4},
{9, 2, 292, 0, 1, gScenarioBufferData40_HT_BOX_3X3_08U, 32, gScenarioKernelData40_HT_BOX_3X3_08U, 4},
{10, 2, 320, 0, 1, gScenarioBufferData41_HT_BOX_3X3_08U, 32, gScenarioKernelData41_HT_BOX_3X3_08U, 4},
{11, 2, 348, 0, 1, gScenarioBufferData42_HT_BOX_3X3_08U, 32, gScenarioKernelData42_HT_BOX_3X3_08U, 4},
{12, 2, 376, 0, 1, gScenarioBufferData43_HT_BOX_3X3_08U, 32, gScenarioKernelData43_HT_BOX_3X3_08U, 4},
{13, 2, 404, 0, 1, gScenarioBufferData44_HT_BOX_3X3_08U, 32, gScenarioKernelData44_HT_BOX_3X3_08U, 4},
{14, 2, 432, 0, 1, gScenarioBufferData45_HT_BOX_3X3_08U, 32, gScenarioKernelData45_HT_BOX_3X3_08U, 4},
{15, 2, 460, 0, 1, gScenarioBufferData46_HT_BOX_3X3_08U, 32, gScenarioKernelData46_HT_BOX_3X3_08U, 4},
{16, 2, 488, 0, 1, gScenarioBufferData47_HT_BOX_3X3_08U, 32, gScenarioKernelData47_HT_BOX_3X3_08U, 4},
{18, 2, 544, 0, 1, gScenarioBufferData48_HT_BOX_3X3_08U, 32, gScenarioKernelData48_HT_BOX_3X3_08U, 4},
{20, 2, 600, 0, 1, gScenarioBufferData49_HT_BOX_3X3_08U, 32, gScenarioKernelData49_HT_BOX_3X3_08U, 4},
{22, 2, 656, 0, 1, gScenarioBufferData50_HT_BOX_3X3_08U, 32, gScenarioKernelData50_HT_BOX_3X3_08U, 4},
{24, 2, 712, 0, 1, gScenarioBufferData51_HT_BOX_3X3_08U, 32, gScenarioKernelData51_HT_BOX_3X3_08U, 4},
{26, 2, 768, 0, 1, gScenarioBufferData52_HT_BOX_3X3_08U, 32, gScenarioKernelData52_HT_BOX_3X3_08U, 4},
{28, 2, 824, 0, 1, gScenarioBufferData53_HT_BOX_3X3_08U, 32, gScenarioKernelData53_HT_BOX_3X3_08U, 4},
{30, 2, 880, 0, 1, gScenarioBufferData54_HT_BOX_3X3_08U, 32, gScenarioKernelData54_HT_BOX_3X3_08U, 4},
{32, 2, 936, 0, 1, gScenarioBufferData55_HT_BOX_3X3_08U, 32, gScenarioKernelData55_HT_BOX_3X3_08U, 4},
{36, 2, 1048, 0, 1, gScenarioBufferData56_HT_BOX_3X3_08U, 32, gScenarioKernelData56_HT_BOX_3X3_08U, 4},
{40, 2, 1160, 0, 1, gScenarioBufferData57_HT_BOX_3X3_08U, 32, gScenarioKernelData57_HT_BOX_3X3_08U, 4},
{44, 2, 1272, 0, 1, gScenarioBufferData58_HT_BOX_3X3_08U, 32, gScenarioKernelData58_HT_BOX_3X3_08U, 4},
{48, 2, 1384, 0, 1, gScenarioBufferData59_HT_BOX_3X3_08U, 32, gScenarioKernelData59_HT_BOX_3X3_08U, 4},
{52, 2, 1496, 0, 1, gScenarioBufferData60_HT_BOX_3X3_08U, 32, gScenarioKernelData60_HT_BOX_3X3_08U, 4},
{56, 2, 1608, 0, 1, gScenarioBufferData61_HT_BOX_3X3_08U, 32, gScenarioKernelData61_HT_BOX_3X3_08U, 4},
{60, 2, 1720, 0, 1, gScenarioBufferData62_HT_BOX_3X3_08U, 32, gScenarioKernelData62_HT_BOX_3X3_08U, 4},
{64, 2, 1832, 0, 1, gScenarioBufferData63_HT_BOX_3X3_08U, 32, gScenarioKernelData63_HT_BOX_3X3_08U, 4},
{1, 3, 96, 0, 1, gScenarioBufferData64_HT_BOX_3X3_08U, 32, gScenarioKernelData64_HT_BOX_3X3_08U, 4},
{2, 3, 136, 0, 1, gScenarioBufferData65_HT_BOX_3X3_08U, 32, gScenarioKernelData65_HT_BOX_3X3_08U, 4},
{3, 3, 176, 0, 1, gScenarioBufferData66_HT_BOX_3X3_08U, 32, gScenarioKernelData66_HT_BOX_3X3_08U, 4},
{4, 3, 216, 0, 1, gScenarioBufferData67_HT_BOX_3X3_08U, 32, gScenarioKernelData67_HT_BOX_3X3_08U, 4},
{5, 3, 256, 0, 1, gScenarioBufferData68_HT_BOX_3X3_08U, 32, gScenarioKernelData68_HT_BOX_3X3_08U, 4},
{6, 3, 296, 0, 1, gScenarioBufferData69_HT_BOX_3X3_08U, 32, gScenarioKernelData69_HT_BOX_3X3_08U, 4},
{7, 3, 336, 0, 1, gScenarioBufferData70_HT_BOX_3X3_08U, 32, gScenarioKernelData70_HT_BOX_3X3_08U, 4},
{8, 3, 376, 0, 1, gScenarioBufferData71_HT_BOX_3X3_08U, 32, gScenarioKernelData71_HT_BOX_3X3_08U, 4},
{9, 3, 416, 0, 1, gScenarioBufferData72_HT_BOX_3X3_08U, 32, gScenarioKernelData72_HT_BOX_3X3_08U, 4},
{10, 3, 456, 0, 1, gScenarioBufferData73_HT_BOX_3X3_08U, 32, gScenarioKernelData73_HT_BOX_3X3_08U, 4},
{11, 3, 496, 0, 1, gScenarioBufferData74_HT_BOX_3X3_08U, 32, gScenarioKernelData74_HT_BOX_3X3_08U, 4},
{12, 3, 536, 0, 1, gScenarioBufferData75_HT_BOX_3X3_08U, 32, gScenarioKernelData75_HT_BOX_3X3_08U, 4},
{13, 3, 576, 0, 1, gScenarioBufferData76_HT_BOX_3X3_08U, 32, gScenarioKernelData76_HT_BOX_3X3_08U, 4},
{14, 3, 616, 0, 1, gScenarioBufferData77_HT_BOX_3X3_08U, 32, gScenarioKernelData77_HT_BOX_3X3_08U, 4},
{15, 3, 656, 0, 1, gScenarioBufferData78_HT_BOX_3X3_08U, 32, gScenarioKernelData78_HT_BOX_3X3_08U, 4},
{16, 3, 696, 0, 1, gScenarioBufferData79_HT_BOX_3X3_08U, 32, gScenarioKernelData79_HT_BOX_3X3_08U, 4},
{18, 3, 776, 0, 1, gScenarioBufferData80_HT_BOX_3X3_08U, 32, gScenarioKernelData80_HT_BOX_3X3_08U, 4},
{20, 3, 856, 0, 1, gScenarioBufferData81_HT_BOX_3X3_08U, 32, gScenarioKernelData81_HT_BOX_3X3_08U, 4},
{22, 3, 936, 0, 1, gScenarioBufferData82_HT_BOX_3X3_08U, 32, gScenarioKernelData82_HT_BOX_3X3_08U, 4},
{24, 3, 1016, 0, 1, gScenarioBufferData83_HT_BOX_3X3_08U, 32, gScenarioKernelData83_HT_BOX_3X3_08U, 4},
{26, 3, 1096, 0, 1, gScenarioBufferData84_HT_BOX_3X3_08U, 32, gScenarioKernelData84_HT_BOX_3X3_08U, 4},
{28, 3, 1176, 0, 1, gScenarioBufferData85_HT_BOX_3X3_08U, 32, gScenarioKernelData85_HT_BOX_3X3_08U, 4},
{30, 3, 1256, 0, 1, gScenarioBufferData86_HT_BOX_3X3_08U, 32, gScenarioKernelData86_HT_BOX_3X3_08U, 4},
{32, 3, 1336, 0, 1, gScenarioBufferData87_HT_BOX_3X3_08U, 32, gScenarioKernelData87_HT_BOX_3X3_08U, 4},
{36, 3, 1496, 0, 1, gScenarioBufferData88_HT_BOX_3X3_08U, 32, gScenarioKernelData88_HT_BOX_3X3_08U, 4},
{40, 3, 1656, 0, 1, gScenarioBufferData89_HT_BOX_3X3_08U, 32, gScenarioKernelData89_HT_BOX_3X3_08U, 4},
{44, 3, 1816, 0, 1, gScenarioBufferData90_HT_BOX_3X3_08U, 32, gScenarioKernelData90_HT_BOX_3X3_08U, 4},
{48, 3, 1976, 0, 1, gScenarioBufferData91_HT_BOX_3X3_08U, 32, gScenarioKernelData91_HT_BOX_3X3_08U, 4},
{52, 3, 2136, 0, 1, gScenarioBufferData92_HT_BOX_3X3_08U, 32, gScenarioKernelData92_HT_BOX_3X3_08U, 4},
{56, 3, 2296, 0, 1, gScenarioBufferData93_HT_BOX_3X3_08U, 32, gScenarioKernelData93_HT_BOX_3X3_08U, 4},
{60, 3, 2456, 0, 1, gScenarioBufferData94_HT_BOX_3X3_08U, 32, gScenarioKernelData94_HT_BOX_3X3_08U, 4},
{64, 3, 2616, 0, 1, gScenarioBufferData95_HT_BOX_3X3_08U, 32, gScenarioKernelData95_HT_BOX_3X3_08U, 4},
{1, 4, 124, 0, 1, gScenarioBufferData96_HT_BOX_3X3_08U, 32, gScenarioKernelData96_HT_BOX_3X3_08U, 4},
{2, 4, 176, 0, 1, gScenarioBufferData97_HT_BOX_3X3_08U, 32, gScenarioKernelData97_HT_BOX_3X3_08U, 4},
{3, 4, 228, 0, 1, gScenarioBufferData98_HT_BOX_3X3_08U, 32, gScenarioKernelData98_HT_BOX_3X3_08U, 4},
{4, 4, 280, 0, 1, gScenarioBufferData99_HT_BOX_3X3_08U, 32, gScenarioKernelData99_HT_BOX_3X3_08U, 4},
{5, 4, 332, 0, 1, gScenarioBufferData100_HT_BOX_3X3_08U, 32, gScenarioKernelData100_HT_BOX_3X3_08U, 4},
{6, 4, 384, 0, 1, gScenarioBufferData101_HT_BOX_3X3_08U, 32, gScenarioKernelData101_HT_BOX_3X3_08U, 4},
{7, 4, 436, 0, 1, gScenarioBufferData102_HT_BOX_3X3_08U, 32, gScenarioKernelData102_HT_BOX_3X3_08U, 4},
{8, 4, 488, 0, 1, gScenarioBufferData103_HT_BOX_3X3_08U, 32, gScenarioKernelData103_HT_BOX_3X3_08U, 4},
{9, 4, 540, 0, 1, gScenarioBufferData104_HT_BOX_3X3_08U, 32, gScenarioKernelData104_HT_BOX_3X3_08U, 4},
{10, 4, 592, 0, 1, gScenarioBufferData105_HT_BOX_3X3_08U, 32, gScenarioKernelData105_HT_BOX_3X3_08U, 4},
{11, 4, 644, 0, 1, gScenarioBufferData106_HT_BOX_3X3_08U, 32, gScenarioKernelData106_HT_BOX_3X3_08U, 4},
{12, 4, 696, 0, 1, gScenarioBufferData107_HT_BOX_3X3_08U, 32, gScenarioKernelData107_HT_BOX_3X3_08U, 4},
{13, 4, 748, 0, 1, gScenarioBufferData108_HT_BOX_3X3_08U, 32, gScenarioKernelData108_HT_BOX_3X3_08U, 4},
{14, 4, 800, 0, 1, gScenarioBufferData109_HT_BOX_3X3_08U, 32, gScenarioKernelData109_HT_BOX_3X3_08U, 4},
{15, 4, 852, 0, 1, gScenarioBufferData110_HT_BOX_3X3_08U, 32, gScenarioKernelData110_HT_BOX_3X3_08U, 4},
{16, 4, 904, 0, 1, gScenarioBufferData111_HT_BOX_3X3_08U, 32, gScenarioKernelData111_HT_BOX_3X3_08U, 4},
{18, 4, 1008, 0, 1, gScenarioBufferData112_HT_BOX_3X3_08U, 32, gScenarioKernelData112_HT_BOX_3X3_08U, 4},
{20, 4, 1112, 0, 1, gScenarioBufferData113_HT_BOX_3X3_08U, 32, gScenarioKernelData113_HT_BOX_3X3_08U, 4},
{22, 4, 1216, 0, 1, gScenarioBufferData114_HT_BOX_3X3_08U, 32, gScenarioKernelData114_HT_BOX_3X3_08U, 4},
{24, 4, 1320, 0, 1, gScenarioBufferData115_HT_BOX_3X3_08U, 32, gScenarioKernelData115_HT_BOX_3X3_08U, 4},
{26, 4, 1424, 0, 1, gScenarioBufferData116_HT_BOX_3X3_08U, 32, gScenarioKernelData116_HT_BOX_3X3_08U, 4},
{28, 4, 1528, 0, 1, gScenarioBufferData117_HT_BOX_3X3_08U, 32, gScenarioKernelData117_HT_BOX_3X3_08U, 4},
{30, 4, 1632, 0, 1, gScenarioBufferData118_HT_BOX_3X3_08U, 32, gScenarioKernelData118_HT_BOX_3X3_08U, 4},
{32, 4, 1736, 0, 1, gScenarioBufferData119_HT_BOX_3X3_08U, 32, gScenarioKernelData119_HT_BOX_3X3_08U, 4},
{36, 4, 1944, 0, 1, gScenarioBufferData120_HT_BOX_3X3_08U, 32, gScenarioKernelData120_HT_BOX_3X3_08U, 4},
{40, 4, 2152, 0, 1, gScenarioBufferData121_HT_BOX_3X3_08U, 32, gScenarioKernelData121_HT_BOX_3X3_08U, 4},
{44, 4, 2360, 0, 1, gScenarioBufferData122_HT_BOX_3X3_08U, 32, gScenarioKernelData122_HT_BOX_3X3_08U, 4},
{48, 4, 2568, 0, 1, gScenarioBufferData123_HT_BOX_3X3_08U, 32, gScenarioKernelData123_HT_BOX_3X3_08U, 4},
{52, 4, 2776, 0, 1, gScenarioBufferData124_HT_BOX_3X3_08U, 32, gScenarioKernelData124_HT_BOX_3X3_08U, 4},
{56, 4, 2984, 0, 1, gScenarioBufferData125_HT_BOX_3X3_08U, 32, gScenarioKernelData125_HT_BOX_3X3_08U, 4},
{60, 4, 3192, 0, 1, gScenarioBufferData126_HT_BOX_3X3_08U, 32, gScenarioKernelData126_HT_BOX_3X3_08U, 4},
{64, 4, 3400, 0, 1, gScenarioBufferData127_HT_BOX_3X3_08U, 32, gScenarioKernelData127_HT_BOX_3X3_08U, 4},
{1, 5, 152, 0, 1, gScenarioBufferData128_HT_BOX_3X3_08U, 32, gScenarioKernelData128_HT_BOX_3X3_08U, 4},
{2, 5, 216, 0, 1, gScenarioBufferData129_HT_BOX_3X3_08U, 32, gScenarioKernelData129_HT_BOX_3X3_08U, 4},
{3, 5, 280, 0, 1, gScenarioBufferData130_HT_BOX_3X3_08U, 32, gScenarioKernelData130_HT_BOX_3X3_08U, 4},
{4, 5, 344, 0, 1, gScenarioBufferData131_HT_BOX_3X3_08U, 32, gScenarioKernelData131_HT_BOX_3X3_08U, 4},
{5, 5, 408, 0, 1, gScenarioBufferData132_HT_BOX_3X3_08U, 32, gScenarioKernelData132_HT_BOX_3X3_08U, 4},
{6, 5, 472, 0, 1, gScenarioBufferData133_HT_BOX_3X3_08U, 32, gScenarioKernelData133_HT_BOX_3X3_08U, 4},
{7, 5, 536, 0, 1, gScenarioBufferData134_HT_BOX_3X3_08U, 32, gScenarioKernelData134_HT_BOX_3X3_08U, 4},
{8, 5, 600, 0, 1, gScenarioBufferData135_HT_BOX_3X3_08U, 32, gScenarioKernelData135_HT_BOX_3X3_08U, 4},
{9, 5, 664, 0, 1, gScenarioBufferData136_HT_BOX_3X3_08U, 32, gScenarioKernelData136_HT_BOX_3X3_08U, 4},
{10, 5, 728, 0, 1, gScenarioBufferData137_HT_BOX_3X3_08U, 32, gScenarioKernelData137_HT_BOX_3X3_08U, 4},
{11, 5, 792, 0, 1, gScenarioBufferData138_HT_BOX_3X3_08U, 32, gScenarioKernelData138_HT_BOX_3X3_08U, 4},
{12, 5, 856, 0, 1, gScenarioBufferData139_HT_BOX_3X3_08U, 32, gScenarioKernelData139_HT_BOX_3X3_08U, 4},
{13, 5, 920, 0, 1, gScenarioBufferData140_HT_BOX_3X3_08U, 32, gScenarioKernelData140_HT_BOX_3X3_08U, 4},
{14, 5, 984, 0, 1, gScenarioBufferData141_HT_BOX_3X3_08U, 32, gScenarioKernelData141_HT_BOX_3X3_08U, 4},
{15, 5, 1048, 0, 1, gScenarioBufferData142_HT_BOX_3X3_08U, 32, gScenarioKernelData142_HT_BOX_3X3_08U, 4},
{16, 5, 1112, 0, 1, gScenarioBufferData143_HT_BOX_3X3_08U, 32, gScenarioKernelData143_HT_BOX_3X3_08U, 4},
{18, 5, 1240, 0, 1, gScenarioBufferData144_HT_BOX_3X3_08U, 32, gScenarioKernelData144_HT_BOX_3X3_08U, 4},
{20, 5, 1368, 0, 1, gScenarioBufferData145_HT_BOX_3X3_08U, 32, gScenarioKernelData145_HT_BOX_3X3_08U, 4},
{22, 5, 1496, 0, 1, gScenarioBufferData146_HT_BOX_3X3_08U, 32, gScenarioKernelData146_HT_BOX_3X3_08U, 4},
{24, 5, 1624, 0, 1, gScenarioBufferData147_HT_BOX_3X3_08U, 32, gScenarioKernelData147_HT_BOX_3X3_08U, 4},
{26, 5, 1752, 0, 1, gScenarioBufferData148_HT_BOX_3X3_08U, 32, gScenarioKernelData148_HT_BOX_3X3_08U, 4},
{28, 5, 1880, 0, 1, gScenarioBufferData149_HT_BOX_3X3_08U, 32, gScenarioKernelData149_HT_BOX_3X3_08U, 4},
{30, 5, 2008, 0, 1, gScenarioBufferData150_HT_BOX_3X3_08U, 32, gScenarioKernelData150_HT_BOX_3X3_08U, 4},
{32, 5, 2136, 0, 1, gScenarioBufferData151_HT_BOX_3X3_08U, 32, gScenarioKernelData151_HT_BOX_3X3_08U, 4},
{36, 5, 2392, 0, 1, gScenarioBufferData152_HT_BOX_3X3_08U, 32, gScenarioKernelData152_HT_BOX_3X3_08U, 4},
{40, 5, 2648, 0, 1, gScenarioBufferData153_HT_BOX_3X3_08U, 32, gScenarioKernelData153_HT_BOX_3X3_08U, 4},
{44, 5, 2904, 0, 1, gScenarioBufferData154_HT_BOX_3X3_08U, 32, gScenarioKernelData154_HT_BOX_3X3_08U, 4},
{48, 5, 3160, 0, 1, gScenarioBufferData155_HT_BOX_3X3_08U, 32, gScenarioKernelData155_HT_BOX_3X3_08U, 4},
{52, 5, 3416, 0, 1, gScenarioBufferData156_HT_BOX_3X3_08U, 32, gScenarioKernelData156_HT_BOX_3X3_08U, 4},
{56, 5, 3672, 0, 1, gScenarioBufferData157_HT_BOX_3X3_08U, 32, gScenarioKernelData157_HT_BOX_3X3_08U, 4},
{1, 6, 180, 0, 1, gScenarioBufferData158_HT_BOX_3X3_08U, 32, gScenarioKernelData158_HT_BOX_3X3_08U, 4},
{2, 6, 256, 0, 1, gScenarioBufferData159_HT_BOX_3X3_08U, 32, gScenarioKernelData159_HT_BOX_3X3_08U, 4},
{3, 6, 332, 0, 1, gScenarioBufferData160_HT_BOX_3X3_08U, 32, gScenarioKernelData160_HT_BOX_3X3_08U, 4},
{4, 6, 408, 0, 1, gScenarioBufferData161_HT_BOX_3X3_08U, 32, gScenarioKernelData161_HT_BOX_3X3_08U, 4},
{5, 6, 484, 0, 1, gScenarioBufferData162_HT_BOX_3X3_08U, 32, gScenarioKernelData162_HT_BOX_3X3_08U, 4},
{6, 6, 560, 0, 1, gScenarioBufferData163_HT_BOX_3X3_08U, 32, gScenarioKernelData163_HT_BOX_3X3_08U, 4},
{7, 6, 636, 0, 1, gScenarioBufferData164_HT_BOX_3X3_08U, 32, gScenarioKernelData164_HT_BOX_3X3_08U, 4},
{8, 6, 712, 0, 1, gScenarioBufferData165_HT_BOX_3X3_08U, 32, gScenarioKernelData165_HT_BOX_3X3_08U, 4},
{9, 6, 788, 0, 1, gScenarioBufferData166_HT_BOX_3X3_08U, 32, gScenarioKernelData166_HT_BOX_3X3_08U, 4},
{10, 6, 864, 0, 1, gScenarioBufferData167_HT_BOX_3X3_08U, 32, gScenarioKernelData167_HT_BOX_3X3_08U, 4},
{11, 6, 940, 0, 1, gScenarioBufferData168_HT_BOX_3X3_08U, 32, gScenarioKernelData168_HT_BOX_3X3_08U, 4},
{12, 6, 1016, 0, 1, gScenarioBufferData169_HT_BOX_3X3_08U, 32, gScenarioKernelData169_HT_BOX_3X3_08U, 4},
{13, 6, 1092, 0, 1, gScenarioBufferData170_HT_BOX_3X3_08U, 32, gScenarioKernelData170_HT_BOX_3X3_08U, 4},
{14, 6, 1168, 0, 1, gScenarioBufferData171_HT_BOX_3X3_08U, 32, gScenarioKernelData171_HT_BOX_3X3_08U, 4},
{15, 6, 1244, 0, 1, gScenarioBufferData172_HT_BOX_3X3_08U, 32, gScenarioKernelData172_HT_BOX_3X3_08U, 4},
{16, 6, 1320, 0, 1, gScenarioBufferData173_HT_BOX_3X3_08U, 32, gScenarioKernelData173_HT_BOX_3X3_08U, 4},
{18, 6, 1472, 0, 1, gScenarioBufferData174_HT_BOX_3X3_08U, 32, gScenarioKernelData174_HT_BOX_3X3_08U, 4},
{20, 6, 1624, 0, 1, gScenarioBufferData175_HT_BOX_3X3_08U, 32, gScenarioKernelData175_HT_BOX_3X3_08U, 4},
{22, 6, 1776, 0, 1, gScenarioBufferData176_HT_BOX_3X3_08U, 32, gScenarioKernelData176_HT_BOX_3X3_08U, 4},
{24, 6, 1928, 0, 1, gScenarioBufferData177_HT_BOX_3X3_08U, 32, gScenarioKernelData177_HT_BOX_3X3_08U, 4},
{26, 6, 2080, 0, 1, gScenarioBufferData178_HT_BOX_3X3_08U, 32, gScenarioKernelData178_HT_BOX_3X3_08U, 4},
{28, 6, 2232, 0, 1, gScenarioBufferData179_HT_BOX_3X3_08U, 32, gScenarioKernelData179_HT_BOX_3X3_08U, 4},
{30, 6, 2384, 0, 1, gScenarioBufferData180_HT_BOX_3X3_08U, 32, gScenarioKernelData180_HT_BOX_3X3_08U, 4},
{32, 6, 2536, 0, 1, gScenarioBufferData181_HT_BOX_3X3_08U, 32, gScenarioKernelData181_HT_BOX_3X3_08U, 4},
{36, 6, 2840, 0, 1, gScenarioBufferData182_HT_BOX_3X3_08U, 32, gScenarioKernelData182_HT_BOX_3X3_08U, 4},
{40, 6, 3144, 0, 1, gScenarioBufferData183_HT_BOX_3X3_08U, 32, gScenarioKernelData183_HT_BOX_3X3_08U, 4},
{44, 6, 3448, 0, 1, gScenarioBufferData184_HT_BOX_3X3_08U, 32, gScenarioKernelData184_HT_BOX_3X3_08U, 4},
{48, 6, 3752, 0, 1, gScenarioBufferData185_HT_BOX_3X3_08U, 32, gScenarioKernelData185_HT_BOX_3X3_08U, 4},
{1, 8, 236, 0, 1, gScenarioBufferData186_HT_BOX_3X3_08U, 32, gScenarioKernelData186_HT_BOX_3X3_08U, 4},
{2, 8, 336, 0, 1, gScenarioBufferData187_HT_BOX_3X3_08U, 32, gScenarioKernelData187_HT_BOX_3X3_08U, 4},
{3, 8, 436, 0, 1, gScenarioBufferData188_HT_BOX_3X3_08U, 32, gScenarioKernelData188_HT_BOX_3X3_08U, 4},
{4, 8, 536, 0, 1, gScenarioBufferData189_HT_BOX_3X3_08U, 32, gScenarioKernelData189_HT_BOX_3X3_08U, 4},
{5, 8, 636, 0, 1, gScenarioBufferData190_HT_BOX_3X3_08U, 32, gScenarioKernelData190_HT_BOX_3X3_08U, 4},
{6, 8, 736, 0, 1, gScenarioBufferData191_HT_BOX_3X3_08U, 32, gScenarioKernelData191_HT_BOX_3X3_08U, 4},
{7, 8, 836, 0, 1, gScenarioBufferData192_HT_BOX_3X3_08U, 32, gScenarioKernelData192_HT_BOX_3X3_08U, 4},
{8, 8, 936, 0, 1, gScenarioBufferData193_HT_BOX_3X3_08U, 32, gScenarioKernelData193_HT_BOX_3X3_08U, 4},
{9, 8, 1036, 0, 1, gScenarioBufferData194_HT_BOX_3X3_08U, 32, gScenarioKernelData194_HT_BOX_3X3_08U, 4},
{10, 8, 1136, 0, 1, gScenarioBufferData195_HT_BOX_3X3_08U, 32, gScenarioKernelData195_HT_BOX_3X3_08U, 4},
{11, 8, 1236, 0, 1, gScenarioBufferData196_HT_BOX_3X3_08U, 32, gScenarioKernelData196_HT_BOX_3X3_08U, 4},
{12, 8, 1336, 0, 1, gScenarioBufferData197_HT_BOX_3X3_08U, 32, gScenarioKernelData197_HT_BOX_3X3_08U, 4},
{13, 8, 1436, 0, 1, gScenarioBufferData198_HT_BOX_3X3_08U, 32, gScenarioKernelData198_HT_BOX_3X3_08U, 4},
{14, 8, 1536, 0, 1, gScenarioBufferData199_HT_BOX_3X3_08U, 32, gScenarioKernelData199_HT_BOX_3X3_08U, 4},
{15, 8, 1636, 0, 1, gScenarioBufferData200_HT_BOX_3X3_08U, 32, gScenarioKernelData200_HT_BOX_3X3_08U, 4},
{16, 8, 1736, 0, 1, gScenarioBufferData201_HT_BOX_3X3_08U, 32, gScenarioKernelData201_HT_BOX_3X3_08U, 4},
{18, 8, 1936, 0, 1, gScenarioBufferData202_HT_BOX_3X3_08U, 32, gScenarioKernelData202_HT_BOX_3X3_08U, 4},
{20, 8, 2136, 0, 1, gScenarioBufferData203_HT_BOX_3X3_08U, 32, gScenarioKernelData203_HT_BOX_3X3_08U, 4},
{22, 8, 2336, 0, 1, gScenarioBufferData204_HT_BOX_3X3_08U, 32, gScenarioKernelData204_HT_BOX_3X3_08U, 4},
{24, 8, 2536, 0, 1, gScenarioBufferData205_HT_BOX_3X3_08U, 32, gScenarioKernelData205_HT_BOX_3X3_08U, 4},
{26, 8, 2736, 0, 1, gScenarioBufferData206_HT_BOX_3X3_08U, 32, gScenarioKernelData206_HT_BOX_3X3_08U, 4},
{28, 8, 2936, 0, 1, gScenarioBufferData207_HT_BOX_3X3_08U, 32, gScenarioKernelData207_HT_BOX_3X3_08U, 4},
{30, 8, 3136, 0, 1, gScenarioBufferData208_HT_BOX_3X3_08U, 32, gScenarioKernelData208_HT_BOX_3X3_08U, 4},
{32, 8, 3336, 0, 1, gScenarioBufferData209_HT_BOX_3X3_08U, 32, gScenarioKernelData209_HT_BOX_3X3_08U, 4},
{36, 8, 3736, 0, 1, gScenarioBufferData210_HT_BOX_3X3_08U, 32, gScenarioKernelData210_HT_BOX_3X3_08U, 4},
{1, 10, 292, 0, 1, gScenarioBufferData211_HT_BOX_3X3_08U, 32, gScenarioKernelData211_HT_BOX_3X3_08U, 4},
{2, 10, 416, 0, 1, gScenarioBufferData212_HT_BOX_3X3_08U, 32, gScenarioKernelData212_HT_BOX_3X3_08U, 4},
{3, 10, 540, 0, 1, gScenarioBufferData213_HT_BOX_3X3_08U, 32, gScenarioKernelData213_HT_BOX_3X3_08U, 4},
{4, 10, 664, 0, 1, gScenarioBufferData214_HT_BOX_3X3_08U, 32, gScenarioKernelData214_HT_BOX_3X3_08U, 4},
{5, 10, 788, 0, 1, gScenarioBufferData215_HT_BOX_3X3_08U, 32, gScenarioKernelData215_HT_BOX_3X3_08U, 4},
{6, 10, 912, 0, 1, gScenarioBufferData216_HT_BOX_3X3_08U, 32, gScenarioKernelData216_HT_BOX_3X3_08U, 4},
{7, 10, 1036, 0, 1, gScenarioBufferData217_HT_BOX_3X3_08U, 32, gScenarioKernelData217_HT_BOX_3X3_08U, 4},
{8, 10, 1160, 0, 1, gScenarioBufferData218_HT_BOX_3X3_08U, 32, gScenarioKernelData218_HT_BOX_3X3_08U, 4},
{9, 10, 1284, 0, 1, gScenarioBufferData219_HT_BOX_3X3_08U, 32, gScenarioKernelData219_HT_BOX_3X3_08U, 4},
{10, 10, 1408, 0, 1, gScenarioBufferData220_HT_BOX_3X3_08U, 32, gScenarioKernelData220_HT_BOX_3X3_08U, 4},
{11, 10, 1532, 0, 1, gScenarioBufferData221_HT_BOX_3X3_08U, 32, gScenarioKernelData221_HT_BOX_3X3_08U, 4},
{12, 10, 1656, 0, 1, gScenarioBufferData222_HT_BOX_3X3_08U, 32, gScenarioKernelData222_HT_BOX_3X3_08U, 4},
{13, 10, 1780, 0, 1, gScenarioBufferData223_HT_BOX_3X3_08U, 32, gScenarioKernelData223_HT_BOX_3X3_08U, 4},
{14, 10, 1904, 0, 1, gScenarioBufferData224_HT_BOX_3X3_08U, 32, gScenarioKernelData224_HT_BOX_3X3_08U, 4},
{15, 10, 2028, 0, 1, gScenarioBufferData225_HT_BOX_3X3_08U, 32, gScenarioKernelData225_HT_BOX_3X3_08U, 4},
{16, 10, 2152, 0, 1, gScenarioBufferData226_HT_BOX_3X3_08U, 32, gScenarioKernelData226_HT_BOX_3X3_08U, 4},
{18, 10, 2400, 0, 1, gScenarioBufferData227_HT_BOX_3X3_08U, 32, gScenarioKernelData227_HT_BOX_3X3_08U, 4},
{20, 10, 2648, 0, 1, gScenarioBufferData228_HT_BOX_3X3_08U, 32, gScenarioKernelData228_HT_BOX_3X3_08U, 4},
{22, 10, 2896, 0, 1, gScenarioBufferData229_HT_BOX_3X3_08U, 32, gScenarioKernelData229_HT_BOX_3X3_08U, 4},
{24, 10, 3144, 0, 1, gScenarioBufferData230_HT_BOX_3X3_08U, 32, gScenarioKernelData230_HT_BOX_3X3_08U, 4},
{26, 10, 3392, 0, 1, gScenarioBufferData231_HT_BOX_3X3_08U, 32, gScenarioKernelData231_HT_BOX_3X3_08U, 4},
{28, 10, 3640, 0, 1, gScenarioBufferData232_HT_BOX_3X3_08U, 32, gScenarioKernelData232_HT_BOX_3X3_08U, 4},
{1, 12, 348, 0, 1, gScenarioBufferData233_HT_BOX_3X3_08U, 32, gScenarioKernelData233_HT_BOX_3X3_08U, 4},
{2, 12, 496, 0, 1, gScenarioBufferData234_HT_BOX_3X3_08U, 32, gScenarioKernelData234_HT_BOX_3X3_08U, 4},
{3, 12, 644, 0, 1, gScenarioBufferData235_HT_BOX_3X3_08U, 32, gScenarioKernelData235_HT_BOX_3X3_08U, 4},
{4, 12, 792, 0, 1, gScenarioBufferData236_HT_BOX_3X3_08U, 32, gScenarioKernelData236_HT_BOX_3X3_08U, 4},
{5, 12, 940, 0, 1, gScenarioBufferData237_HT_BOX_3X3_08U, 32, gScenarioKernelData237_HT_BOX_3X3_08U, 4},
{6, 12, 1088, 0, 1, gScenarioBufferData238_HT_BOX_3X3_08U, 32, gScenarioKernelData238_HT_BOX_3X3_08U, 4},
{7, 12, 1236, 0, 1, gScenarioBufferData239_HT_BOX_3X3_08U, 32, gScenarioKernelData239_HT_BOX_3X3_08U, 4},
{8, 12, 1384, 0, 1, gScenarioBufferData240_HT_BOX_3X3_08U, 32, gScenarioKernelData240_HT_BOX_3X3_08U, 4},
{9, 12, 1532, 0, 1, gScenarioBufferData241_HT_BOX_3X3_08U, 32, gScenarioKernelData241_HT_BOX_3X3_08U, 4},
{10, 12, 1680, 0, 1, gScenarioBufferData242_HT_BOX_3X3_08U, 32, gScenarioKernelData242_HT_BOX_3X3_08U, 4},
{11, 12, 1828, 0, 1, gScenarioBufferData243_HT_BOX_3X3_08U, 32, gScenarioKernelData243_HT_BOX_3X3_08U, 4},
{12, 12, 1976, 0, 1, gScenarioBufferData244_HT_BOX_3X3_08U, 32, gScenarioKernelData244_HT_BOX_3X3_08U, 4},
{13, 12, 2124, 0, 1, gScenarioBufferData245_HT_BOX_3X3_08U, 32, gScenarioKernelData245_HT_BOX_3X3_08U, 4},
{14, 12, 2272, 0, 1, gScenarioBufferData246_HT_BOX_3X3_08U, 32, gScenarioKernelData246_HT_BOX_3X3_08U, 4},
{15, 12, 2420, 0, 1, gScenarioBufferData247_HT_BOX_3X3_08U, 32, gScenarioKernelData247_HT_BOX_3X3_08U, 4},
{16, 12, 2568, 0, 1, gScenarioBufferData248_HT_BOX_3X3_08U, 32, gScenarioKernelData248_HT_BOX_3X3_08U, 4},
{18, 12, 2864, 0, 1, gScenarioBufferData249_HT_BOX_3X3_08U, 32, gScenarioKernelData249_HT_BOX_3X3_08U, 4},
{20, 12, 3160, 0, 1, gScenarioBufferData250_HT_BOX_3X3_08U, 32, gScenarioKernelData250_HT_BOX_3X3_08U, 4},
{22, 12, 3456, 0, 1, gScenarioBufferData251_HT_BOX_3X3_08U, 32, gScenarioKernelData251_HT_BOX_3X3_08U, 4},
{24, 12, 3752, 0, 1, gScenarioBufferData252_HT_BOX_3X3_08U, 32, gScenarioKernelData252_HT_BOX_3X3_08U, 4},
{1, 14, 404, 0, 1, gScenarioBufferData253_HT_BOX_3X3_08U, 32, gScenarioKernelData253_HT_BOX_3X3_08U, 4},
{2, 14, 576, 0, 1, gScenarioBufferData254_HT_BOX_3X3_08U, 32, gScenarioKernelData254_HT_BOX_3X3_08U, 4},
{3, 14, 748, 0, 1, gScenarioBufferData255_HT_BOX_3X3_08U, 32, gScenarioKernelData255_HT_BOX_3X3_08U, 4},
{4, 14, 920, 0, 1, gScenarioBufferData256_HT_BOX_3X3_08U, 32, gScenarioKernelData256_HT_BOX_3X3_08U, 4},
{5, 14, 1092, 0, 1, gScenarioBufferData257_HT_BOX_3X3_08U, 32, gScenarioKernelData257_HT_BOX_3X3_08U, 4},
{6, 14, 1264, 0, 1, gScenarioBufferData258_HT_BOX_3X3_08U, 32, gScenarioKernelData258_HT_BOX_3X3_08U, 4},
{7, 14, 1436, 0, 1, gScenarioBufferData259_HT_BOX_3X3_08U, 32, gScenarioKernelData259_HT_BOX_3X3_08U, 4},
{8, 14, 1608, 0, 1, gScenarioBufferData260_HT_BOX_3X3_08U, 32, gScenarioKernelData260_HT_BOX_3X3_08U, 4},
{9, 14, 1780, 0, 1, gScenarioBufferData261_HT_BOX_3X3_08U, 32, gScenarioKernelData261_HT_BOX_3X3_08U, 4},
{10, 14, 1952, 0, 1, gScenarioBufferData262_HT_BOX_3X3_08U, 32, gScenarioKernelData262_HT_BOX_3X3_08U, 4},
{11, 14, 2124, 0, 1, gScenarioBufferData263_HT_BOX_3X3_08U, 32, gScenarioKernelData263_HT_BOX_3X3_08U, 4},
{12, 14, 2296, 0, 1, gScenarioBufferData264_HT_BOX_3X3_08U, 32, gScenarioKernelData264_HT_BOX_3X3_08U, 4},
{13, 14, 2468, 0, 1, gScenarioBufferData265_HT_BOX_3X3_08U, 32, gScenarioKernelData265_HT_BOX_3X3_08U, 4},
{14, 14, 2640, 0, 1, gScenarioBufferData266_HT_BOX_3X3_08U, 32, gScenarioKernelData266_HT_BOX_3X3_08U, 4},
{15, 14, 2812, 0, 1, gScenarioBufferData267_HT_BOX_3X3_08U, 32, gScenarioKernelData267_HT_BOX_3X3_08U, 4},
{16, 14, 2984, 0, 1, gScenarioBufferData268_HT_BOX_3X3_08U, 32, gScenarioKernelData268_HT_BOX_3X3_08U, 4},
{18, 14, 3328, 0, 1, gScenarioBufferData269_HT_BOX_3X3_08U, 32, gScenarioKernelData269_HT_BOX_3X3_08U, 4},
{20, 14, 3672, 0, 1, gScenarioBufferData270_HT_BOX_3X3_08U, 32, gScenarioKernelData270_HT_BOX_3X3_08U, 4},
{1, 16, 460, 0, 1, gScenarioBufferData271_HT_BOX_3X3_08U, 32, gScenarioKernelData271_HT_BOX_3X3_08U, 4},
{2, 16, 656, 0, 1, gScenarioBufferData272_HT_BOX_3X3_08U, 32, gScenarioKernelData272_HT_BOX_3X3_08U, 4},
{3, 16, 852, 0, 1, gScenarioBufferData273_HT_BOX_3X3_08U, 32, gScenarioKernelData273_HT_BOX_3X3_08U, 4},
{4, 16, 1048, 0, 1, gScenarioBufferData274_HT_BOX_3X3_08U, 32, gScenarioKernelData274_HT_BOX_3X3_08U, 4},
{5, 16, 1244, 0, 1, gScenarioBufferData275_HT_BOX_3X3_08U, 32, gScenarioKernelData275_HT_BOX_3X3_08U, 4},
{6, 16, 1440, 0, 1, gScenarioBufferData276_HT_BOX_3X3_08U, 32, gScenarioKernelData276_HT_BOX_3X3_08U, 4},
{7, 16, 1636, 0, 1, gScenarioBufferData277_HT_BOX_3X3_08U, 32, gScenarioKernelData277_HT_BOX_3X3_08U, 4},
{8, 16, 1832, 0, 1, gScenarioBufferData278_HT_BOX_3X3_08U, 32, gScenarioKernelData278_HT_BOX_3X3_08U, 4},
{9, 16, 2028, 0, 1, gScenarioBufferData279_HT_BOX_3X3_08U, 32, gScenarioKernelData279_HT_BOX_3X3_08U, 4},
{10, 16, 2224, 0, 1, gScenarioBufferData280_HT_BOX_3X3_08U, 32, gScenarioKernelData280_HT_BOX_3X3_08U, 4},
{11, 16, 2420, 0, 1, gScenarioBufferData281_HT_BOX_3X3_08U, 32, gScenarioKernelData281_HT_BOX_3X3_08U, 4},
{12, 16, 2616, 0, 1, gScenarioBufferData282_HT_BOX_3X3_08U, 32, gScenarioKernelData282_HT_BOX_3X3_08U, 4},
{13, 16, 2812, 0, 1, gScenarioBufferData283_HT_BOX_3X3_08U, 32, gScenarioKernelData283_HT_BOX_3X3_08U, 4},
{14, 16, 3008, 0, 1, gScenarioBufferData284_HT_BOX_3X3_08U, 32, gScenarioKernelData284_HT_BOX_3X3_08U, 4},
{15, 16, 3204, 0, 1, gScenarioBufferData285_HT_BOX_3X3_08U, 32, gScenarioKernelData285_HT_BOX_3X3_08U, 4},
{16, 16, 3400, 0, 1, gScenarioBufferData286_HT_BOX_3X3_08U, 32, gScenarioKernelData286_HT_BOX_3X3_08U, 4},
{18, 16, 3792, 0, 1, gScenarioBufferData287_HT_BOX_3X3_08U, 32, gScenarioKernelData287_HT_BOX_3X3_08U, 4},
{1, 18, 516, 0, 1, gScenarioBufferData288_HT_BOX_3X3_08U, 32, gScenarioKernelData288_HT_BOX_3X3_08U, 4},
{2, 18, 736, 0, 1, gScenarioBufferData289_HT_BOX_3X3_08U, 32, gScenarioKernelData289_HT_BOX_3X3_08U, 4},
{3, 18, 956, 0, 1, gScenarioBufferData290_HT_BOX_3X3_08U, 32, gScenarioKernelData290_HT_BOX_3X3_08U, 4},
{4, 18, 1176, 0, 1, gScenarioBufferData291_HT_BOX_3X3_08U, 32, gScenarioKernelData291_HT_BOX_3X3_08U, 4},
{5, 18, 1396, 0, 1, gScenarioBufferData292_HT_BOX_3X3_08U, 32, gScenarioKernelData292_HT_BOX_3X3_08U, 4},
{6, 18, 1616, 0, 1, gScenarioBufferData293_HT_BOX_3X3_08U, 32, gScenarioKernelData293_HT_BOX_3X3_08U, 4},
{7, 18, 1836, 0, 1, gScenarioBufferData294_HT_BOX_3X3_08U, 32, gScenarioKernelData294_HT_BOX_3X3_08U, 4},
{8, 18, 2056, 0, 1, gScenarioBufferData295_HT_BOX_3X3_08U, 32, gScenarioKernelData295_HT_BOX_3X3_08U, 4},
{9, 18, 2276, 0, 1, gScenarioBufferData296_HT_BOX_3X3_08U, 32, gScenarioKernelData296_HT_BOX_3X3_08U, 4},
{10, 18, 2496, 0, 1, gScenarioBufferData297_HT_BOX_3X3_08U, 32, gScenarioKernelData297_HT_BOX_3X3_08U, 4},
{11, 18, 2716, 0, 1, gScenarioBufferData298_HT_BOX_3X3_08U, 32, gScenarioKernelData298_HT_BOX_3X3_08U, 4},
{12, 18, 2936, 0, 1, gScenarioBufferData299_HT_BOX_3X3_08U, 32, gScenarioKernelData299_HT_BOX_3X3_08U, 4},
{13, 18, 3156, 0, 1, gScenarioBufferData300_HT_BOX_3X3_08U, 32, gScenarioKernelData300_HT_BOX_3X3_08U, 4},
{14, 18, 3376, 0, 1, gScenarioBufferData301_HT_BOX_3X3_08U, 32, gScenarioKernelData301_HT_BOX_3X3_08U, 4},
{15, 18, 3596, 0, 1, gScenarioBufferData302_HT_BOX_3X3_08U, 32, gScenarioKernelData302_HT_BOX_3X3_08U, 4},
{16, 18, 3816, 0, 1, gScenarioBufferData303_HT_BOX_3X3_08U, 32, gScenarioKernelData303_HT_BOX_3X3_08U, 4},
{1, 20, 572, 0, 1, gScenarioBufferData304_HT_BOX_3X3_08U, 32, gScenarioKernelData304_HT_BOX_3X3_08U, 4},
{2, 20, 816, 0, 1, gScenarioBufferData305_HT_BOX_3X3_08U, 32, gScenarioKernelData305_HT_BOX_3X3_08U, 4},
{3, 20, 1060, 0, 1, gScenarioBufferData306_HT_BOX_3X3_08U, 32, gScenarioKernelData306_HT_BOX_3X3_08U, 4},
{4, 20, 1304, 0, 1, gScenarioBufferData307_HT_BOX_3X3_08U, 32, gScenarioKernelData307_HT_BOX_3X3_08U, 4},
{5, 20, 1548, 0, 1, gScenarioBufferData308_HT_BOX_3X3_08U, 32, gScenarioKernelData308_HT_BOX_3X3_08U, 4},
{6, 20, 1792, 0, 1, gScenarioBufferData309_HT_BOX_3X3_08U, 32, gScenarioKernelData309_HT_BOX_3X3_08U, 4},
{7, 20, 2036, 0, 1, gScenarioBufferData310_HT_BOX_3X3_08U, 32, gScenarioKernelData310_HT_BOX_3X3_08U, 4},
{8, 20, 2280, 0, 1, gScenarioBufferData311_HT_BOX_3X3_08U, 32, gScenarioKernelData311_HT_BOX_3X3_08U, 4},
{9, 20, 2524, 0, 1, gScenarioBufferData312_HT_BOX_3X3_08U, 32, gScenarioKernelData312_HT_BOX_3X3_08U, 4},
{10, 20, 2768, 0, 1, gScenarioBufferData313_HT_BOX_3X3_08U, 32, gScenarioKernelData313_HT_BOX_3X3_08U, 4},
{11, 20, 3012, 0, 1, gScenarioBufferData314_HT_BOX_3X3_08U, 32, gScenarioKernelData314_HT_BOX_3X3_08U, 4},
{12, 20, 3256, 0, 1, gScenarioBufferData315_HT_BOX_3X3_08U, 32, gScenarioKernelData315_HT_BOX_3X3_08U, 4},
{13, 20, 3500, 0, 1, gScenarioBufferData316_HT_BOX_3X3_08U, 32, gScenarioKernelData316_HT_BOX_3X3_08U, 4},
{14, 20, 3744, 0, 1, gScenarioBufferData317_HT_BOX_3X3_08U, 32, gScenarioKernelData317_HT_BOX_3X3_08U, 4},
{1, 22, 628, 0, 1, gScenarioBufferData318_HT_BOX_3X3_08U, 32, gScenarioKernelData318_HT_BOX_3X3_08U, 4},
{2, 22, 896, 0, 1, gScenarioBufferData319_HT_BOX_3X3_08U, 32, gScenarioKernelData319_HT_BOX_3X3_08U, 4},
{3, 22, 1164, 0, 1, gScenarioBufferData320_HT_BOX_3X3_08U, 32, gScenarioKernelData320_HT_BOX_3X3_08U, 4},
{4, 22, 1432, 0, 1, gScenarioBufferData321_HT_BOX_3X3_08U, 32, gScenarioKernelData321_HT_BOX_3X3_08U, 4},
{5, 22, 1700, 0, 1, gScenarioBufferData322_HT_BOX_3X3_08U, 32, gScenarioKernelData322_HT_BOX_3X3_08U, 4},
{6, 22, 1968, 0, 1, gScenarioBufferData323_HT_BOX_3X3_08U, 32, gScenarioKernelData323_HT_BOX_3X3_08U, 4},
{7, 22, 2236, 0, 1, gScenarioBufferData324_HT_BOX_3X3_08U, 32, gScenarioKernelData324_HT_BOX_3X3_08U, 4},
{8, 22, 2504, 0, 1, gScenarioBufferData325_HT_BOX_3X3_08U, 32, gScenarioKernelData325_HT_BOX_3X3_08U, 4},
{9, 22, 2772, 0, 1, gScenarioBufferData326_HT_BOX_3X3_08U, 32, gScenarioKernelData326_HT_BOX_3X3_08U, 4},
{10, 22, 3040, 0, 1, gScenarioBufferData327_HT_BOX_3X3_08U, 32, gScenarioKernelData327_HT_BOX_3X3_08U, 4},
{11, 22, 3308, 0, 1, gScenarioBufferData328_HT_BOX_3X3_08U, 32, gScenarioKernelData328_HT_BOX_3X3_08U, 4},
{12, 22, 3576, 0, 1, gScenarioBufferData329_HT_BOX_3X3_08U, 32, gScenarioKernelData329_HT_BOX_3X3_08U, 4},
{1, 24, 684, 0, 1, gScenarioBufferData330_HT_BOX_3X3_08U, 32, gScenarioKernelData330_HT_BOX_3X3_08U, 4},
{2, 24, 976, 0, 1, gScenarioBufferData331_HT_BOX_3X3_08U, 32, gScenarioKernelData331_HT_BOX_3X3_08U, 4},
{3, 24, 1268, 0, 1, gScenarioBufferData332_HT_BOX_3X3_08U, 32, gScenarioKernelData332_HT_BOX_3X3_08U, 4},
{4, 24, 1560, 0, 1, gScenarioBufferData333_HT_BOX_3X3_08U, 32, gScenarioKernelData333_HT_BOX_3X3_08U, 4},
{5, 24, 1852, 0, 1, gScenarioBufferData334_HT_BOX_3X3_08U, 32, gScenarioKernelData334_HT_BOX_3X3_08U, 4},
{6, 24, 2144, 0, 1, gScenarioBufferData335_HT_BOX_3X3_08U, 32, gScenarioKernelData335_HT_BOX_3X3_08U, 4},
{7, 24, 2436, 0, 1, gScenarioBufferData336_HT_BOX_3X3_08U, 32, gScenarioKernelData336_HT_BOX_3X3_08U, 4},
{8, 24, 2728, 0, 1, gScenarioBufferData337_HT_BOX_3X3_08U, 32, gScenarioKernelData337_HT_BOX_3X3_08U, 4},
{9, 24, 3020, 0, 1, gScenarioBufferData338_HT_BOX_3X3_08U, 32, gScenarioKernelData338_HT_BOX_3X3_08U, 4},
{10, 24, 3312, 0, 1, gScenarioBufferData339_HT_BOX_3X3_08U, 32, gScenarioKernelData339_HT_BOX_3X3_08U, 4},
{11, 24, 3604, 0, 1, gScenarioBufferData340_HT_BOX_3X3_08U, 32, gScenarioKernelData340_HT_BOX_3X3_08U, 4},
{1, 26, 740, 0, 1, gScenarioBufferData341_HT_BOX_3X3_08U, 32, gScenarioKernelData341_HT_BOX_3X3_08U, 4},
{2, 26, 1056, 0, 1, gScenarioBufferData342_HT_BOX_3X3_08U, 32, gScenarioKernelData342_HT_BOX_3X3_08U, 4},
{3, 26, 1372, 0, 1, gScenarioBufferData343_HT_BOX_3X3_08U, 32, gScenarioKernelData343_HT_BOX_3X3_08U, 4},
{4, 26, 1688, 0, 1, gScenarioBufferData344_HT_BOX_3X3_08U, 32, gScenarioKernelData344_HT_BOX_3X3_08U, 4},
{5, 26, 2004, 0, 1, gScenarioBufferData345_HT_BOX_3X3_08U, 32, gScenarioKernelData345_HT_BOX_3X3_08U, 4},
{6, 26, 2320, 0, 1, gScenarioBufferData346_HT_BOX_3X3_08U, 32, gScenarioKernelData346_HT_BOX_3X3_08U, 4},
{7, 26, 2636, 0, 1, gScenarioBufferData347_HT_BOX_3X3_08U, 32, gScenarioKernelData347_HT_BOX_3X3_08U, 4},
{8, 26, 2952, 0, 1, gScenarioBufferData348_HT_BOX_3X3_08U, 32, gScenarioKernelData348_HT_BOX_3X3_08U, 4},
{9, 26, 3268, 0, 1, gScenarioBufferData349_HT_BOX_3X3_08U, 32, gScenarioKernelData349_HT_BOX_3X3_08U, 4},
{10, 26, 3584, 0, 1, gScenarioBufferData350_HT_BOX_3X3_08U, 32, gScenarioKernelData350_HT_BOX_3X3_08U, 4},
{1, 28, 796, 0, 1, gScenarioBufferData351_HT_BOX_3X3_08U, 32, gScenarioKernelData351_HT_BOX_3X3_08U, 4},
{2, 28, 1136, 0, 1, gScenarioBufferData352_HT_BOX_3X3_08U, 32, gScenarioKernelData352_HT_BOX_3X3_08U, 4},
{3, 28, 1476, 0, 1, gScenarioBufferData353_HT_BOX_3X3_08U, 32, gScenarioKernelData353_HT_BOX_3X3_08U, 4},
{4, 28, 1816, 0, 1, gScenarioBufferData354_HT_BOX_3X3_08U, 32, gScenarioKernelData354_HT_BOX_3X3_08U, 4},
{5, 28, 2156, 0, 1, gScenarioBufferData355_HT_BOX_3X3_08U, 32, gScenarioKernelData355_HT_BOX_3X3_08U, 4},
{6, 28, 2496, 0, 1, gScenarioBufferData356_HT_BOX_3X3_08U, 32, gScenarioKernelData356_HT_BOX_3X3_08U, 4},
{7, 28, 2836, 0, 1, gScenarioBufferData357_HT_BOX_3X3_08U, 32, gScenarioKernelData357_HT_BOX_3X3_08U, 4},
{8, 28, 3176, 0, 1, gScenarioBufferData358_HT_BOX_3X3_08U, 32, gScenarioKernelData358_HT_BOX_3X3_08U, 4},
{9, 28, 3516, 0, 1, gScenarioBufferData359_HT_BOX_3X3_08U, 32, gScenarioKernelData359_HT_BOX_3X3_08U, 4},
{1, 30, 852, 0, 1, gScenarioBufferData360_HT_BOX_3X3_08U, 32, gScenarioKernelData360_HT_BOX_3X3_08U, 4},
{2, 30, 1216, 0, 1, gScenarioBufferData361_HT_BOX_3X3_08U, 32, gScenarioKernelData361_HT_BOX_3X3_08U, 4},
{3, 30, 1580, 0, 1, gScenarioBufferData362_HT_BOX_3X3_08U, 32, gScenarioKernelData362_HT_BOX_3X3_08U, 4},
{4, 30, 1944, 0, 1, gScenarioBufferData363_HT_BOX_3X3_08U, 32, gScenarioKernelData363_HT_BOX_3X3_08U, 4},
{5, 30, 2308, 0, 1, gScenarioBufferData364_HT_BOX_3X3_08U, 32, gScenarioKernelData364_HT_BOX_3X3_08U, 4},
{6, 30, 2672, 0, 1, gScenarioBufferData365_HT_BOX_3X3_08U, 32, gScenarioKernelData365_HT_BOX_3X3_08U, 4},
{7, 30, 3036, 0, 1, gScenarioBufferData366_HT_BOX_3X3_08U, 32, gScenarioKernelData366_HT_BOX_3X3_08U, 4},
{8, 30, 3400, 0, 1, gScenarioBufferData367_HT_BOX_3X3_08U, 32, gScenarioKernelData367_HT_BOX_3X3_08U, 4},
{9, 30, 3764, 0, 1, gScenarioBufferData368_HT_BOX_3X3_08U, 32, gScenarioKernelData368_HT_BOX_3X3_08U, 4},
{1, 32, 908, 0, 1, gScenarioBufferData369_HT_BOX_3X3_08U, 32, gScenarioKernelData369_HT_BOX_3X3_08U, 4},
{2, 32, 1296, 0, 1, gScenarioBufferData370_HT_BOX_3X3_08U, 32, gScenarioKernelData370_HT_BOX_3X3_08U, 4},
{3, 32, 1684, 0, 1, gScenarioBufferData371_HT_BOX_3X3_08U, 32, gScenarioKernelData371_HT_BOX_3X3_08U, 4},
{4, 32, 2072, 0, 1, gScenarioBufferData372_HT_BOX_3X3_08U, 32, gScenarioKernelData372_HT_BOX_3X3_08U, 4},
{5, 32, 2460, 0, 1, gScenarioBufferData373_HT_BOX_3X3_08U, 32, gScenarioKernelData373_HT_BOX_3X3_08U, 4},
{6, 32, 2848, 0, 1, gScenarioBufferData374_HT_BOX_3X3_08U, 32, gScenarioKernelData374_HT_BOX_3X3_08U, 4},
{7, 32, 3236, 0, 1, gScenarioBufferData375_HT_BOX_3X3_08U, 32, gScenarioKernelData375_HT_BOX_3X3_08U, 4},
{8, 32, 3624, 0, 1, gScenarioBufferData376_HT_BOX_3X3_08U, 32, gScenarioKernelData376_HT_BOX_3X3_08U, 4}
};

static acf_scenario_list gScenarioList_HT_BOX_3X3_08U = {
377, //number of scenarios
gScenarioArray_HT_BOX_3X3_08U};
//**************************************************************

class HT_BOX_3X3_08U : public ACF_Process_APU
{

public:
   HT_BOX_3X3_08U(int32_t apex_id = 0) : ACF_Process_APU(apex_id)
   {} 

   int32_t Initialize()
   {
      int32_t lRetVal = 0;

      if (!Initialized()) //initialization steps that only need to occur once
      {
         SetProcessIdentifier("HT_BOX_3X3_08U");
         SetApuLoadInfo(HT_BOX_3X3_08U_LOAD_SEGMENTS,
                        HT_BOX_3X3_08U_LOAD_PMEM, HT_BOX_3X3_08U_LOAD_PMEM_SIZE,
                        HT_BOX_3X3_08U_LOAD_DMEM, HT_BOX_3X3_08U_LOAD_DMEM_SIZE,
                        0, 0); //assuming _LOAD_CMEM does not exist

         lRetVal = NonParametricInitialization();
         if (lRetVal != 0) return lRetVal;

         FlagSpatialDep();

         //***NOTE: the order in which the following ports are added is meaningful; do not change!

         AddPort("INPUT_0", icp::DATATYPE_08U, 1, 1, 2, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0);
         AddPort("OUTPUT_0", icp::DATATYPE_08U, 1, 1, 2, 1, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0);

         CfgScenarios(&gScenarioList_HT_BOX_3X3_08U);

         FlagAsInitialized();
      }

      lRetVal = SelectApuConfiguration(mApuCfg, mApexId); //by default mApuCfg = ACF_APU_CFG__APEX0_DEFAULT and mApexId = 0

      return lRetVal;
   }
};

#endif //_ACF_PROCESS_APU_HT_BOX_3X3_08U