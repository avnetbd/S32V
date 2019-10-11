#ifndef _ACF_PROCESS_APU_APU_GAUSS_5X5
#define _ACF_PROCESS_APU_APU_GAUSS_5X5

#include <acf_process_apu.h>
#include <APU_GAUSS_5X5_APU_LOAD.h> //APU load associated with this process
#include <stdint.h>


//SCENARIO LIST*************************************************
static acf_scenario_buffer_data gScenarioBufferData0_APU_GAUSS_5X5[] = {{2, 1, 7, 0, {2, 2, 2, 2}}, {2, 1, 2, 2, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData1_APU_GAUSS_5X5[] = {{4, 1, 7, 0, {2, 2, 2, 2}}, {4, 1, 2, 2, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData2_APU_GAUSS_5X5[] = {{6, 1, 7, 0, {2, 2, 2, 2}}, {6, 1, 2, 2, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData3_APU_GAUSS_5X5[] = {{8, 1, 7, 0, {2, 2, 2, 2}}, {8, 1, 2, 2, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData4_APU_GAUSS_5X5[] = {{10, 1, 7, 0, {2, 2, 2, 2}}, {10, 1, 2, 2, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData5_APU_GAUSS_5X5[] = {{12, 1, 7, 0, {2, 2, 2, 2}}, {12, 1, 2, 2, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData6_APU_GAUSS_5X5[] = {{14, 1, 7, 0, {2, 2, 2, 2}}, {14, 1, 2, 2, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData7_APU_GAUSS_5X5[] = {{16, 1, 7, 0, {2, 2, 2, 2}}, {16, 1, 2, 2, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData8_APU_GAUSS_5X5[] = {{18, 1, 7, 0, {2, 2, 2, 2}}, {18, 1, 2, 2, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData9_APU_GAUSS_5X5[] = {{20, 1, 7, 0, {2, 2, 2, 2}}, {20, 1, 2, 2, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData10_APU_GAUSS_5X5[] = {{22, 1, 7, 0, {2, 2, 2, 2}}, {22, 1, 2, 2, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData11_APU_GAUSS_5X5[] = {{24, 1, 7, 0, {2, 2, 2, 2}}, {24, 1, 2, 2, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData12_APU_GAUSS_5X5[] = {{26, 1, 7, 0, {2, 2, 2, 2}}, {26, 1, 2, 2, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData13_APU_GAUSS_5X5[] = {{28, 1, 7, 0, {2, 2, 2, 2}}, {28, 1, 2, 2, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData14_APU_GAUSS_5X5[] = {{30, 1, 7, 0, {2, 2, 2, 2}}, {30, 1, 2, 2, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData15_APU_GAUSS_5X5[] = {{32, 1, 7, 0, {2, 2, 2, 2}}, {32, 1, 2, 2, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData16_APU_GAUSS_5X5[] = {{36, 1, 7, 0, {2, 2, 2, 2}}, {36, 1, 2, 2, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData17_APU_GAUSS_5X5[] = {{40, 1, 7, 0, {2, 2, 2, 2}}, {40, 1, 2, 2, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData18_APU_GAUSS_5X5[] = {{44, 1, 7, 0, {2, 2, 2, 2}}, {44, 1, 2, 2, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData19_APU_GAUSS_5X5[] = {{48, 1, 7, 0, {2, 2, 2, 2}}, {48, 1, 2, 2, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData20_APU_GAUSS_5X5[] = {{52, 1, 7, 0, {2, 2, 2, 2}}, {52, 1, 2, 2, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData21_APU_GAUSS_5X5[] = {{56, 1, 7, 0, {2, 2, 2, 2}}, {56, 1, 2, 2, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData22_APU_GAUSS_5X5[] = {{60, 1, 7, 0, {2, 2, 2, 2}}, {60, 1, 2, 2, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData23_APU_GAUSS_5X5[] = {{64, 1, 7, 0, {2, 2, 2, 2}}, {64, 1, 2, 2, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData24_APU_GAUSS_5X5[] = {{72, 1, 7, 0, {2, 2, 2, 2}}, {72, 1, 2, 2, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData25_APU_GAUSS_5X5[] = {{80, 1, 7, 0, {2, 2, 2, 2}}, {80, 1, 2, 2, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData26_APU_GAUSS_5X5[] = {{88, 1, 7, 0, {2, 2, 2, 2}}, {88, 1, 2, 2, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData27_APU_GAUSS_5X5[] = {{96, 1, 7, 0, {2, 2, 2, 2}}, {96, 1, 2, 2, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData28_APU_GAUSS_5X5[] = {{104, 1, 7, 0, {2, 2, 2, 2}}, {104, 1, 2, 2, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData29_APU_GAUSS_5X5[] = {{112, 1, 7, 0, {2, 2, 2, 2}}, {112, 1, 2, 2, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData30_APU_GAUSS_5X5[] = {{120, 1, 7, 0, {2, 2, 2, 2}}, {120, 1, 2, 2, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData31_APU_GAUSS_5X5[] = {{128, 1, 7, 0, {2, 2, 2, 2}}, {128, 1, 2, 2, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData32_APU_GAUSS_5X5[] = {{2, 2, 4, 0, {2, 2, 2, 2}}, {2, 2, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData33_APU_GAUSS_5X5[] = {{4, 2, 4, 0, {2, 2, 2, 2}}, {4, 2, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData34_APU_GAUSS_5X5[] = {{6, 2, 4, 0, {2, 2, 2, 2}}, {6, 2, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData35_APU_GAUSS_5X5[] = {{8, 2, 4, 0, {2, 2, 2, 2}}, {8, 2, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData36_APU_GAUSS_5X5[] = {{10, 2, 4, 0, {2, 2, 2, 2}}, {10, 2, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData37_APU_GAUSS_5X5[] = {{12, 2, 4, 0, {2, 2, 2, 2}}, {12, 2, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData38_APU_GAUSS_5X5[] = {{14, 2, 4, 0, {2, 2, 2, 2}}, {14, 2, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData39_APU_GAUSS_5X5[] = {{16, 2, 4, 0, {2, 2, 2, 2}}, {16, 2, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData40_APU_GAUSS_5X5[] = {{18, 2, 4, 0, {2, 2, 2, 2}}, {18, 2, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData41_APU_GAUSS_5X5[] = {{20, 2, 4, 0, {2, 2, 2, 2}}, {20, 2, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData42_APU_GAUSS_5X5[] = {{22, 2, 4, 0, {2, 2, 2, 2}}, {22, 2, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData43_APU_GAUSS_5X5[] = {{24, 2, 4, 0, {2, 2, 2, 2}}, {24, 2, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData44_APU_GAUSS_5X5[] = {{26, 2, 4, 0, {2, 2, 2, 2}}, {26, 2, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData45_APU_GAUSS_5X5[] = {{28, 2, 4, 0, {2, 2, 2, 2}}, {28, 2, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData46_APU_GAUSS_5X5[] = {{30, 2, 4, 0, {2, 2, 2, 2}}, {30, 2, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData47_APU_GAUSS_5X5[] = {{32, 2, 4, 0, {2, 2, 2, 2}}, {32, 2, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData48_APU_GAUSS_5X5[] = {{36, 2, 4, 0, {2, 2, 2, 2}}, {36, 2, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData49_APU_GAUSS_5X5[] = {{40, 2, 4, 0, {2, 2, 2, 2}}, {40, 2, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData50_APU_GAUSS_5X5[] = {{44, 2, 4, 0, {2, 2, 2, 2}}, {44, 2, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData51_APU_GAUSS_5X5[] = {{48, 2, 4, 0, {2, 2, 2, 2}}, {48, 2, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData52_APU_GAUSS_5X5[] = {{52, 2, 4, 0, {2, 2, 2, 2}}, {52, 2, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData53_APU_GAUSS_5X5[] = {{56, 2, 4, 0, {2, 2, 2, 2}}, {56, 2, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData54_APU_GAUSS_5X5[] = {{60, 2, 4, 0, {2, 2, 2, 2}}, {60, 2, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData55_APU_GAUSS_5X5[] = {{64, 2, 4, 0, {2, 2, 2, 2}}, {64, 2, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData56_APU_GAUSS_5X5[] = {{72, 2, 4, 0, {2, 2, 2, 2}}, {72, 2, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData57_APU_GAUSS_5X5[] = {{80, 2, 4, 0, {2, 2, 2, 2}}, {80, 2, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData58_APU_GAUSS_5X5[] = {{88, 2, 4, 0, {2, 2, 2, 2}}, {88, 2, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData59_APU_GAUSS_5X5[] = {{96, 2, 4, 0, {2, 2, 2, 2}}, {96, 2, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData60_APU_GAUSS_5X5[] = {{104, 2, 4, 0, {2, 2, 2, 2}}, {104, 2, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData61_APU_GAUSS_5X5[] = {{112, 2, 4, 0, {2, 2, 2, 2}}, {112, 2, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData62_APU_GAUSS_5X5[] = {{120, 2, 4, 0, {2, 2, 2, 2}}, {120, 2, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData63_APU_GAUSS_5X5[] = {{128, 2, 4, 0, {2, 2, 2, 2}}, {128, 2, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData64_APU_GAUSS_5X5[] = {{2, 3, 4, 0, {2, 2, 2, 2}}, {2, 3, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData65_APU_GAUSS_5X5[] = {{4, 3, 4, 0, {2, 2, 2, 2}}, {4, 3, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData66_APU_GAUSS_5X5[] = {{6, 3, 4, 0, {2, 2, 2, 2}}, {6, 3, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData67_APU_GAUSS_5X5[] = {{8, 3, 4, 0, {2, 2, 2, 2}}, {8, 3, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData68_APU_GAUSS_5X5[] = {{10, 3, 4, 0, {2, 2, 2, 2}}, {10, 3, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData69_APU_GAUSS_5X5[] = {{12, 3, 4, 0, {2, 2, 2, 2}}, {12, 3, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData70_APU_GAUSS_5X5[] = {{14, 3, 4, 0, {2, 2, 2, 2}}, {14, 3, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData71_APU_GAUSS_5X5[] = {{16, 3, 4, 0, {2, 2, 2, 2}}, {16, 3, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData72_APU_GAUSS_5X5[] = {{18, 3, 4, 0, {2, 2, 2, 2}}, {18, 3, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData73_APU_GAUSS_5X5[] = {{20, 3, 4, 0, {2, 2, 2, 2}}, {20, 3, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData74_APU_GAUSS_5X5[] = {{22, 3, 4, 0, {2, 2, 2, 2}}, {22, 3, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData75_APU_GAUSS_5X5[] = {{24, 3, 4, 0, {2, 2, 2, 2}}, {24, 3, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData76_APU_GAUSS_5X5[] = {{26, 3, 4, 0, {2, 2, 2, 2}}, {26, 3, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData77_APU_GAUSS_5X5[] = {{28, 3, 4, 0, {2, 2, 2, 2}}, {28, 3, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData78_APU_GAUSS_5X5[] = {{30, 3, 4, 0, {2, 2, 2, 2}}, {30, 3, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData79_APU_GAUSS_5X5[] = {{32, 3, 4, 0, {2, 2, 2, 2}}, {32, 3, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData80_APU_GAUSS_5X5[] = {{36, 3, 4, 0, {2, 2, 2, 2}}, {36, 3, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData81_APU_GAUSS_5X5[] = {{40, 3, 4, 0, {2, 2, 2, 2}}, {40, 3, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData82_APU_GAUSS_5X5[] = {{44, 3, 4, 0, {2, 2, 2, 2}}, {44, 3, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData83_APU_GAUSS_5X5[] = {{48, 3, 4, 0, {2, 2, 2, 2}}, {48, 3, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData84_APU_GAUSS_5X5[] = {{52, 3, 4, 0, {2, 2, 2, 2}}, {52, 3, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData85_APU_GAUSS_5X5[] = {{56, 3, 4, 0, {2, 2, 2, 2}}, {56, 3, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData86_APU_GAUSS_5X5[] = {{60, 3, 4, 0, {2, 2, 2, 2}}, {60, 3, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData87_APU_GAUSS_5X5[] = {{64, 3, 4, 0, {2, 2, 2, 2}}, {64, 3, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData88_APU_GAUSS_5X5[] = {{72, 3, 4, 0, {2, 2, 2, 2}}, {72, 3, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData89_APU_GAUSS_5X5[] = {{80, 3, 4, 0, {2, 2, 2, 2}}, {80, 3, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData90_APU_GAUSS_5X5[] = {{88, 3, 4, 0, {2, 2, 2, 2}}, {88, 3, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData91_APU_GAUSS_5X5[] = {{96, 3, 4, 0, {2, 2, 2, 2}}, {96, 3, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData92_APU_GAUSS_5X5[] = {{104, 3, 4, 0, {2, 2, 2, 2}}, {104, 3, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData93_APU_GAUSS_5X5[] = {{112, 3, 4, 0, {2, 2, 2, 2}}, {112, 3, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData94_APU_GAUSS_5X5[] = {{120, 3, 4, 0, {2, 2, 2, 2}}, {120, 3, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData95_APU_GAUSS_5X5[] = {{128, 3, 4, 0, {2, 2, 2, 2}}, {128, 3, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData96_APU_GAUSS_5X5[] = {{2, 4, 4, 0, {2, 2, 2, 2}}, {2, 4, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData97_APU_GAUSS_5X5[] = {{4, 4, 4, 0, {2, 2, 2, 2}}, {4, 4, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData98_APU_GAUSS_5X5[] = {{6, 4, 4, 0, {2, 2, 2, 2}}, {6, 4, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData99_APU_GAUSS_5X5[] = {{8, 4, 4, 0, {2, 2, 2, 2}}, {8, 4, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData100_APU_GAUSS_5X5[] = {{10, 4, 4, 0, {2, 2, 2, 2}}, {10, 4, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData101_APU_GAUSS_5X5[] = {{12, 4, 4, 0, {2, 2, 2, 2}}, {12, 4, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData102_APU_GAUSS_5X5[] = {{14, 4, 4, 0, {2, 2, 2, 2}}, {14, 4, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData103_APU_GAUSS_5X5[] = {{16, 4, 4, 0, {2, 2, 2, 2}}, {16, 4, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData104_APU_GAUSS_5X5[] = {{18, 4, 4, 0, {2, 2, 2, 2}}, {18, 4, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData105_APU_GAUSS_5X5[] = {{20, 4, 4, 0, {2, 2, 2, 2}}, {20, 4, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData106_APU_GAUSS_5X5[] = {{22, 4, 4, 0, {2, 2, 2, 2}}, {22, 4, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData107_APU_GAUSS_5X5[] = {{24, 4, 4, 0, {2, 2, 2, 2}}, {24, 4, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData108_APU_GAUSS_5X5[] = {{26, 4, 4, 0, {2, 2, 2, 2}}, {26, 4, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData109_APU_GAUSS_5X5[] = {{28, 4, 4, 0, {2, 2, 2, 2}}, {28, 4, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData110_APU_GAUSS_5X5[] = {{30, 4, 4, 0, {2, 2, 2, 2}}, {30, 4, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData111_APU_GAUSS_5X5[] = {{32, 4, 4, 0, {2, 2, 2, 2}}, {32, 4, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData112_APU_GAUSS_5X5[] = {{36, 4, 4, 0, {2, 2, 2, 2}}, {36, 4, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData113_APU_GAUSS_5X5[] = {{40, 4, 4, 0, {2, 2, 2, 2}}, {40, 4, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData114_APU_GAUSS_5X5[] = {{44, 4, 4, 0, {2, 2, 2, 2}}, {44, 4, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData115_APU_GAUSS_5X5[] = {{48, 4, 4, 0, {2, 2, 2, 2}}, {48, 4, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData116_APU_GAUSS_5X5[] = {{52, 4, 4, 0, {2, 2, 2, 2}}, {52, 4, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData117_APU_GAUSS_5X5[] = {{56, 4, 4, 0, {2, 2, 2, 2}}, {56, 4, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData118_APU_GAUSS_5X5[] = {{60, 4, 4, 0, {2, 2, 2, 2}}, {60, 4, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData119_APU_GAUSS_5X5[] = {{64, 4, 4, 0, {2, 2, 2, 2}}, {64, 4, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData120_APU_GAUSS_5X5[] = {{72, 4, 4, 0, {2, 2, 2, 2}}, {72, 4, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData121_APU_GAUSS_5X5[] = {{80, 4, 4, 0, {2, 2, 2, 2}}, {80, 4, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData122_APU_GAUSS_5X5[] = {{88, 4, 4, 0, {2, 2, 2, 2}}, {88, 4, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData123_APU_GAUSS_5X5[] = {{96, 4, 4, 0, {2, 2, 2, 2}}, {96, 4, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData124_APU_GAUSS_5X5[] = {{104, 4, 4, 0, {2, 2, 2, 2}}, {104, 4, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData125_APU_GAUSS_5X5[] = {{112, 4, 4, 0, {2, 2, 2, 2}}, {112, 4, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData126_APU_GAUSS_5X5[] = {{120, 4, 4, 0, {2, 2, 2, 2}}, {120, 4, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData127_APU_GAUSS_5X5[] = {{128, 4, 4, 0, {2, 2, 2, 2}}, {128, 4, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData128_APU_GAUSS_5X5[] = {{2, 5, 4, 0, {2, 2, 2, 2}}, {2, 5, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData129_APU_GAUSS_5X5[] = {{4, 5, 4, 0, {2, 2, 2, 2}}, {4, 5, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData130_APU_GAUSS_5X5[] = {{6, 5, 4, 0, {2, 2, 2, 2}}, {6, 5, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData131_APU_GAUSS_5X5[] = {{8, 5, 4, 0, {2, 2, 2, 2}}, {8, 5, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData132_APU_GAUSS_5X5[] = {{10, 5, 4, 0, {2, 2, 2, 2}}, {10, 5, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData133_APU_GAUSS_5X5[] = {{12, 5, 4, 0, {2, 2, 2, 2}}, {12, 5, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData134_APU_GAUSS_5X5[] = {{14, 5, 4, 0, {2, 2, 2, 2}}, {14, 5, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData135_APU_GAUSS_5X5[] = {{16, 5, 4, 0, {2, 2, 2, 2}}, {16, 5, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData136_APU_GAUSS_5X5[] = {{18, 5, 4, 0, {2, 2, 2, 2}}, {18, 5, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData137_APU_GAUSS_5X5[] = {{20, 5, 4, 0, {2, 2, 2, 2}}, {20, 5, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData138_APU_GAUSS_5X5[] = {{22, 5, 4, 0, {2, 2, 2, 2}}, {22, 5, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData139_APU_GAUSS_5X5[] = {{24, 5, 4, 0, {2, 2, 2, 2}}, {24, 5, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData140_APU_GAUSS_5X5[] = {{26, 5, 4, 0, {2, 2, 2, 2}}, {26, 5, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData141_APU_GAUSS_5X5[] = {{28, 5, 4, 0, {2, 2, 2, 2}}, {28, 5, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData142_APU_GAUSS_5X5[] = {{30, 5, 4, 0, {2, 2, 2, 2}}, {30, 5, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData143_APU_GAUSS_5X5[] = {{32, 5, 4, 0, {2, 2, 2, 2}}, {32, 5, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData144_APU_GAUSS_5X5[] = {{36, 5, 4, 0, {2, 2, 2, 2}}, {36, 5, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData145_APU_GAUSS_5X5[] = {{40, 5, 4, 0, {2, 2, 2, 2}}, {40, 5, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData146_APU_GAUSS_5X5[] = {{44, 5, 4, 0, {2, 2, 2, 2}}, {44, 5, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData147_APU_GAUSS_5X5[] = {{48, 5, 4, 0, {2, 2, 2, 2}}, {48, 5, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData148_APU_GAUSS_5X5[] = {{52, 5, 4, 0, {2, 2, 2, 2}}, {52, 5, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData149_APU_GAUSS_5X5[] = {{56, 5, 4, 0, {2, 2, 2, 2}}, {56, 5, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData150_APU_GAUSS_5X5[] = {{60, 5, 4, 0, {2, 2, 2, 2}}, {60, 5, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData151_APU_GAUSS_5X5[] = {{64, 5, 4, 0, {2, 2, 2, 2}}, {64, 5, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData152_APU_GAUSS_5X5[] = {{72, 5, 4, 0, {2, 2, 2, 2}}, {72, 5, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData153_APU_GAUSS_5X5[] = {{80, 5, 4, 0, {2, 2, 2, 2}}, {80, 5, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData154_APU_GAUSS_5X5[] = {{88, 5, 4, 0, {2, 2, 2, 2}}, {88, 5, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData155_APU_GAUSS_5X5[] = {{96, 5, 4, 0, {2, 2, 2, 2}}, {96, 5, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData156_APU_GAUSS_5X5[] = {{104, 5, 4, 0, {2, 2, 2, 2}}, {104, 5, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData157_APU_GAUSS_5X5[] = {{2, 6, 4, 0, {2, 2, 2, 2}}, {2, 6, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData158_APU_GAUSS_5X5[] = {{4, 6, 4, 0, {2, 2, 2, 2}}, {4, 6, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData159_APU_GAUSS_5X5[] = {{6, 6, 4, 0, {2, 2, 2, 2}}, {6, 6, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData160_APU_GAUSS_5X5[] = {{8, 6, 4, 0, {2, 2, 2, 2}}, {8, 6, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData161_APU_GAUSS_5X5[] = {{10, 6, 4, 0, {2, 2, 2, 2}}, {10, 6, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData162_APU_GAUSS_5X5[] = {{12, 6, 4, 0, {2, 2, 2, 2}}, {12, 6, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData163_APU_GAUSS_5X5[] = {{14, 6, 4, 0, {2, 2, 2, 2}}, {14, 6, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData164_APU_GAUSS_5X5[] = {{16, 6, 4, 0, {2, 2, 2, 2}}, {16, 6, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData165_APU_GAUSS_5X5[] = {{18, 6, 4, 0, {2, 2, 2, 2}}, {18, 6, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData166_APU_GAUSS_5X5[] = {{20, 6, 4, 0, {2, 2, 2, 2}}, {20, 6, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData167_APU_GAUSS_5X5[] = {{22, 6, 4, 0, {2, 2, 2, 2}}, {22, 6, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData168_APU_GAUSS_5X5[] = {{24, 6, 4, 0, {2, 2, 2, 2}}, {24, 6, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData169_APU_GAUSS_5X5[] = {{26, 6, 4, 0, {2, 2, 2, 2}}, {26, 6, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData170_APU_GAUSS_5X5[] = {{28, 6, 4, 0, {2, 2, 2, 2}}, {28, 6, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData171_APU_GAUSS_5X5[] = {{30, 6, 4, 0, {2, 2, 2, 2}}, {30, 6, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData172_APU_GAUSS_5X5[] = {{32, 6, 4, 0, {2, 2, 2, 2}}, {32, 6, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData173_APU_GAUSS_5X5[] = {{36, 6, 4, 0, {2, 2, 2, 2}}, {36, 6, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData174_APU_GAUSS_5X5[] = {{40, 6, 4, 0, {2, 2, 2, 2}}, {40, 6, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData175_APU_GAUSS_5X5[] = {{44, 6, 4, 0, {2, 2, 2, 2}}, {44, 6, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData176_APU_GAUSS_5X5[] = {{48, 6, 4, 0, {2, 2, 2, 2}}, {48, 6, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData177_APU_GAUSS_5X5[] = {{52, 6, 4, 0, {2, 2, 2, 2}}, {52, 6, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData178_APU_GAUSS_5X5[] = {{56, 6, 4, 0, {2, 2, 2, 2}}, {56, 6, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData179_APU_GAUSS_5X5[] = {{60, 6, 4, 0, {2, 2, 2, 2}}, {60, 6, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData180_APU_GAUSS_5X5[] = {{64, 6, 4, 0, {2, 2, 2, 2}}, {64, 6, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData181_APU_GAUSS_5X5[] = {{72, 6, 4, 0, {2, 2, 2, 2}}, {72, 6, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData182_APU_GAUSS_5X5[] = {{80, 6, 4, 0, {2, 2, 2, 2}}, {80, 6, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData183_APU_GAUSS_5X5[] = {{88, 6, 4, 0, {2, 2, 2, 2}}, {88, 6, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData184_APU_GAUSS_5X5[] = {{2, 8, 4, 0, {2, 2, 2, 2}}, {2, 8, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData185_APU_GAUSS_5X5[] = {{4, 8, 4, 0, {2, 2, 2, 2}}, {4, 8, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData186_APU_GAUSS_5X5[] = {{6, 8, 4, 0, {2, 2, 2, 2}}, {6, 8, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData187_APU_GAUSS_5X5[] = {{8, 8, 4, 0, {2, 2, 2, 2}}, {8, 8, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData188_APU_GAUSS_5X5[] = {{10, 8, 4, 0, {2, 2, 2, 2}}, {10, 8, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData189_APU_GAUSS_5X5[] = {{12, 8, 4, 0, {2, 2, 2, 2}}, {12, 8, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData190_APU_GAUSS_5X5[] = {{14, 8, 4, 0, {2, 2, 2, 2}}, {14, 8, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData191_APU_GAUSS_5X5[] = {{16, 8, 4, 0, {2, 2, 2, 2}}, {16, 8, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData192_APU_GAUSS_5X5[] = {{18, 8, 4, 0, {2, 2, 2, 2}}, {18, 8, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData193_APU_GAUSS_5X5[] = {{20, 8, 4, 0, {2, 2, 2, 2}}, {20, 8, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData194_APU_GAUSS_5X5[] = {{22, 8, 4, 0, {2, 2, 2, 2}}, {22, 8, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData195_APU_GAUSS_5X5[] = {{24, 8, 4, 0, {2, 2, 2, 2}}, {24, 8, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData196_APU_GAUSS_5X5[] = {{26, 8, 4, 0, {2, 2, 2, 2}}, {26, 8, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData197_APU_GAUSS_5X5[] = {{28, 8, 4, 0, {2, 2, 2, 2}}, {28, 8, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData198_APU_GAUSS_5X5[] = {{30, 8, 4, 0, {2, 2, 2, 2}}, {30, 8, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData199_APU_GAUSS_5X5[] = {{32, 8, 4, 0, {2, 2, 2, 2}}, {32, 8, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData200_APU_GAUSS_5X5[] = {{36, 8, 4, 0, {2, 2, 2, 2}}, {36, 8, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData201_APU_GAUSS_5X5[] = {{40, 8, 4, 0, {2, 2, 2, 2}}, {40, 8, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData202_APU_GAUSS_5X5[] = {{44, 8, 4, 0, {2, 2, 2, 2}}, {44, 8, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData203_APU_GAUSS_5X5[] = {{48, 8, 4, 0, {2, 2, 2, 2}}, {48, 8, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData204_APU_GAUSS_5X5[] = {{52, 8, 4, 0, {2, 2, 2, 2}}, {52, 8, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData205_APU_GAUSS_5X5[] = {{56, 8, 4, 0, {2, 2, 2, 2}}, {56, 8, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData206_APU_GAUSS_5X5[] = {{60, 8, 4, 0, {2, 2, 2, 2}}, {60, 8, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData207_APU_GAUSS_5X5[] = {{64, 8, 4, 0, {2, 2, 2, 2}}, {64, 8, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData208_APU_GAUSS_5X5[] = {{2, 10, 4, 0, {2, 2, 2, 2}}, {2, 10, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData209_APU_GAUSS_5X5[] = {{4, 10, 4, 0, {2, 2, 2, 2}}, {4, 10, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData210_APU_GAUSS_5X5[] = {{6, 10, 4, 0, {2, 2, 2, 2}}, {6, 10, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData211_APU_GAUSS_5X5[] = {{8, 10, 4, 0, {2, 2, 2, 2}}, {8, 10, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData212_APU_GAUSS_5X5[] = {{10, 10, 4, 0, {2, 2, 2, 2}}, {10, 10, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData213_APU_GAUSS_5X5[] = {{12, 10, 4, 0, {2, 2, 2, 2}}, {12, 10, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData214_APU_GAUSS_5X5[] = {{14, 10, 4, 0, {2, 2, 2, 2}}, {14, 10, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData215_APU_GAUSS_5X5[] = {{16, 10, 4, 0, {2, 2, 2, 2}}, {16, 10, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData216_APU_GAUSS_5X5[] = {{18, 10, 4, 0, {2, 2, 2, 2}}, {18, 10, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData217_APU_GAUSS_5X5[] = {{20, 10, 4, 0, {2, 2, 2, 2}}, {20, 10, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData218_APU_GAUSS_5X5[] = {{22, 10, 4, 0, {2, 2, 2, 2}}, {22, 10, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData219_APU_GAUSS_5X5[] = {{24, 10, 4, 0, {2, 2, 2, 2}}, {24, 10, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData220_APU_GAUSS_5X5[] = {{26, 10, 4, 0, {2, 2, 2, 2}}, {26, 10, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData221_APU_GAUSS_5X5[] = {{28, 10, 4, 0, {2, 2, 2, 2}}, {28, 10, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData222_APU_GAUSS_5X5[] = {{30, 10, 4, 0, {2, 2, 2, 2}}, {30, 10, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData223_APU_GAUSS_5X5[] = {{32, 10, 4, 0, {2, 2, 2, 2}}, {32, 10, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData224_APU_GAUSS_5X5[] = {{36, 10, 4, 0, {2, 2, 2, 2}}, {36, 10, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData225_APU_GAUSS_5X5[] = {{40, 10, 4, 0, {2, 2, 2, 2}}, {40, 10, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData226_APU_GAUSS_5X5[] = {{44, 10, 4, 0, {2, 2, 2, 2}}, {44, 10, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData227_APU_GAUSS_5X5[] = {{48, 10, 4, 0, {2, 2, 2, 2}}, {48, 10, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData228_APU_GAUSS_5X5[] = {{52, 10, 4, 0, {2, 2, 2, 2}}, {52, 10, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData229_APU_GAUSS_5X5[] = {{56, 10, 4, 0, {2, 2, 2, 2}}, {56, 10, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData230_APU_GAUSS_5X5[] = {{2, 12, 4, 0, {2, 2, 2, 2}}, {2, 12, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData231_APU_GAUSS_5X5[] = {{4, 12, 4, 0, {2, 2, 2, 2}}, {4, 12, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData232_APU_GAUSS_5X5[] = {{6, 12, 4, 0, {2, 2, 2, 2}}, {6, 12, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData233_APU_GAUSS_5X5[] = {{8, 12, 4, 0, {2, 2, 2, 2}}, {8, 12, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData234_APU_GAUSS_5X5[] = {{10, 12, 4, 0, {2, 2, 2, 2}}, {10, 12, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData235_APU_GAUSS_5X5[] = {{12, 12, 4, 0, {2, 2, 2, 2}}, {12, 12, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData236_APU_GAUSS_5X5[] = {{14, 12, 4, 0, {2, 2, 2, 2}}, {14, 12, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData237_APU_GAUSS_5X5[] = {{16, 12, 4, 0, {2, 2, 2, 2}}, {16, 12, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData238_APU_GAUSS_5X5[] = {{18, 12, 4, 0, {2, 2, 2, 2}}, {18, 12, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData239_APU_GAUSS_5X5[] = {{20, 12, 4, 0, {2, 2, 2, 2}}, {20, 12, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData240_APU_GAUSS_5X5[] = {{22, 12, 4, 0, {2, 2, 2, 2}}, {22, 12, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData241_APU_GAUSS_5X5[] = {{24, 12, 4, 0, {2, 2, 2, 2}}, {24, 12, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData242_APU_GAUSS_5X5[] = {{26, 12, 4, 0, {2, 2, 2, 2}}, {26, 12, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData243_APU_GAUSS_5X5[] = {{28, 12, 4, 0, {2, 2, 2, 2}}, {28, 12, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData244_APU_GAUSS_5X5[] = {{30, 12, 4, 0, {2, 2, 2, 2}}, {30, 12, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData245_APU_GAUSS_5X5[] = {{32, 12, 4, 0, {2, 2, 2, 2}}, {32, 12, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData246_APU_GAUSS_5X5[] = {{36, 12, 4, 0, {2, 2, 2, 2}}, {36, 12, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData247_APU_GAUSS_5X5[] = {{40, 12, 4, 0, {2, 2, 2, 2}}, {40, 12, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData248_APU_GAUSS_5X5[] = {{44, 12, 4, 0, {2, 2, 2, 2}}, {44, 12, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData249_APU_GAUSS_5X5[] = {{2, 14, 4, 0, {2, 2, 2, 2}}, {2, 14, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData250_APU_GAUSS_5X5[] = {{4, 14, 4, 0, {2, 2, 2, 2}}, {4, 14, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData251_APU_GAUSS_5X5[] = {{6, 14, 4, 0, {2, 2, 2, 2}}, {6, 14, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData252_APU_GAUSS_5X5[] = {{8, 14, 4, 0, {2, 2, 2, 2}}, {8, 14, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData253_APU_GAUSS_5X5[] = {{10, 14, 4, 0, {2, 2, 2, 2}}, {10, 14, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData254_APU_GAUSS_5X5[] = {{12, 14, 4, 0, {2, 2, 2, 2}}, {12, 14, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData255_APU_GAUSS_5X5[] = {{14, 14, 4, 0, {2, 2, 2, 2}}, {14, 14, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData256_APU_GAUSS_5X5[] = {{16, 14, 4, 0, {2, 2, 2, 2}}, {16, 14, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData257_APU_GAUSS_5X5[] = {{18, 14, 4, 0, {2, 2, 2, 2}}, {18, 14, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData258_APU_GAUSS_5X5[] = {{20, 14, 4, 0, {2, 2, 2, 2}}, {20, 14, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData259_APU_GAUSS_5X5[] = {{22, 14, 4, 0, {2, 2, 2, 2}}, {22, 14, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData260_APU_GAUSS_5X5[] = {{24, 14, 4, 0, {2, 2, 2, 2}}, {24, 14, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData261_APU_GAUSS_5X5[] = {{26, 14, 4, 0, {2, 2, 2, 2}}, {26, 14, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData262_APU_GAUSS_5X5[] = {{28, 14, 4, 0, {2, 2, 2, 2}}, {28, 14, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData263_APU_GAUSS_5X5[] = {{30, 14, 4, 0, {2, 2, 2, 2}}, {30, 14, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData264_APU_GAUSS_5X5[] = {{32, 14, 4, 0, {2, 2, 2, 2}}, {32, 14, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData265_APU_GAUSS_5X5[] = {{36, 14, 4, 0, {2, 2, 2, 2}}, {36, 14, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData266_APU_GAUSS_5X5[] = {{40, 14, 4, 0, {2, 2, 2, 2}}, {40, 14, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData267_APU_GAUSS_5X5[] = {{2, 16, 4, 0, {2, 2, 2, 2}}, {2, 16, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData268_APU_GAUSS_5X5[] = {{4, 16, 4, 0, {2, 2, 2, 2}}, {4, 16, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData269_APU_GAUSS_5X5[] = {{6, 16, 4, 0, {2, 2, 2, 2}}, {6, 16, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData270_APU_GAUSS_5X5[] = {{8, 16, 4, 0, {2, 2, 2, 2}}, {8, 16, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData271_APU_GAUSS_5X5[] = {{10, 16, 4, 0, {2, 2, 2, 2}}, {10, 16, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData272_APU_GAUSS_5X5[] = {{12, 16, 4, 0, {2, 2, 2, 2}}, {12, 16, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData273_APU_GAUSS_5X5[] = {{14, 16, 4, 0, {2, 2, 2, 2}}, {14, 16, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData274_APU_GAUSS_5X5[] = {{16, 16, 4, 0, {2, 2, 2, 2}}, {16, 16, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData275_APU_GAUSS_5X5[] = {{18, 16, 4, 0, {2, 2, 2, 2}}, {18, 16, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData276_APU_GAUSS_5X5[] = {{20, 16, 4, 0, {2, 2, 2, 2}}, {20, 16, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData277_APU_GAUSS_5X5[] = {{22, 16, 4, 0, {2, 2, 2, 2}}, {22, 16, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData278_APU_GAUSS_5X5[] = {{24, 16, 4, 0, {2, 2, 2, 2}}, {24, 16, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData279_APU_GAUSS_5X5[] = {{26, 16, 4, 0, {2, 2, 2, 2}}, {26, 16, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData280_APU_GAUSS_5X5[] = {{28, 16, 4, 0, {2, 2, 2, 2}}, {28, 16, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData281_APU_GAUSS_5X5[] = {{30, 16, 4, 0, {2, 2, 2, 2}}, {30, 16, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData282_APU_GAUSS_5X5[] = {{32, 16, 4, 0, {2, 2, 2, 2}}, {32, 16, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData283_APU_GAUSS_5X5[] = {{2, 18, 4, 0, {2, 2, 2, 2}}, {2, 18, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData284_APU_GAUSS_5X5[] = {{4, 18, 4, 0, {2, 2, 2, 2}}, {4, 18, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData285_APU_GAUSS_5X5[] = {{6, 18, 4, 0, {2, 2, 2, 2}}, {6, 18, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData286_APU_GAUSS_5X5[] = {{8, 18, 4, 0, {2, 2, 2, 2}}, {8, 18, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData287_APU_GAUSS_5X5[] = {{10, 18, 4, 0, {2, 2, 2, 2}}, {10, 18, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData288_APU_GAUSS_5X5[] = {{12, 18, 4, 0, {2, 2, 2, 2}}, {12, 18, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData289_APU_GAUSS_5X5[] = {{14, 18, 4, 0, {2, 2, 2, 2}}, {14, 18, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData290_APU_GAUSS_5X5[] = {{16, 18, 4, 0, {2, 2, 2, 2}}, {16, 18, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData291_APU_GAUSS_5X5[] = {{18, 18, 4, 0, {2, 2, 2, 2}}, {18, 18, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData292_APU_GAUSS_5X5[] = {{20, 18, 4, 0, {2, 2, 2, 2}}, {20, 18, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData293_APU_GAUSS_5X5[] = {{22, 18, 4, 0, {2, 2, 2, 2}}, {22, 18, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData294_APU_GAUSS_5X5[] = {{24, 18, 4, 0, {2, 2, 2, 2}}, {24, 18, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData295_APU_GAUSS_5X5[] = {{26, 18, 4, 0, {2, 2, 2, 2}}, {26, 18, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData296_APU_GAUSS_5X5[] = {{28, 18, 4, 0, {2, 2, 2, 2}}, {28, 18, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData297_APU_GAUSS_5X5[] = {{30, 18, 4, 0, {2, 2, 2, 2}}, {30, 18, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData298_APU_GAUSS_5X5[] = {{2, 20, 4, 0, {2, 2, 2, 2}}, {2, 20, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData299_APU_GAUSS_5X5[] = {{4, 20, 4, 0, {2, 2, 2, 2}}, {4, 20, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData300_APU_GAUSS_5X5[] = {{6, 20, 4, 0, {2, 2, 2, 2}}, {6, 20, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData301_APU_GAUSS_5X5[] = {{8, 20, 4, 0, {2, 2, 2, 2}}, {8, 20, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData302_APU_GAUSS_5X5[] = {{10, 20, 4, 0, {2, 2, 2, 2}}, {10, 20, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData303_APU_GAUSS_5X5[] = {{12, 20, 4, 0, {2, 2, 2, 2}}, {12, 20, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData304_APU_GAUSS_5X5[] = {{14, 20, 4, 0, {2, 2, 2, 2}}, {14, 20, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData305_APU_GAUSS_5X5[] = {{16, 20, 4, 0, {2, 2, 2, 2}}, {16, 20, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData306_APU_GAUSS_5X5[] = {{18, 20, 4, 0, {2, 2, 2, 2}}, {18, 20, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData307_APU_GAUSS_5X5[] = {{20, 20, 4, 0, {2, 2, 2, 2}}, {20, 20, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData308_APU_GAUSS_5X5[] = {{22, 20, 4, 0, {2, 2, 2, 2}}, {22, 20, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData309_APU_GAUSS_5X5[] = {{24, 20, 4, 0, {2, 2, 2, 2}}, {24, 20, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData310_APU_GAUSS_5X5[] = {{26, 20, 4, 0, {2, 2, 2, 2}}, {26, 20, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData311_APU_GAUSS_5X5[] = {{28, 20, 4, 0, {2, 2, 2, 2}}, {28, 20, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData312_APU_GAUSS_5X5[] = {{2, 22, 4, 0, {2, 2, 2, 2}}, {2, 22, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData313_APU_GAUSS_5X5[] = {{4, 22, 4, 0, {2, 2, 2, 2}}, {4, 22, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData314_APU_GAUSS_5X5[] = {{6, 22, 4, 0, {2, 2, 2, 2}}, {6, 22, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData315_APU_GAUSS_5X5[] = {{8, 22, 4, 0, {2, 2, 2, 2}}, {8, 22, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData316_APU_GAUSS_5X5[] = {{10, 22, 4, 0, {2, 2, 2, 2}}, {10, 22, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData317_APU_GAUSS_5X5[] = {{12, 22, 4, 0, {2, 2, 2, 2}}, {12, 22, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData318_APU_GAUSS_5X5[] = {{14, 22, 4, 0, {2, 2, 2, 2}}, {14, 22, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData319_APU_GAUSS_5X5[] = {{16, 22, 4, 0, {2, 2, 2, 2}}, {16, 22, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData320_APU_GAUSS_5X5[] = {{18, 22, 4, 0, {2, 2, 2, 2}}, {18, 22, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData321_APU_GAUSS_5X5[] = {{20, 22, 4, 0, {2, 2, 2, 2}}, {20, 22, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData322_APU_GAUSS_5X5[] = {{22, 22, 4, 0, {2, 2, 2, 2}}, {22, 22, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData323_APU_GAUSS_5X5[] = {{24, 22, 4, 0, {2, 2, 2, 2}}, {24, 22, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData324_APU_GAUSS_5X5[] = {{2, 24, 4, 0, {2, 2, 2, 2}}, {2, 24, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData325_APU_GAUSS_5X5[] = {{4, 24, 4, 0, {2, 2, 2, 2}}, {4, 24, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData326_APU_GAUSS_5X5[] = {{6, 24, 4, 0, {2, 2, 2, 2}}, {6, 24, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData327_APU_GAUSS_5X5[] = {{8, 24, 4, 0, {2, 2, 2, 2}}, {8, 24, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData328_APU_GAUSS_5X5[] = {{10, 24, 4, 0, {2, 2, 2, 2}}, {10, 24, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData329_APU_GAUSS_5X5[] = {{12, 24, 4, 0, {2, 2, 2, 2}}, {12, 24, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData330_APU_GAUSS_5X5[] = {{14, 24, 4, 0, {2, 2, 2, 2}}, {14, 24, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData331_APU_GAUSS_5X5[] = {{16, 24, 4, 0, {2, 2, 2, 2}}, {16, 24, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData332_APU_GAUSS_5X5[] = {{18, 24, 4, 0, {2, 2, 2, 2}}, {18, 24, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData333_APU_GAUSS_5X5[] = {{20, 24, 4, 0, {2, 2, 2, 2}}, {20, 24, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData334_APU_GAUSS_5X5[] = {{22, 24, 4, 0, {2, 2, 2, 2}}, {22, 24, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData335_APU_GAUSS_5X5[] = {{2, 26, 4, 0, {2, 2, 2, 2}}, {2, 26, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData336_APU_GAUSS_5X5[] = {{4, 26, 4, 0, {2, 2, 2, 2}}, {4, 26, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData337_APU_GAUSS_5X5[] = {{6, 26, 4, 0, {2, 2, 2, 2}}, {6, 26, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData338_APU_GAUSS_5X5[] = {{8, 26, 4, 0, {2, 2, 2, 2}}, {8, 26, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData339_APU_GAUSS_5X5[] = {{10, 26, 4, 0, {2, 2, 2, 2}}, {10, 26, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData340_APU_GAUSS_5X5[] = {{12, 26, 4, 0, {2, 2, 2, 2}}, {12, 26, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData341_APU_GAUSS_5X5[] = {{14, 26, 4, 0, {2, 2, 2, 2}}, {14, 26, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData342_APU_GAUSS_5X5[] = {{16, 26, 4, 0, {2, 2, 2, 2}}, {16, 26, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData343_APU_GAUSS_5X5[] = {{18, 26, 4, 0, {2, 2, 2, 2}}, {18, 26, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData344_APU_GAUSS_5X5[] = {{20, 26, 4, 0, {2, 2, 2, 2}}, {20, 26, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData345_APU_GAUSS_5X5[] = {{2, 28, 4, 0, {2, 2, 2, 2}}, {2, 28, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData346_APU_GAUSS_5X5[] = {{4, 28, 4, 0, {2, 2, 2, 2}}, {4, 28, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData347_APU_GAUSS_5X5[] = {{6, 28, 4, 0, {2, 2, 2, 2}}, {6, 28, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData348_APU_GAUSS_5X5[] = {{8, 28, 4, 0, {2, 2, 2, 2}}, {8, 28, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData349_APU_GAUSS_5X5[] = {{10, 28, 4, 0, {2, 2, 2, 2}}, {10, 28, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData350_APU_GAUSS_5X5[] = {{12, 28, 4, 0, {2, 2, 2, 2}}, {12, 28, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData351_APU_GAUSS_5X5[] = {{14, 28, 4, 0, {2, 2, 2, 2}}, {14, 28, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData352_APU_GAUSS_5X5[] = {{16, 28, 4, 0, {2, 2, 2, 2}}, {16, 28, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData353_APU_GAUSS_5X5[] = {{18, 28, 4, 0, {2, 2, 2, 2}}, {18, 28, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData354_APU_GAUSS_5X5[] = {{2, 30, 4, 0, {2, 2, 2, 2}}, {2, 30, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData355_APU_GAUSS_5X5[] = {{4, 30, 4, 0, {2, 2, 2, 2}}, {4, 30, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData356_APU_GAUSS_5X5[] = {{6, 30, 4, 0, {2, 2, 2, 2}}, {6, 30, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData357_APU_GAUSS_5X5[] = {{8, 30, 4, 0, {2, 2, 2, 2}}, {8, 30, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData358_APU_GAUSS_5X5[] = {{10, 30, 4, 0, {2, 2, 2, 2}}, {10, 30, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData359_APU_GAUSS_5X5[] = {{12, 30, 4, 0, {2, 2, 2, 2}}, {12, 30, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData360_APU_GAUSS_5X5[] = {{14, 30, 4, 0, {2, 2, 2, 2}}, {14, 30, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData361_APU_GAUSS_5X5[] = {{16, 30, 4, 0, {2, 2, 2, 2}}, {16, 30, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData362_APU_GAUSS_5X5[] = {{18, 30, 4, 0, {2, 2, 2, 2}}, {18, 30, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData363_APU_GAUSS_5X5[] = {{2, 32, 4, 0, {2, 2, 2, 2}}, {2, 32, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData364_APU_GAUSS_5X5[] = {{4, 32, 4, 0, {2, 2, 2, 2}}, {4, 32, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData365_APU_GAUSS_5X5[] = {{6, 32, 4, 0, {2, 2, 2, 2}}, {6, 32, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData366_APU_GAUSS_5X5[] = {{8, 32, 4, 0, {2, 2, 2, 2}}, {8, 32, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData367_APU_GAUSS_5X5[] = {{10, 32, 4, 0, {2, 2, 2, 2}}, {10, 32, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData368_APU_GAUSS_5X5[] = {{12, 32, 4, 0, {2, 2, 2, 2}}, {12, 32, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData369_APU_GAUSS_5X5[] = {{14, 32, 4, 0, {2, 2, 2, 2}}, {14, 32, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData370_APU_GAUSS_5X5[] = {{16, 32, 4, 0, {2, 2, 2, 2}}, {16, 32, 2, 1, {0, 0, 0, 0}}};

static acf_scenario_kernel_data gScenarioKernelData0_APU_GAUSS_5X5[] = {{2, 0}};
static acf_scenario_kernel_data gScenarioKernelData1_APU_GAUSS_5X5[] = {{2, 0}};
static acf_scenario_kernel_data gScenarioKernelData2_APU_GAUSS_5X5[] = {{2, 0}};
static acf_scenario_kernel_data gScenarioKernelData3_APU_GAUSS_5X5[] = {{2, 0}};
static acf_scenario_kernel_data gScenarioKernelData4_APU_GAUSS_5X5[] = {{2, 0}};
static acf_scenario_kernel_data gScenarioKernelData5_APU_GAUSS_5X5[] = {{2, 0}};
static acf_scenario_kernel_data gScenarioKernelData6_APU_GAUSS_5X5[] = {{2, 0}};
static acf_scenario_kernel_data gScenarioKernelData7_APU_GAUSS_5X5[] = {{2, 0}};
static acf_scenario_kernel_data gScenarioKernelData8_APU_GAUSS_5X5[] = {{2, 0}};
static acf_scenario_kernel_data gScenarioKernelData9_APU_GAUSS_5X5[] = {{2, 0}};
static acf_scenario_kernel_data gScenarioKernelData10_APU_GAUSS_5X5[] = {{2, 0}};
static acf_scenario_kernel_data gScenarioKernelData11_APU_GAUSS_5X5[] = {{2, 0}};
static acf_scenario_kernel_data gScenarioKernelData12_APU_GAUSS_5X5[] = {{2, 0}};
static acf_scenario_kernel_data gScenarioKernelData13_APU_GAUSS_5X5[] = {{2, 0}};
static acf_scenario_kernel_data gScenarioKernelData14_APU_GAUSS_5X5[] = {{2, 0}};
static acf_scenario_kernel_data gScenarioKernelData15_APU_GAUSS_5X5[] = {{2, 0}};
static acf_scenario_kernel_data gScenarioKernelData16_APU_GAUSS_5X5[] = {{2, 0}};
static acf_scenario_kernel_data gScenarioKernelData17_APU_GAUSS_5X5[] = {{2, 0}};
static acf_scenario_kernel_data gScenarioKernelData18_APU_GAUSS_5X5[] = {{2, 0}};
static acf_scenario_kernel_data gScenarioKernelData19_APU_GAUSS_5X5[] = {{2, 0}};
static acf_scenario_kernel_data gScenarioKernelData20_APU_GAUSS_5X5[] = {{2, 0}};
static acf_scenario_kernel_data gScenarioKernelData21_APU_GAUSS_5X5[] = {{2, 0}};
static acf_scenario_kernel_data gScenarioKernelData22_APU_GAUSS_5X5[] = {{2, 0}};
static acf_scenario_kernel_data gScenarioKernelData23_APU_GAUSS_5X5[] = {{2, 0}};
static acf_scenario_kernel_data gScenarioKernelData24_APU_GAUSS_5X5[] = {{2, 0}};
static acf_scenario_kernel_data gScenarioKernelData25_APU_GAUSS_5X5[] = {{2, 0}};
static acf_scenario_kernel_data gScenarioKernelData26_APU_GAUSS_5X5[] = {{2, 0}};
static acf_scenario_kernel_data gScenarioKernelData27_APU_GAUSS_5X5[] = {{2, 0}};
static acf_scenario_kernel_data gScenarioKernelData28_APU_GAUSS_5X5[] = {{2, 0}};
static acf_scenario_kernel_data gScenarioKernelData29_APU_GAUSS_5X5[] = {{2, 0}};
static acf_scenario_kernel_data gScenarioKernelData30_APU_GAUSS_5X5[] = {{2, 0}};
static acf_scenario_kernel_data gScenarioKernelData31_APU_GAUSS_5X5[] = {{2, 0}};
static acf_scenario_kernel_data gScenarioKernelData32_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData33_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData34_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData35_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData36_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData37_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData38_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData39_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData40_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData41_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData42_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData43_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData44_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData45_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData46_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData47_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData48_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData49_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData50_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData51_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData52_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData53_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData54_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData55_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData56_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData57_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData58_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData59_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData60_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData61_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData62_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData63_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData64_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData65_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData66_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData67_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData68_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData69_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData70_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData71_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData72_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData73_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData74_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData75_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData76_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData77_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData78_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData79_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData80_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData81_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData82_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData83_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData84_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData85_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData86_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData87_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData88_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData89_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData90_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData91_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData92_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData93_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData94_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData95_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData96_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData97_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData98_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData99_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData100_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData101_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData102_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData103_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData104_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData105_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData106_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData107_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData108_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData109_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData110_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData111_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData112_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData113_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData114_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData115_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData116_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData117_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData118_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData119_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData120_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData121_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData122_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData123_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData124_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData125_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData126_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData127_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData128_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData129_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData130_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData131_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData132_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData133_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData134_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData135_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData136_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData137_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData138_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData139_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData140_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData141_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData142_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData143_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData144_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData145_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData146_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData147_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData148_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData149_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData150_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData151_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData152_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData153_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData154_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData155_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData156_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData157_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData158_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData159_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData160_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData161_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData162_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData163_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData164_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData165_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData166_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData167_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData168_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData169_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData170_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData171_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData172_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData173_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData174_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData175_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData176_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData177_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData178_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData179_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData180_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData181_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData182_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData183_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData184_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData185_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData186_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData187_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData188_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData189_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData190_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData191_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData192_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData193_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData194_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData195_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData196_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData197_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData198_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData199_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData200_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData201_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData202_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData203_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData204_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData205_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData206_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData207_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData208_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData209_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData210_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData211_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData212_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData213_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData214_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData215_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData216_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData217_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData218_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData219_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData220_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData221_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData222_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData223_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData224_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData225_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData226_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData227_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData228_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData229_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData230_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData231_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData232_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData233_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData234_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData235_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData236_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData237_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData238_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData239_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData240_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData241_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData242_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData243_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData244_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData245_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData246_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData247_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData248_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData249_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData250_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData251_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData252_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData253_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData254_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData255_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData256_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData257_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData258_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData259_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData260_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData261_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData262_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData263_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData264_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData265_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData266_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData267_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData268_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData269_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData270_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData271_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData272_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData273_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData274_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData275_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData276_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData277_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData278_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData279_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData280_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData281_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData282_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData283_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData284_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData285_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData286_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData287_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData288_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData289_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData290_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData291_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData292_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData293_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData294_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData295_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData296_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData297_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData298_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData299_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData300_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData301_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData302_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData303_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData304_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData305_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData306_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData307_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData308_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData309_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData310_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData311_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData312_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData313_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData314_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData315_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData316_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData317_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData318_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData319_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData320_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData321_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData322_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData323_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData324_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData325_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData326_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData327_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData328_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData329_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData330_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData331_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData332_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData333_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData334_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData335_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData336_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData337_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData338_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData339_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData340_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData341_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData342_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData343_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData344_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData345_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData346_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData347_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData348_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData349_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData350_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData351_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData352_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData353_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData354_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData355_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData356_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData357_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData358_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData359_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData360_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData361_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData362_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData363_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData364_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData365_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData366_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData367_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData368_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData369_APU_GAUSS_5X5[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData370_APU_GAUSS_5X5[] = {{1, 0}};

static acf_scenario gScenarioArray_APU_GAUSS_5X5[] = {
{2, 1, 72, 0, 2, gScenarioBufferData0_APU_GAUSS_5X5, 32, gScenarioKernelData0_APU_GAUSS_5X5, 4},
{4, 1, 96, 0, 2, gScenarioBufferData1_APU_GAUSS_5X5, 32, gScenarioKernelData1_APU_GAUSS_5X5, 4},
{6, 1, 124, 0, 2, gScenarioBufferData2_APU_GAUSS_5X5, 32, gScenarioKernelData2_APU_GAUSS_5X5, 4},
{8, 1, 148, 0, 2, gScenarioBufferData3_APU_GAUSS_5X5, 32, gScenarioKernelData3_APU_GAUSS_5X5, 4},
{10, 1, 176, 0, 2, gScenarioBufferData4_APU_GAUSS_5X5, 32, gScenarioKernelData4_APU_GAUSS_5X5, 4},
{12, 1, 200, 0, 2, gScenarioBufferData5_APU_GAUSS_5X5, 32, gScenarioKernelData5_APU_GAUSS_5X5, 4},
{14, 1, 228, 0, 2, gScenarioBufferData6_APU_GAUSS_5X5, 32, gScenarioKernelData6_APU_GAUSS_5X5, 4},
{16, 1, 252, 0, 2, gScenarioBufferData7_APU_GAUSS_5X5, 32, gScenarioKernelData7_APU_GAUSS_5X5, 4},
{18, 1, 280, 0, 2, gScenarioBufferData8_APU_GAUSS_5X5, 32, gScenarioKernelData8_APU_GAUSS_5X5, 4},
{20, 1, 304, 0, 2, gScenarioBufferData9_APU_GAUSS_5X5, 32, gScenarioKernelData9_APU_GAUSS_5X5, 4},
{22, 1, 332, 0, 2, gScenarioBufferData10_APU_GAUSS_5X5, 32, gScenarioKernelData10_APU_GAUSS_5X5, 4},
{24, 1, 356, 0, 2, gScenarioBufferData11_APU_GAUSS_5X5, 32, gScenarioKernelData11_APU_GAUSS_5X5, 4},
{26, 1, 384, 0, 2, gScenarioBufferData12_APU_GAUSS_5X5, 32, gScenarioKernelData12_APU_GAUSS_5X5, 4},
{28, 1, 408, 0, 2, gScenarioBufferData13_APU_GAUSS_5X5, 32, gScenarioKernelData13_APU_GAUSS_5X5, 4},
{30, 1, 436, 0, 2, gScenarioBufferData14_APU_GAUSS_5X5, 32, gScenarioKernelData14_APU_GAUSS_5X5, 4},
{32, 1, 460, 0, 2, gScenarioBufferData15_APU_GAUSS_5X5, 32, gScenarioKernelData15_APU_GAUSS_5X5, 4},
{36, 1, 512, 0, 2, gScenarioBufferData16_APU_GAUSS_5X5, 32, gScenarioKernelData16_APU_GAUSS_5X5, 4},
{40, 1, 564, 0, 2, gScenarioBufferData17_APU_GAUSS_5X5, 32, gScenarioKernelData17_APU_GAUSS_5X5, 4},
{44, 1, 616, 0, 2, gScenarioBufferData18_APU_GAUSS_5X5, 32, gScenarioKernelData18_APU_GAUSS_5X5, 4},
{48, 1, 668, 0, 2, gScenarioBufferData19_APU_GAUSS_5X5, 32, gScenarioKernelData19_APU_GAUSS_5X5, 4},
{52, 1, 720, 0, 2, gScenarioBufferData20_APU_GAUSS_5X5, 32, gScenarioKernelData20_APU_GAUSS_5X5, 4},
{56, 1, 772, 0, 2, gScenarioBufferData21_APU_GAUSS_5X5, 32, gScenarioKernelData21_APU_GAUSS_5X5, 4},
{60, 1, 824, 0, 2, gScenarioBufferData22_APU_GAUSS_5X5, 32, gScenarioKernelData22_APU_GAUSS_5X5, 4},
{64, 1, 876, 0, 2, gScenarioBufferData23_APU_GAUSS_5X5, 32, gScenarioKernelData23_APU_GAUSS_5X5, 4},
{72, 1, 980, 0, 2, gScenarioBufferData24_APU_GAUSS_5X5, 32, gScenarioKernelData24_APU_GAUSS_5X5, 4},
{80, 1, 1084, 0, 2, gScenarioBufferData25_APU_GAUSS_5X5, 32, gScenarioKernelData25_APU_GAUSS_5X5, 4},
{88, 1, 1188, 0, 2, gScenarioBufferData26_APU_GAUSS_5X5, 32, gScenarioKernelData26_APU_GAUSS_5X5, 4},
{96, 1, 1292, 0, 2, gScenarioBufferData27_APU_GAUSS_5X5, 32, gScenarioKernelData27_APU_GAUSS_5X5, 4},
{104, 1, 1396, 0, 2, gScenarioBufferData28_APU_GAUSS_5X5, 32, gScenarioKernelData28_APU_GAUSS_5X5, 4},
{112, 1, 1500, 0, 2, gScenarioBufferData29_APU_GAUSS_5X5, 32, gScenarioKernelData29_APU_GAUSS_5X5, 4},
{120, 1, 1604, 0, 2, gScenarioBufferData30_APU_GAUSS_5X5, 32, gScenarioKernelData30_APU_GAUSS_5X5, 4},
{128, 1, 1708, 0, 2, gScenarioBufferData31_APU_GAUSS_5X5, 32, gScenarioKernelData31_APU_GAUSS_5X5, 4},
{2, 2, 80, 0, 1, gScenarioBufferData32_APU_GAUSS_5X5, 32, gScenarioKernelData32_APU_GAUSS_5X5, 4},
{4, 2, 112, 0, 1, gScenarioBufferData33_APU_GAUSS_5X5, 32, gScenarioKernelData33_APU_GAUSS_5X5, 4},
{6, 2, 144, 0, 1, gScenarioBufferData34_APU_GAUSS_5X5, 32, gScenarioKernelData34_APU_GAUSS_5X5, 4},
{8, 2, 176, 0, 1, gScenarioBufferData35_APU_GAUSS_5X5, 32, gScenarioKernelData35_APU_GAUSS_5X5, 4},
{10, 2, 208, 0, 1, gScenarioBufferData36_APU_GAUSS_5X5, 32, gScenarioKernelData36_APU_GAUSS_5X5, 4},
{12, 2, 240, 0, 1, gScenarioBufferData37_APU_GAUSS_5X5, 32, gScenarioKernelData37_APU_GAUSS_5X5, 4},
{14, 2, 272, 0, 1, gScenarioBufferData38_APU_GAUSS_5X5, 32, gScenarioKernelData38_APU_GAUSS_5X5, 4},
{16, 2, 304, 0, 1, gScenarioBufferData39_APU_GAUSS_5X5, 32, gScenarioKernelData39_APU_GAUSS_5X5, 4},
{18, 2, 336, 0, 1, gScenarioBufferData40_APU_GAUSS_5X5, 32, gScenarioKernelData40_APU_GAUSS_5X5, 4},
{20, 2, 368, 0, 1, gScenarioBufferData41_APU_GAUSS_5X5, 32, gScenarioKernelData41_APU_GAUSS_5X5, 4},
{22, 2, 400, 0, 1, gScenarioBufferData42_APU_GAUSS_5X5, 32, gScenarioKernelData42_APU_GAUSS_5X5, 4},
{24, 2, 432, 0, 1, gScenarioBufferData43_APU_GAUSS_5X5, 32, gScenarioKernelData43_APU_GAUSS_5X5, 4},
{26, 2, 464, 0, 1, gScenarioBufferData44_APU_GAUSS_5X5, 32, gScenarioKernelData44_APU_GAUSS_5X5, 4},
{28, 2, 496, 0, 1, gScenarioBufferData45_APU_GAUSS_5X5, 32, gScenarioKernelData45_APU_GAUSS_5X5, 4},
{30, 2, 528, 0, 1, gScenarioBufferData46_APU_GAUSS_5X5, 32, gScenarioKernelData46_APU_GAUSS_5X5, 4},
{32, 2, 560, 0, 1, gScenarioBufferData47_APU_GAUSS_5X5, 32, gScenarioKernelData47_APU_GAUSS_5X5, 4},
{36, 2, 624, 0, 1, gScenarioBufferData48_APU_GAUSS_5X5, 32, gScenarioKernelData48_APU_GAUSS_5X5, 4},
{40, 2, 688, 0, 1, gScenarioBufferData49_APU_GAUSS_5X5, 32, gScenarioKernelData49_APU_GAUSS_5X5, 4},
{44, 2, 752, 0, 1, gScenarioBufferData50_APU_GAUSS_5X5, 32, gScenarioKernelData50_APU_GAUSS_5X5, 4},
{48, 2, 816, 0, 1, gScenarioBufferData51_APU_GAUSS_5X5, 32, gScenarioKernelData51_APU_GAUSS_5X5, 4},
{52, 2, 880, 0, 1, gScenarioBufferData52_APU_GAUSS_5X5, 32, gScenarioKernelData52_APU_GAUSS_5X5, 4},
{56, 2, 944, 0, 1, gScenarioBufferData53_APU_GAUSS_5X5, 32, gScenarioKernelData53_APU_GAUSS_5X5, 4},
{60, 2, 1008, 0, 1, gScenarioBufferData54_APU_GAUSS_5X5, 32, gScenarioKernelData54_APU_GAUSS_5X5, 4},
{64, 2, 1072, 0, 1, gScenarioBufferData55_APU_GAUSS_5X5, 32, gScenarioKernelData55_APU_GAUSS_5X5, 4},
{72, 2, 1200, 0, 1, gScenarioBufferData56_APU_GAUSS_5X5, 32, gScenarioKernelData56_APU_GAUSS_5X5, 4},
{80, 2, 1328, 0, 1, gScenarioBufferData57_APU_GAUSS_5X5, 32, gScenarioKernelData57_APU_GAUSS_5X5, 4},
{88, 2, 1456, 0, 1, gScenarioBufferData58_APU_GAUSS_5X5, 32, gScenarioKernelData58_APU_GAUSS_5X5, 4},
{96, 2, 1584, 0, 1, gScenarioBufferData59_APU_GAUSS_5X5, 32, gScenarioKernelData59_APU_GAUSS_5X5, 4},
{104, 2, 1712, 0, 1, gScenarioBufferData60_APU_GAUSS_5X5, 32, gScenarioKernelData60_APU_GAUSS_5X5, 4},
{112, 2, 1840, 0, 1, gScenarioBufferData61_APU_GAUSS_5X5, 32, gScenarioKernelData61_APU_GAUSS_5X5, 4},
{120, 2, 1968, 0, 1, gScenarioBufferData62_APU_GAUSS_5X5, 32, gScenarioKernelData62_APU_GAUSS_5X5, 4},
{128, 2, 2096, 0, 1, gScenarioBufferData63_APU_GAUSS_5X5, 32, gScenarioKernelData63_APU_GAUSS_5X5, 4},
{2, 3, 108, 0, 1, gScenarioBufferData64_APU_GAUSS_5X5, 32, gScenarioKernelData64_APU_GAUSS_5X5, 4},
{4, 3, 152, 0, 1, gScenarioBufferData65_APU_GAUSS_5X5, 32, gScenarioKernelData65_APU_GAUSS_5X5, 4},
{6, 3, 196, 0, 1, gScenarioBufferData66_APU_GAUSS_5X5, 32, gScenarioKernelData66_APU_GAUSS_5X5, 4},
{8, 3, 240, 0, 1, gScenarioBufferData67_APU_GAUSS_5X5, 32, gScenarioKernelData67_APU_GAUSS_5X5, 4},
{10, 3, 284, 0, 1, gScenarioBufferData68_APU_GAUSS_5X5, 32, gScenarioKernelData68_APU_GAUSS_5X5, 4},
{12, 3, 328, 0, 1, gScenarioBufferData69_APU_GAUSS_5X5, 32, gScenarioKernelData69_APU_GAUSS_5X5, 4},
{14, 3, 372, 0, 1, gScenarioBufferData70_APU_GAUSS_5X5, 32, gScenarioKernelData70_APU_GAUSS_5X5, 4},
{16, 3, 416, 0, 1, gScenarioBufferData71_APU_GAUSS_5X5, 32, gScenarioKernelData71_APU_GAUSS_5X5, 4},
{18, 3, 460, 0, 1, gScenarioBufferData72_APU_GAUSS_5X5, 32, gScenarioKernelData72_APU_GAUSS_5X5, 4},
{20, 3, 504, 0, 1, gScenarioBufferData73_APU_GAUSS_5X5, 32, gScenarioKernelData73_APU_GAUSS_5X5, 4},
{22, 3, 548, 0, 1, gScenarioBufferData74_APU_GAUSS_5X5, 32, gScenarioKernelData74_APU_GAUSS_5X5, 4},
{24, 3, 592, 0, 1, gScenarioBufferData75_APU_GAUSS_5X5, 32, gScenarioKernelData75_APU_GAUSS_5X5, 4},
{26, 3, 636, 0, 1, gScenarioBufferData76_APU_GAUSS_5X5, 32, gScenarioKernelData76_APU_GAUSS_5X5, 4},
{28, 3, 680, 0, 1, gScenarioBufferData77_APU_GAUSS_5X5, 32, gScenarioKernelData77_APU_GAUSS_5X5, 4},
{30, 3, 724, 0, 1, gScenarioBufferData78_APU_GAUSS_5X5, 32, gScenarioKernelData78_APU_GAUSS_5X5, 4},
{32, 3, 768, 0, 1, gScenarioBufferData79_APU_GAUSS_5X5, 32, gScenarioKernelData79_APU_GAUSS_5X5, 4},
{36, 3, 856, 0, 1, gScenarioBufferData80_APU_GAUSS_5X5, 32, gScenarioKernelData80_APU_GAUSS_5X5, 4},
{40, 3, 944, 0, 1, gScenarioBufferData81_APU_GAUSS_5X5, 32, gScenarioKernelData81_APU_GAUSS_5X5, 4},
{44, 3, 1032, 0, 1, gScenarioBufferData82_APU_GAUSS_5X5, 32, gScenarioKernelData82_APU_GAUSS_5X5, 4},
{48, 3, 1120, 0, 1, gScenarioBufferData83_APU_GAUSS_5X5, 32, gScenarioKernelData83_APU_GAUSS_5X5, 4},
{52, 3, 1208, 0, 1, gScenarioBufferData84_APU_GAUSS_5X5, 32, gScenarioKernelData84_APU_GAUSS_5X5, 4},
{56, 3, 1296, 0, 1, gScenarioBufferData85_APU_GAUSS_5X5, 32, gScenarioKernelData85_APU_GAUSS_5X5, 4},
{60, 3, 1384, 0, 1, gScenarioBufferData86_APU_GAUSS_5X5, 32, gScenarioKernelData86_APU_GAUSS_5X5, 4},
{64, 3, 1472, 0, 1, gScenarioBufferData87_APU_GAUSS_5X5, 32, gScenarioKernelData87_APU_GAUSS_5X5, 4},
{72, 3, 1648, 0, 1, gScenarioBufferData88_APU_GAUSS_5X5, 32, gScenarioKernelData88_APU_GAUSS_5X5, 4},
{80, 3, 1824, 0, 1, gScenarioBufferData89_APU_GAUSS_5X5, 32, gScenarioKernelData89_APU_GAUSS_5X5, 4},
{88, 3, 2000, 0, 1, gScenarioBufferData90_APU_GAUSS_5X5, 32, gScenarioKernelData90_APU_GAUSS_5X5, 4},
{96, 3, 2176, 0, 1, gScenarioBufferData91_APU_GAUSS_5X5, 32, gScenarioKernelData91_APU_GAUSS_5X5, 4},
{104, 3, 2352, 0, 1, gScenarioBufferData92_APU_GAUSS_5X5, 32, gScenarioKernelData92_APU_GAUSS_5X5, 4},
{112, 3, 2528, 0, 1, gScenarioBufferData93_APU_GAUSS_5X5, 32, gScenarioKernelData93_APU_GAUSS_5X5, 4},
{120, 3, 2704, 0, 1, gScenarioBufferData94_APU_GAUSS_5X5, 32, gScenarioKernelData94_APU_GAUSS_5X5, 4},
{128, 3, 2880, 0, 1, gScenarioBufferData95_APU_GAUSS_5X5, 32, gScenarioKernelData95_APU_GAUSS_5X5, 4},
{2, 4, 136, 0, 1, gScenarioBufferData96_APU_GAUSS_5X5, 32, gScenarioKernelData96_APU_GAUSS_5X5, 4},
{4, 4, 192, 0, 1, gScenarioBufferData97_APU_GAUSS_5X5, 32, gScenarioKernelData97_APU_GAUSS_5X5, 4},
{6, 4, 248, 0, 1, gScenarioBufferData98_APU_GAUSS_5X5, 32, gScenarioKernelData98_APU_GAUSS_5X5, 4},
{8, 4, 304, 0, 1, gScenarioBufferData99_APU_GAUSS_5X5, 32, gScenarioKernelData99_APU_GAUSS_5X5, 4},
{10, 4, 360, 0, 1, gScenarioBufferData100_APU_GAUSS_5X5, 32, gScenarioKernelData100_APU_GAUSS_5X5, 4},
{12, 4, 416, 0, 1, gScenarioBufferData101_APU_GAUSS_5X5, 32, gScenarioKernelData101_APU_GAUSS_5X5, 4},
{14, 4, 472, 0, 1, gScenarioBufferData102_APU_GAUSS_5X5, 32, gScenarioKernelData102_APU_GAUSS_5X5, 4},
{16, 4, 528, 0, 1, gScenarioBufferData103_APU_GAUSS_5X5, 32, gScenarioKernelData103_APU_GAUSS_5X5, 4},
{18, 4, 584, 0, 1, gScenarioBufferData104_APU_GAUSS_5X5, 32, gScenarioKernelData104_APU_GAUSS_5X5, 4},
{20, 4, 640, 0, 1, gScenarioBufferData105_APU_GAUSS_5X5, 32, gScenarioKernelData105_APU_GAUSS_5X5, 4},
{22, 4, 696, 0, 1, gScenarioBufferData106_APU_GAUSS_5X5, 32, gScenarioKernelData106_APU_GAUSS_5X5, 4},
{24, 4, 752, 0, 1, gScenarioBufferData107_APU_GAUSS_5X5, 32, gScenarioKernelData107_APU_GAUSS_5X5, 4},
{26, 4, 808, 0, 1, gScenarioBufferData108_APU_GAUSS_5X5, 32, gScenarioKernelData108_APU_GAUSS_5X5, 4},
{28, 4, 864, 0, 1, gScenarioBufferData109_APU_GAUSS_5X5, 32, gScenarioKernelData109_APU_GAUSS_5X5, 4},
{30, 4, 920, 0, 1, gScenarioBufferData110_APU_GAUSS_5X5, 32, gScenarioKernelData110_APU_GAUSS_5X5, 4},
{32, 4, 976, 0, 1, gScenarioBufferData111_APU_GAUSS_5X5, 32, gScenarioKernelData111_APU_GAUSS_5X5, 4},
{36, 4, 1088, 0, 1, gScenarioBufferData112_APU_GAUSS_5X5, 32, gScenarioKernelData112_APU_GAUSS_5X5, 4},
{40, 4, 1200, 0, 1, gScenarioBufferData113_APU_GAUSS_5X5, 32, gScenarioKernelData113_APU_GAUSS_5X5, 4},
{44, 4, 1312, 0, 1, gScenarioBufferData114_APU_GAUSS_5X5, 32, gScenarioKernelData114_APU_GAUSS_5X5, 4},
{48, 4, 1424, 0, 1, gScenarioBufferData115_APU_GAUSS_5X5, 32, gScenarioKernelData115_APU_GAUSS_5X5, 4},
{52, 4, 1536, 0, 1, gScenarioBufferData116_APU_GAUSS_5X5, 32, gScenarioKernelData116_APU_GAUSS_5X5, 4},
{56, 4, 1648, 0, 1, gScenarioBufferData117_APU_GAUSS_5X5, 32, gScenarioKernelData117_APU_GAUSS_5X5, 4},
{60, 4, 1760, 0, 1, gScenarioBufferData118_APU_GAUSS_5X5, 32, gScenarioKernelData118_APU_GAUSS_5X5, 4},
{64, 4, 1872, 0, 1, gScenarioBufferData119_APU_GAUSS_5X5, 32, gScenarioKernelData119_APU_GAUSS_5X5, 4},
{72, 4, 2096, 0, 1, gScenarioBufferData120_APU_GAUSS_5X5, 32, gScenarioKernelData120_APU_GAUSS_5X5, 4},
{80, 4, 2320, 0, 1, gScenarioBufferData121_APU_GAUSS_5X5, 32, gScenarioKernelData121_APU_GAUSS_5X5, 4},
{88, 4, 2544, 0, 1, gScenarioBufferData122_APU_GAUSS_5X5, 32, gScenarioKernelData122_APU_GAUSS_5X5, 4},
{96, 4, 2768, 0, 1, gScenarioBufferData123_APU_GAUSS_5X5, 32, gScenarioKernelData123_APU_GAUSS_5X5, 4},
{104, 4, 2992, 0, 1, gScenarioBufferData124_APU_GAUSS_5X5, 32, gScenarioKernelData124_APU_GAUSS_5X5, 4},
{112, 4, 3216, 0, 1, gScenarioBufferData125_APU_GAUSS_5X5, 32, gScenarioKernelData125_APU_GAUSS_5X5, 4},
{120, 4, 3440, 0, 1, gScenarioBufferData126_APU_GAUSS_5X5, 32, gScenarioKernelData126_APU_GAUSS_5X5, 4},
{128, 4, 3664, 0, 1, gScenarioBufferData127_APU_GAUSS_5X5, 32, gScenarioKernelData127_APU_GAUSS_5X5, 4},
{2, 5, 164, 0, 1, gScenarioBufferData128_APU_GAUSS_5X5, 32, gScenarioKernelData128_APU_GAUSS_5X5, 4},
{4, 5, 232, 0, 1, gScenarioBufferData129_APU_GAUSS_5X5, 32, gScenarioKernelData129_APU_GAUSS_5X5, 4},
{6, 5, 300, 0, 1, gScenarioBufferData130_APU_GAUSS_5X5, 32, gScenarioKernelData130_APU_GAUSS_5X5, 4},
{8, 5, 368, 0, 1, gScenarioBufferData131_APU_GAUSS_5X5, 32, gScenarioKernelData131_APU_GAUSS_5X5, 4},
{10, 5, 436, 0, 1, gScenarioBufferData132_APU_GAUSS_5X5, 32, gScenarioKernelData132_APU_GAUSS_5X5, 4},
{12, 5, 504, 0, 1, gScenarioBufferData133_APU_GAUSS_5X5, 32, gScenarioKernelData133_APU_GAUSS_5X5, 4},
{14, 5, 572, 0, 1, gScenarioBufferData134_APU_GAUSS_5X5, 32, gScenarioKernelData134_APU_GAUSS_5X5, 4},
{16, 5, 640, 0, 1, gScenarioBufferData135_APU_GAUSS_5X5, 32, gScenarioKernelData135_APU_GAUSS_5X5, 4},
{18, 5, 708, 0, 1, gScenarioBufferData136_APU_GAUSS_5X5, 32, gScenarioKernelData136_APU_GAUSS_5X5, 4},
{20, 5, 776, 0, 1, gScenarioBufferData137_APU_GAUSS_5X5, 32, gScenarioKernelData137_APU_GAUSS_5X5, 4},
{22, 5, 844, 0, 1, gScenarioBufferData138_APU_GAUSS_5X5, 32, gScenarioKernelData138_APU_GAUSS_5X5, 4},
{24, 5, 912, 0, 1, gScenarioBufferData139_APU_GAUSS_5X5, 32, gScenarioKernelData139_APU_GAUSS_5X5, 4},
{26, 5, 980, 0, 1, gScenarioBufferData140_APU_GAUSS_5X5, 32, gScenarioKernelData140_APU_GAUSS_5X5, 4},
{28, 5, 1048, 0, 1, gScenarioBufferData141_APU_GAUSS_5X5, 32, gScenarioKernelData141_APU_GAUSS_5X5, 4},
{30, 5, 1116, 0, 1, gScenarioBufferData142_APU_GAUSS_5X5, 32, gScenarioKernelData142_APU_GAUSS_5X5, 4},
{32, 5, 1184, 0, 1, gScenarioBufferData143_APU_GAUSS_5X5, 32, gScenarioKernelData143_APU_GAUSS_5X5, 4},
{36, 5, 1320, 0, 1, gScenarioBufferData144_APU_GAUSS_5X5, 32, gScenarioKernelData144_APU_GAUSS_5X5, 4},
{40, 5, 1456, 0, 1, gScenarioBufferData145_APU_GAUSS_5X5, 32, gScenarioKernelData145_APU_GAUSS_5X5, 4},
{44, 5, 1592, 0, 1, gScenarioBufferData146_APU_GAUSS_5X5, 32, gScenarioKernelData146_APU_GAUSS_5X5, 4},
{48, 5, 1728, 0, 1, gScenarioBufferData147_APU_GAUSS_5X5, 32, gScenarioKernelData147_APU_GAUSS_5X5, 4},
{52, 5, 1864, 0, 1, gScenarioBufferData148_APU_GAUSS_5X5, 32, gScenarioKernelData148_APU_GAUSS_5X5, 4},
{56, 5, 2000, 0, 1, gScenarioBufferData149_APU_GAUSS_5X5, 32, gScenarioKernelData149_APU_GAUSS_5X5, 4},
{60, 5, 2136, 0, 1, gScenarioBufferData150_APU_GAUSS_5X5, 32, gScenarioKernelData150_APU_GAUSS_5X5, 4},
{64, 5, 2272, 0, 1, gScenarioBufferData151_APU_GAUSS_5X5, 32, gScenarioKernelData151_APU_GAUSS_5X5, 4},
{72, 5, 2544, 0, 1, gScenarioBufferData152_APU_GAUSS_5X5, 32, gScenarioKernelData152_APU_GAUSS_5X5, 4},
{80, 5, 2816, 0, 1, gScenarioBufferData153_APU_GAUSS_5X5, 32, gScenarioKernelData153_APU_GAUSS_5X5, 4},
{88, 5, 3088, 0, 1, gScenarioBufferData154_APU_GAUSS_5X5, 32, gScenarioKernelData154_APU_GAUSS_5X5, 4},
{96, 5, 3360, 0, 1, gScenarioBufferData155_APU_GAUSS_5X5, 32, gScenarioKernelData155_APU_GAUSS_5X5, 4},
{104, 5, 3632, 0, 1, gScenarioBufferData156_APU_GAUSS_5X5, 32, gScenarioKernelData156_APU_GAUSS_5X5, 4},
{2, 6, 192, 0, 1, gScenarioBufferData157_APU_GAUSS_5X5, 32, gScenarioKernelData157_APU_GAUSS_5X5, 4},
{4, 6, 272, 0, 1, gScenarioBufferData158_APU_GAUSS_5X5, 32, gScenarioKernelData158_APU_GAUSS_5X5, 4},
{6, 6, 352, 0, 1, gScenarioBufferData159_APU_GAUSS_5X5, 32, gScenarioKernelData159_APU_GAUSS_5X5, 4},
{8, 6, 432, 0, 1, gScenarioBufferData160_APU_GAUSS_5X5, 32, gScenarioKernelData160_APU_GAUSS_5X5, 4},
{10, 6, 512, 0, 1, gScenarioBufferData161_APU_GAUSS_5X5, 32, gScenarioKernelData161_APU_GAUSS_5X5, 4},
{12, 6, 592, 0, 1, gScenarioBufferData162_APU_GAUSS_5X5, 32, gScenarioKernelData162_APU_GAUSS_5X5, 4},
{14, 6, 672, 0, 1, gScenarioBufferData163_APU_GAUSS_5X5, 32, gScenarioKernelData163_APU_GAUSS_5X5, 4},
{16, 6, 752, 0, 1, gScenarioBufferData164_APU_GAUSS_5X5, 32, gScenarioKernelData164_APU_GAUSS_5X5, 4},
{18, 6, 832, 0, 1, gScenarioBufferData165_APU_GAUSS_5X5, 32, gScenarioKernelData165_APU_GAUSS_5X5, 4},
{20, 6, 912, 0, 1, gScenarioBufferData166_APU_GAUSS_5X5, 32, gScenarioKernelData166_APU_GAUSS_5X5, 4},
{22, 6, 992, 0, 1, gScenarioBufferData167_APU_GAUSS_5X5, 32, gScenarioKernelData167_APU_GAUSS_5X5, 4},
{24, 6, 1072, 0, 1, gScenarioBufferData168_APU_GAUSS_5X5, 32, gScenarioKernelData168_APU_GAUSS_5X5, 4},
{26, 6, 1152, 0, 1, gScenarioBufferData169_APU_GAUSS_5X5, 32, gScenarioKernelData169_APU_GAUSS_5X5, 4},
{28, 6, 1232, 0, 1, gScenarioBufferData170_APU_GAUSS_5X5, 32, gScenarioKernelData170_APU_GAUSS_5X5, 4},
{30, 6, 1312, 0, 1, gScenarioBufferData171_APU_GAUSS_5X5, 32, gScenarioKernelData171_APU_GAUSS_5X5, 4},
{32, 6, 1392, 0, 1, gScenarioBufferData172_APU_GAUSS_5X5, 32, gScenarioKernelData172_APU_GAUSS_5X5, 4},
{36, 6, 1552, 0, 1, gScenarioBufferData173_APU_GAUSS_5X5, 32, gScenarioKernelData173_APU_GAUSS_5X5, 4},
{40, 6, 1712, 0, 1, gScenarioBufferData174_APU_GAUSS_5X5, 32, gScenarioKernelData174_APU_GAUSS_5X5, 4},
{44, 6, 1872, 0, 1, gScenarioBufferData175_APU_GAUSS_5X5, 32, gScenarioKernelData175_APU_GAUSS_5X5, 4},
{48, 6, 2032, 0, 1, gScenarioBufferData176_APU_GAUSS_5X5, 32, gScenarioKernelData176_APU_GAUSS_5X5, 4},
{52, 6, 2192, 0, 1, gScenarioBufferData177_APU_GAUSS_5X5, 32, gScenarioKernelData177_APU_GAUSS_5X5, 4},
{56, 6, 2352, 0, 1, gScenarioBufferData178_APU_GAUSS_5X5, 32, gScenarioKernelData178_APU_GAUSS_5X5, 4},
{60, 6, 2512, 0, 1, gScenarioBufferData179_APU_GAUSS_5X5, 32, gScenarioKernelData179_APU_GAUSS_5X5, 4},
{64, 6, 2672, 0, 1, gScenarioBufferData180_APU_GAUSS_5X5, 32, gScenarioKernelData180_APU_GAUSS_5X5, 4},
{72, 6, 2992, 0, 1, gScenarioBufferData181_APU_GAUSS_5X5, 32, gScenarioKernelData181_APU_GAUSS_5X5, 4},
{80, 6, 3312, 0, 1, gScenarioBufferData182_APU_GAUSS_5X5, 32, gScenarioKernelData182_APU_GAUSS_5X5, 4},
{88, 6, 3632, 0, 1, gScenarioBufferData183_APU_GAUSS_5X5, 32, gScenarioKernelData183_APU_GAUSS_5X5, 4},
{2, 8, 248, 0, 1, gScenarioBufferData184_APU_GAUSS_5X5, 32, gScenarioKernelData184_APU_GAUSS_5X5, 4},
{4, 8, 352, 0, 1, gScenarioBufferData185_APU_GAUSS_5X5, 32, gScenarioKernelData185_APU_GAUSS_5X5, 4},
{6, 8, 456, 0, 1, gScenarioBufferData186_APU_GAUSS_5X5, 32, gScenarioKernelData186_APU_GAUSS_5X5, 4},
{8, 8, 560, 0, 1, gScenarioBufferData187_APU_GAUSS_5X5, 32, gScenarioKernelData187_APU_GAUSS_5X5, 4},
{10, 8, 664, 0, 1, gScenarioBufferData188_APU_GAUSS_5X5, 32, gScenarioKernelData188_APU_GAUSS_5X5, 4},
{12, 8, 768, 0, 1, gScenarioBufferData189_APU_GAUSS_5X5, 32, gScenarioKernelData189_APU_GAUSS_5X5, 4},
{14, 8, 872, 0, 1, gScenarioBufferData190_APU_GAUSS_5X5, 32, gScenarioKernelData190_APU_GAUSS_5X5, 4},
{16, 8, 976, 0, 1, gScenarioBufferData191_APU_GAUSS_5X5, 32, gScenarioKernelData191_APU_GAUSS_5X5, 4},
{18, 8, 1080, 0, 1, gScenarioBufferData192_APU_GAUSS_5X5, 32, gScenarioKernelData192_APU_GAUSS_5X5, 4},
{20, 8, 1184, 0, 1, gScenarioBufferData193_APU_GAUSS_5X5, 32, gScenarioKernelData193_APU_GAUSS_5X5, 4},
{22, 8, 1288, 0, 1, gScenarioBufferData194_APU_GAUSS_5X5, 32, gScenarioKernelData194_APU_GAUSS_5X5, 4},
{24, 8, 1392, 0, 1, gScenarioBufferData195_APU_GAUSS_5X5, 32, gScenarioKernelData195_APU_GAUSS_5X5, 4},
{26, 8, 1496, 0, 1, gScenarioBufferData196_APU_GAUSS_5X5, 32, gScenarioKernelData196_APU_GAUSS_5X5, 4},
{28, 8, 1600, 0, 1, gScenarioBufferData197_APU_GAUSS_5X5, 32, gScenarioKernelData197_APU_GAUSS_5X5, 4},
{30, 8, 1704, 0, 1, gScenarioBufferData198_APU_GAUSS_5X5, 32, gScenarioKernelData198_APU_GAUSS_5X5, 4},
{32, 8, 1808, 0, 1, gScenarioBufferData199_APU_GAUSS_5X5, 32, gScenarioKernelData199_APU_GAUSS_5X5, 4},
{36, 8, 2016, 0, 1, gScenarioBufferData200_APU_GAUSS_5X5, 32, gScenarioKernelData200_APU_GAUSS_5X5, 4},
{40, 8, 2224, 0, 1, gScenarioBufferData201_APU_GAUSS_5X5, 32, gScenarioKernelData201_APU_GAUSS_5X5, 4},
{44, 8, 2432, 0, 1, gScenarioBufferData202_APU_GAUSS_5X5, 32, gScenarioKernelData202_APU_GAUSS_5X5, 4},
{48, 8, 2640, 0, 1, gScenarioBufferData203_APU_GAUSS_5X5, 32, gScenarioKernelData203_APU_GAUSS_5X5, 4},
{52, 8, 2848, 0, 1, gScenarioBufferData204_APU_GAUSS_5X5, 32, gScenarioKernelData204_APU_GAUSS_5X5, 4},
{56, 8, 3056, 0, 1, gScenarioBufferData205_APU_GAUSS_5X5, 32, gScenarioKernelData205_APU_GAUSS_5X5, 4},
{60, 8, 3264, 0, 1, gScenarioBufferData206_APU_GAUSS_5X5, 32, gScenarioKernelData206_APU_GAUSS_5X5, 4},
{64, 8, 3472, 0, 1, gScenarioBufferData207_APU_GAUSS_5X5, 32, gScenarioKernelData207_APU_GAUSS_5X5, 4},
{2, 10, 304, 0, 1, gScenarioBufferData208_APU_GAUSS_5X5, 32, gScenarioKernelData208_APU_GAUSS_5X5, 4},
{4, 10, 432, 0, 1, gScenarioBufferData209_APU_GAUSS_5X5, 32, gScenarioKernelData209_APU_GAUSS_5X5, 4},
{6, 10, 560, 0, 1, gScenarioBufferData210_APU_GAUSS_5X5, 32, gScenarioKernelData210_APU_GAUSS_5X5, 4},
{8, 10, 688, 0, 1, gScenarioBufferData211_APU_GAUSS_5X5, 32, gScenarioKernelData211_APU_GAUSS_5X5, 4},
{10, 10, 816, 0, 1, gScenarioBufferData212_APU_GAUSS_5X5, 32, gScenarioKernelData212_APU_GAUSS_5X5, 4},
{12, 10, 944, 0, 1, gScenarioBufferData213_APU_GAUSS_5X5, 32, gScenarioKernelData213_APU_GAUSS_5X5, 4},
{14, 10, 1072, 0, 1, gScenarioBufferData214_APU_GAUSS_5X5, 32, gScenarioKernelData214_APU_GAUSS_5X5, 4},
{16, 10, 1200, 0, 1, gScenarioBufferData215_APU_GAUSS_5X5, 32, gScenarioKernelData215_APU_GAUSS_5X5, 4},
{18, 10, 1328, 0, 1, gScenarioBufferData216_APU_GAUSS_5X5, 32, gScenarioKernelData216_APU_GAUSS_5X5, 4},
{20, 10, 1456, 0, 1, gScenarioBufferData217_APU_GAUSS_5X5, 32, gScenarioKernelData217_APU_GAUSS_5X5, 4},
{22, 10, 1584, 0, 1, gScenarioBufferData218_APU_GAUSS_5X5, 32, gScenarioKernelData218_APU_GAUSS_5X5, 4},
{24, 10, 1712, 0, 1, gScenarioBufferData219_APU_GAUSS_5X5, 32, gScenarioKernelData219_APU_GAUSS_5X5, 4},
{26, 10, 1840, 0, 1, gScenarioBufferData220_APU_GAUSS_5X5, 32, gScenarioKernelData220_APU_GAUSS_5X5, 4},
{28, 10, 1968, 0, 1, gScenarioBufferData221_APU_GAUSS_5X5, 32, gScenarioKernelData221_APU_GAUSS_5X5, 4},
{30, 10, 2096, 0, 1, gScenarioBufferData222_APU_GAUSS_5X5, 32, gScenarioKernelData222_APU_GAUSS_5X5, 4},
{32, 10, 2224, 0, 1, gScenarioBufferData223_APU_GAUSS_5X5, 32, gScenarioKernelData223_APU_GAUSS_5X5, 4},
{36, 10, 2480, 0, 1, gScenarioBufferData224_APU_GAUSS_5X5, 32, gScenarioKernelData224_APU_GAUSS_5X5, 4},
{40, 10, 2736, 0, 1, gScenarioBufferData225_APU_GAUSS_5X5, 32, gScenarioKernelData225_APU_GAUSS_5X5, 4},
{44, 10, 2992, 0, 1, gScenarioBufferData226_APU_GAUSS_5X5, 32, gScenarioKernelData226_APU_GAUSS_5X5, 4},
{48, 10, 3248, 0, 1, gScenarioBufferData227_APU_GAUSS_5X5, 32, gScenarioKernelData227_APU_GAUSS_5X5, 4},
{52, 10, 3504, 0, 1, gScenarioBufferData228_APU_GAUSS_5X5, 32, gScenarioKernelData228_APU_GAUSS_5X5, 4},
{56, 10, 3760, 0, 1, gScenarioBufferData229_APU_GAUSS_5X5, 32, gScenarioKernelData229_APU_GAUSS_5X5, 4},
{2, 12, 360, 0, 1, gScenarioBufferData230_APU_GAUSS_5X5, 32, gScenarioKernelData230_APU_GAUSS_5X5, 4},
{4, 12, 512, 0, 1, gScenarioBufferData231_APU_GAUSS_5X5, 32, gScenarioKernelData231_APU_GAUSS_5X5, 4},
{6, 12, 664, 0, 1, gScenarioBufferData232_APU_GAUSS_5X5, 32, gScenarioKernelData232_APU_GAUSS_5X5, 4},
{8, 12, 816, 0, 1, gScenarioBufferData233_APU_GAUSS_5X5, 32, gScenarioKernelData233_APU_GAUSS_5X5, 4},
{10, 12, 968, 0, 1, gScenarioBufferData234_APU_GAUSS_5X5, 32, gScenarioKernelData234_APU_GAUSS_5X5, 4},
{12, 12, 1120, 0, 1, gScenarioBufferData235_APU_GAUSS_5X5, 32, gScenarioKernelData235_APU_GAUSS_5X5, 4},
{14, 12, 1272, 0, 1, gScenarioBufferData236_APU_GAUSS_5X5, 32, gScenarioKernelData236_APU_GAUSS_5X5, 4},
{16, 12, 1424, 0, 1, gScenarioBufferData237_APU_GAUSS_5X5, 32, gScenarioKernelData237_APU_GAUSS_5X5, 4},
{18, 12, 1576, 0, 1, gScenarioBufferData238_APU_GAUSS_5X5, 32, gScenarioKernelData238_APU_GAUSS_5X5, 4},
{20, 12, 1728, 0, 1, gScenarioBufferData239_APU_GAUSS_5X5, 32, gScenarioKernelData239_APU_GAUSS_5X5, 4},
{22, 12, 1880, 0, 1, gScenarioBufferData240_APU_GAUSS_5X5, 32, gScenarioKernelData240_APU_GAUSS_5X5, 4},
{24, 12, 2032, 0, 1, gScenarioBufferData241_APU_GAUSS_5X5, 32, gScenarioKernelData241_APU_GAUSS_5X5, 4},
{26, 12, 2184, 0, 1, gScenarioBufferData242_APU_GAUSS_5X5, 32, gScenarioKernelData242_APU_GAUSS_5X5, 4},
{28, 12, 2336, 0, 1, gScenarioBufferData243_APU_GAUSS_5X5, 32, gScenarioKernelData243_APU_GAUSS_5X5, 4},
{30, 12, 2488, 0, 1, gScenarioBufferData244_APU_GAUSS_5X5, 32, gScenarioKernelData244_APU_GAUSS_5X5, 4},
{32, 12, 2640, 0, 1, gScenarioBufferData245_APU_GAUSS_5X5, 32, gScenarioKernelData245_APU_GAUSS_5X5, 4},
{36, 12, 2944, 0, 1, gScenarioBufferData246_APU_GAUSS_5X5, 32, gScenarioKernelData246_APU_GAUSS_5X5, 4},
{40, 12, 3248, 0, 1, gScenarioBufferData247_APU_GAUSS_5X5, 32, gScenarioKernelData247_APU_GAUSS_5X5, 4},
{44, 12, 3552, 0, 1, gScenarioBufferData248_APU_GAUSS_5X5, 32, gScenarioKernelData248_APU_GAUSS_5X5, 4},
{2, 14, 416, 0, 1, gScenarioBufferData249_APU_GAUSS_5X5, 32, gScenarioKernelData249_APU_GAUSS_5X5, 4},
{4, 14, 592, 0, 1, gScenarioBufferData250_APU_GAUSS_5X5, 32, gScenarioKernelData250_APU_GAUSS_5X5, 4},
{6, 14, 768, 0, 1, gScenarioBufferData251_APU_GAUSS_5X5, 32, gScenarioKernelData251_APU_GAUSS_5X5, 4},
{8, 14, 944, 0, 1, gScenarioBufferData252_APU_GAUSS_5X5, 32, gScenarioKernelData252_APU_GAUSS_5X5, 4},
{10, 14, 1120, 0, 1, gScenarioBufferData253_APU_GAUSS_5X5, 32, gScenarioKernelData253_APU_GAUSS_5X5, 4},
{12, 14, 1296, 0, 1, gScenarioBufferData254_APU_GAUSS_5X5, 32, gScenarioKernelData254_APU_GAUSS_5X5, 4},
{14, 14, 1472, 0, 1, gScenarioBufferData255_APU_GAUSS_5X5, 32, gScenarioKernelData255_APU_GAUSS_5X5, 4},
{16, 14, 1648, 0, 1, gScenarioBufferData256_APU_GAUSS_5X5, 32, gScenarioKernelData256_APU_GAUSS_5X5, 4},
{18, 14, 1824, 0, 1, gScenarioBufferData257_APU_GAUSS_5X5, 32, gScenarioKernelData257_APU_GAUSS_5X5, 4},
{20, 14, 2000, 0, 1, gScenarioBufferData258_APU_GAUSS_5X5, 32, gScenarioKernelData258_APU_GAUSS_5X5, 4},
{22, 14, 2176, 0, 1, gScenarioBufferData259_APU_GAUSS_5X5, 32, gScenarioKernelData259_APU_GAUSS_5X5, 4},
{24, 14, 2352, 0, 1, gScenarioBufferData260_APU_GAUSS_5X5, 32, gScenarioKernelData260_APU_GAUSS_5X5, 4},
{26, 14, 2528, 0, 1, gScenarioBufferData261_APU_GAUSS_5X5, 32, gScenarioKernelData261_APU_GAUSS_5X5, 4},
{28, 14, 2704, 0, 1, gScenarioBufferData262_APU_GAUSS_5X5, 32, gScenarioKernelData262_APU_GAUSS_5X5, 4},
{30, 14, 2880, 0, 1, gScenarioBufferData263_APU_GAUSS_5X5, 32, gScenarioKernelData263_APU_GAUSS_5X5, 4},
{32, 14, 3056, 0, 1, gScenarioBufferData264_APU_GAUSS_5X5, 32, gScenarioKernelData264_APU_GAUSS_5X5, 4},
{36, 14, 3408, 0, 1, gScenarioBufferData265_APU_GAUSS_5X5, 32, gScenarioKernelData265_APU_GAUSS_5X5, 4},
{40, 14, 3760, 0, 1, gScenarioBufferData266_APU_GAUSS_5X5, 32, gScenarioKernelData266_APU_GAUSS_5X5, 4},
{2, 16, 472, 0, 1, gScenarioBufferData267_APU_GAUSS_5X5, 32, gScenarioKernelData267_APU_GAUSS_5X5, 4},
{4, 16, 672, 0, 1, gScenarioBufferData268_APU_GAUSS_5X5, 32, gScenarioKernelData268_APU_GAUSS_5X5, 4},
{6, 16, 872, 0, 1, gScenarioBufferData269_APU_GAUSS_5X5, 32, gScenarioKernelData269_APU_GAUSS_5X5, 4},
{8, 16, 1072, 0, 1, gScenarioBufferData270_APU_GAUSS_5X5, 32, gScenarioKernelData270_APU_GAUSS_5X5, 4},
{10, 16, 1272, 0, 1, gScenarioBufferData271_APU_GAUSS_5X5, 32, gScenarioKernelData271_APU_GAUSS_5X5, 4},
{12, 16, 1472, 0, 1, gScenarioBufferData272_APU_GAUSS_5X5, 32, gScenarioKernelData272_APU_GAUSS_5X5, 4},
{14, 16, 1672, 0, 1, gScenarioBufferData273_APU_GAUSS_5X5, 32, gScenarioKernelData273_APU_GAUSS_5X5, 4},
{16, 16, 1872, 0, 1, gScenarioBufferData274_APU_GAUSS_5X5, 32, gScenarioKernelData274_APU_GAUSS_5X5, 4},
{18, 16, 2072, 0, 1, gScenarioBufferData275_APU_GAUSS_5X5, 32, gScenarioKernelData275_APU_GAUSS_5X5, 4},
{20, 16, 2272, 0, 1, gScenarioBufferData276_APU_GAUSS_5X5, 32, gScenarioKernelData276_APU_GAUSS_5X5, 4},
{22, 16, 2472, 0, 1, gScenarioBufferData277_APU_GAUSS_5X5, 32, gScenarioKernelData277_APU_GAUSS_5X5, 4},
{24, 16, 2672, 0, 1, gScenarioBufferData278_APU_GAUSS_5X5, 32, gScenarioKernelData278_APU_GAUSS_5X5, 4},
{26, 16, 2872, 0, 1, gScenarioBufferData279_APU_GAUSS_5X5, 32, gScenarioKernelData279_APU_GAUSS_5X5, 4},
{28, 16, 3072, 0, 1, gScenarioBufferData280_APU_GAUSS_5X5, 32, gScenarioKernelData280_APU_GAUSS_5X5, 4},
{30, 16, 3272, 0, 1, gScenarioBufferData281_APU_GAUSS_5X5, 32, gScenarioKernelData281_APU_GAUSS_5X5, 4},
{32, 16, 3472, 0, 1, gScenarioBufferData282_APU_GAUSS_5X5, 32, gScenarioKernelData282_APU_GAUSS_5X5, 4},
{2, 18, 528, 0, 1, gScenarioBufferData283_APU_GAUSS_5X5, 32, gScenarioKernelData283_APU_GAUSS_5X5, 4},
{4, 18, 752, 0, 1, gScenarioBufferData284_APU_GAUSS_5X5, 32, gScenarioKernelData284_APU_GAUSS_5X5, 4},
{6, 18, 976, 0, 1, gScenarioBufferData285_APU_GAUSS_5X5, 32, gScenarioKernelData285_APU_GAUSS_5X5, 4},
{8, 18, 1200, 0, 1, gScenarioBufferData286_APU_GAUSS_5X5, 32, gScenarioKernelData286_APU_GAUSS_5X5, 4},
{10, 18, 1424, 0, 1, gScenarioBufferData287_APU_GAUSS_5X5, 32, gScenarioKernelData287_APU_GAUSS_5X5, 4},
{12, 18, 1648, 0, 1, gScenarioBufferData288_APU_GAUSS_5X5, 32, gScenarioKernelData288_APU_GAUSS_5X5, 4},
{14, 18, 1872, 0, 1, gScenarioBufferData289_APU_GAUSS_5X5, 32, gScenarioKernelData289_APU_GAUSS_5X5, 4},
{16, 18, 2096, 0, 1, gScenarioBufferData290_APU_GAUSS_5X5, 32, gScenarioKernelData290_APU_GAUSS_5X5, 4},
{18, 18, 2320, 0, 1, gScenarioBufferData291_APU_GAUSS_5X5, 32, gScenarioKernelData291_APU_GAUSS_5X5, 4},
{20, 18, 2544, 0, 1, gScenarioBufferData292_APU_GAUSS_5X5, 32, gScenarioKernelData292_APU_GAUSS_5X5, 4},
{22, 18, 2768, 0, 1, gScenarioBufferData293_APU_GAUSS_5X5, 32, gScenarioKernelData293_APU_GAUSS_5X5, 4},
{24, 18, 2992, 0, 1, gScenarioBufferData294_APU_GAUSS_5X5, 32, gScenarioKernelData294_APU_GAUSS_5X5, 4},
{26, 18, 3216, 0, 1, gScenarioBufferData295_APU_GAUSS_5X5, 32, gScenarioKernelData295_APU_GAUSS_5X5, 4},
{28, 18, 3440, 0, 1, gScenarioBufferData296_APU_GAUSS_5X5, 32, gScenarioKernelData296_APU_GAUSS_5X5, 4},
{30, 18, 3664, 0, 1, gScenarioBufferData297_APU_GAUSS_5X5, 32, gScenarioKernelData297_APU_GAUSS_5X5, 4},
{2, 20, 584, 0, 1, gScenarioBufferData298_APU_GAUSS_5X5, 32, gScenarioKernelData298_APU_GAUSS_5X5, 4},
{4, 20, 832, 0, 1, gScenarioBufferData299_APU_GAUSS_5X5, 32, gScenarioKernelData299_APU_GAUSS_5X5, 4},
{6, 20, 1080, 0, 1, gScenarioBufferData300_APU_GAUSS_5X5, 32, gScenarioKernelData300_APU_GAUSS_5X5, 4},
{8, 20, 1328, 0, 1, gScenarioBufferData301_APU_GAUSS_5X5, 32, gScenarioKernelData301_APU_GAUSS_5X5, 4},
{10, 20, 1576, 0, 1, gScenarioBufferData302_APU_GAUSS_5X5, 32, gScenarioKernelData302_APU_GAUSS_5X5, 4},
{12, 20, 1824, 0, 1, gScenarioBufferData303_APU_GAUSS_5X5, 32, gScenarioKernelData303_APU_GAUSS_5X5, 4},
{14, 20, 2072, 0, 1, gScenarioBufferData304_APU_GAUSS_5X5, 32, gScenarioKernelData304_APU_GAUSS_5X5, 4},
{16, 20, 2320, 0, 1, gScenarioBufferData305_APU_GAUSS_5X5, 32, gScenarioKernelData305_APU_GAUSS_5X5, 4},
{18, 20, 2568, 0, 1, gScenarioBufferData306_APU_GAUSS_5X5, 32, gScenarioKernelData306_APU_GAUSS_5X5, 4},
{20, 20, 2816, 0, 1, gScenarioBufferData307_APU_GAUSS_5X5, 32, gScenarioKernelData307_APU_GAUSS_5X5, 4},
{22, 20, 3064, 0, 1, gScenarioBufferData308_APU_GAUSS_5X5, 32, gScenarioKernelData308_APU_GAUSS_5X5, 4},
{24, 20, 3312, 0, 1, gScenarioBufferData309_APU_GAUSS_5X5, 32, gScenarioKernelData309_APU_GAUSS_5X5, 4},
{26, 20, 3560, 0, 1, gScenarioBufferData310_APU_GAUSS_5X5, 32, gScenarioKernelData310_APU_GAUSS_5X5, 4},
{28, 20, 3808, 0, 1, gScenarioBufferData311_APU_GAUSS_5X5, 32, gScenarioKernelData311_APU_GAUSS_5X5, 4},
{2, 22, 640, 0, 1, gScenarioBufferData312_APU_GAUSS_5X5, 32, gScenarioKernelData312_APU_GAUSS_5X5, 4},
{4, 22, 912, 0, 1, gScenarioBufferData313_APU_GAUSS_5X5, 32, gScenarioKernelData313_APU_GAUSS_5X5, 4},
{6, 22, 1184, 0, 1, gScenarioBufferData314_APU_GAUSS_5X5, 32, gScenarioKernelData314_APU_GAUSS_5X5, 4},
{8, 22, 1456, 0, 1, gScenarioBufferData315_APU_GAUSS_5X5, 32, gScenarioKernelData315_APU_GAUSS_5X5, 4},
{10, 22, 1728, 0, 1, gScenarioBufferData316_APU_GAUSS_5X5, 32, gScenarioKernelData316_APU_GAUSS_5X5, 4},
{12, 22, 2000, 0, 1, gScenarioBufferData317_APU_GAUSS_5X5, 32, gScenarioKernelData317_APU_GAUSS_5X5, 4},
{14, 22, 2272, 0, 1, gScenarioBufferData318_APU_GAUSS_5X5, 32, gScenarioKernelData318_APU_GAUSS_5X5, 4},
{16, 22, 2544, 0, 1, gScenarioBufferData319_APU_GAUSS_5X5, 32, gScenarioKernelData319_APU_GAUSS_5X5, 4},
{18, 22, 2816, 0, 1, gScenarioBufferData320_APU_GAUSS_5X5, 32, gScenarioKernelData320_APU_GAUSS_5X5, 4},
{20, 22, 3088, 0, 1, gScenarioBufferData321_APU_GAUSS_5X5, 32, gScenarioKernelData321_APU_GAUSS_5X5, 4},
{22, 22, 3360, 0, 1, gScenarioBufferData322_APU_GAUSS_5X5, 32, gScenarioKernelData322_APU_GAUSS_5X5, 4},
{24, 22, 3632, 0, 1, gScenarioBufferData323_APU_GAUSS_5X5, 32, gScenarioKernelData323_APU_GAUSS_5X5, 4},
{2, 24, 696, 0, 1, gScenarioBufferData324_APU_GAUSS_5X5, 32, gScenarioKernelData324_APU_GAUSS_5X5, 4},
{4, 24, 992, 0, 1, gScenarioBufferData325_APU_GAUSS_5X5, 32, gScenarioKernelData325_APU_GAUSS_5X5, 4},
{6, 24, 1288, 0, 1, gScenarioBufferData326_APU_GAUSS_5X5, 32, gScenarioKernelData326_APU_GAUSS_5X5, 4},
{8, 24, 1584, 0, 1, gScenarioBufferData327_APU_GAUSS_5X5, 32, gScenarioKernelData327_APU_GAUSS_5X5, 4},
{10, 24, 1880, 0, 1, gScenarioBufferData328_APU_GAUSS_5X5, 32, gScenarioKernelData328_APU_GAUSS_5X5, 4},
{12, 24, 2176, 0, 1, gScenarioBufferData329_APU_GAUSS_5X5, 32, gScenarioKernelData329_APU_GAUSS_5X5, 4},
{14, 24, 2472, 0, 1, gScenarioBufferData330_APU_GAUSS_5X5, 32, gScenarioKernelData330_APU_GAUSS_5X5, 4},
{16, 24, 2768, 0, 1, gScenarioBufferData331_APU_GAUSS_5X5, 32, gScenarioKernelData331_APU_GAUSS_5X5, 4},
{18, 24, 3064, 0, 1, gScenarioBufferData332_APU_GAUSS_5X5, 32, gScenarioKernelData332_APU_GAUSS_5X5, 4},
{20, 24, 3360, 0, 1, gScenarioBufferData333_APU_GAUSS_5X5, 32, gScenarioKernelData333_APU_GAUSS_5X5, 4},
{22, 24, 3656, 0, 1, gScenarioBufferData334_APU_GAUSS_5X5, 32, gScenarioKernelData334_APU_GAUSS_5X5, 4},
{2, 26, 752, 0, 1, gScenarioBufferData335_APU_GAUSS_5X5, 32, gScenarioKernelData335_APU_GAUSS_5X5, 4},
{4, 26, 1072, 0, 1, gScenarioBufferData336_APU_GAUSS_5X5, 32, gScenarioKernelData336_APU_GAUSS_5X5, 4},
{6, 26, 1392, 0, 1, gScenarioBufferData337_APU_GAUSS_5X5, 32, gScenarioKernelData337_APU_GAUSS_5X5, 4},
{8, 26, 1712, 0, 1, gScenarioBufferData338_APU_GAUSS_5X5, 32, gScenarioKernelData338_APU_GAUSS_5X5, 4},
{10, 26, 2032, 0, 1, gScenarioBufferData339_APU_GAUSS_5X5, 32, gScenarioKernelData339_APU_GAUSS_5X5, 4},
{12, 26, 2352, 0, 1, gScenarioBufferData340_APU_GAUSS_5X5, 32, gScenarioKernelData340_APU_GAUSS_5X5, 4},
{14, 26, 2672, 0, 1, gScenarioBufferData341_APU_GAUSS_5X5, 32, gScenarioKernelData341_APU_GAUSS_5X5, 4},
{16, 26, 2992, 0, 1, gScenarioBufferData342_APU_GAUSS_5X5, 32, gScenarioKernelData342_APU_GAUSS_5X5, 4},
{18, 26, 3312, 0, 1, gScenarioBufferData343_APU_GAUSS_5X5, 32, gScenarioKernelData343_APU_GAUSS_5X5, 4},
{20, 26, 3632, 0, 1, gScenarioBufferData344_APU_GAUSS_5X5, 32, gScenarioKernelData344_APU_GAUSS_5X5, 4},
{2, 28, 808, 0, 1, gScenarioBufferData345_APU_GAUSS_5X5, 32, gScenarioKernelData345_APU_GAUSS_5X5, 4},
{4, 28, 1152, 0, 1, gScenarioBufferData346_APU_GAUSS_5X5, 32, gScenarioKernelData346_APU_GAUSS_5X5, 4},
{6, 28, 1496, 0, 1, gScenarioBufferData347_APU_GAUSS_5X5, 32, gScenarioKernelData347_APU_GAUSS_5X5, 4},
{8, 28, 1840, 0, 1, gScenarioBufferData348_APU_GAUSS_5X5, 32, gScenarioKernelData348_APU_GAUSS_5X5, 4},
{10, 28, 2184, 0, 1, gScenarioBufferData349_APU_GAUSS_5X5, 32, gScenarioKernelData349_APU_GAUSS_5X5, 4},
{12, 28, 2528, 0, 1, gScenarioBufferData350_APU_GAUSS_5X5, 32, gScenarioKernelData350_APU_GAUSS_5X5, 4},
{14, 28, 2872, 0, 1, gScenarioBufferData351_APU_GAUSS_5X5, 32, gScenarioKernelData351_APU_GAUSS_5X5, 4},
{16, 28, 3216, 0, 1, gScenarioBufferData352_APU_GAUSS_5X5, 32, gScenarioKernelData352_APU_GAUSS_5X5, 4},
{18, 28, 3560, 0, 1, gScenarioBufferData353_APU_GAUSS_5X5, 32, gScenarioKernelData353_APU_GAUSS_5X5, 4},
{2, 30, 864, 0, 1, gScenarioBufferData354_APU_GAUSS_5X5, 32, gScenarioKernelData354_APU_GAUSS_5X5, 4},
{4, 30, 1232, 0, 1, gScenarioBufferData355_APU_GAUSS_5X5, 32, gScenarioKernelData355_APU_GAUSS_5X5, 4},
{6, 30, 1600, 0, 1, gScenarioBufferData356_APU_GAUSS_5X5, 32, gScenarioKernelData356_APU_GAUSS_5X5, 4},
{8, 30, 1968, 0, 1, gScenarioBufferData357_APU_GAUSS_5X5, 32, gScenarioKernelData357_APU_GAUSS_5X5, 4},
{10, 30, 2336, 0, 1, gScenarioBufferData358_APU_GAUSS_5X5, 32, gScenarioKernelData358_APU_GAUSS_5X5, 4},
{12, 30, 2704, 0, 1, gScenarioBufferData359_APU_GAUSS_5X5, 32, gScenarioKernelData359_APU_GAUSS_5X5, 4},
{14, 30, 3072, 0, 1, gScenarioBufferData360_APU_GAUSS_5X5, 32, gScenarioKernelData360_APU_GAUSS_5X5, 4},
{16, 30, 3440, 0, 1, gScenarioBufferData361_APU_GAUSS_5X5, 32, gScenarioKernelData361_APU_GAUSS_5X5, 4},
{18, 30, 3808, 0, 1, gScenarioBufferData362_APU_GAUSS_5X5, 32, gScenarioKernelData362_APU_GAUSS_5X5, 4},
{2, 32, 920, 0, 1, gScenarioBufferData363_APU_GAUSS_5X5, 32, gScenarioKernelData363_APU_GAUSS_5X5, 4},
{4, 32, 1312, 0, 1, gScenarioBufferData364_APU_GAUSS_5X5, 32, gScenarioKernelData364_APU_GAUSS_5X5, 4},
{6, 32, 1704, 0, 1, gScenarioBufferData365_APU_GAUSS_5X5, 32, gScenarioKernelData365_APU_GAUSS_5X5, 4},
{8, 32, 2096, 0, 1, gScenarioBufferData366_APU_GAUSS_5X5, 32, gScenarioKernelData366_APU_GAUSS_5X5, 4},
{10, 32, 2488, 0, 1, gScenarioBufferData367_APU_GAUSS_5X5, 32, gScenarioKernelData367_APU_GAUSS_5X5, 4},
{12, 32, 2880, 0, 1, gScenarioBufferData368_APU_GAUSS_5X5, 32, gScenarioKernelData368_APU_GAUSS_5X5, 4},
{14, 32, 3272, 0, 1, gScenarioBufferData369_APU_GAUSS_5X5, 32, gScenarioKernelData369_APU_GAUSS_5X5, 4},
{16, 32, 3664, 0, 1, gScenarioBufferData370_APU_GAUSS_5X5, 32, gScenarioKernelData370_APU_GAUSS_5X5, 4}
};

static acf_scenario_list gScenarioList_APU_GAUSS_5X5 = {
371, //number of scenarios
gScenarioArray_APU_GAUSS_5X5};
//**************************************************************

class APU_GAUSS_5X5 : public ACF_Process_APU
{

public:
   APU_GAUSS_5X5(int32_t apex_id = 0) : ACF_Process_APU(apex_id)
   {} 

   int32_t Initialize()
   {
      int32_t lRetVal = 0;

      if (!Initialized()) //initialization steps that only need to occur once
      {
         SetProcessIdentifier("APU_GAUSS_5X5");
         SetApuLoadInfo(APU_GAUSS_5X5_LOAD_SEGMENTS,
                        APU_GAUSS_5X5_LOAD_PMEM, APU_GAUSS_5X5_LOAD_PMEM_SIZE,
                        APU_GAUSS_5X5_LOAD_DMEM, APU_GAUSS_5X5_LOAD_DMEM_SIZE,
                        0, 0); //assuming _LOAD_CMEM does not exist

         lRetVal = NonParametricInitialization();
         if (lRetVal != 0) return lRetVal;

         FlagSpatialDep();

         //***NOTE: the order in which the following ports are added is meaningful; do not change!

         AddPort("INPUT_0", icp::DATATYPE_08U, 1, 1, 2, 1, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2, 2, 0);
         AddPort("OUTPUT_0", icp::DATATYPE_08U, 1, 1, 2, 1, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0);

         CfgScenarios(&gScenarioList_APU_GAUSS_5X5);

         FlagAsInitialized();
      }

      lRetVal = SelectApuConfiguration(mApuCfg, mApexId); //by default mApuCfg = ACF_APU_CFG__APEX0_DEFAULT and mApexId = 0

      return lRetVal;
   }
};

#endif //_ACF_PROCESS_APU_APU_GAUSS_5X5
