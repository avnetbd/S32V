#ifndef _ACF_PROCESS_APU_APU_DRAW_SEGMENTS
#define _ACF_PROCESS_APU_APU_DRAW_SEGMENTS

#include <acf_process_apu.h>
#include <APU_DRAW_SEGMENTS_APU_LOAD.h> //APU load associated with this process
#include <stdint.h>


//SCENARIO LIST*************************************************
static acf_scenario_buffer_data gScenarioBufferData0_APU_DRAW_SEGMENTS[] = {{2, 1, 4, 0, {1, 1, 1, 1}}, {2, 1, 2, 1, {0, 0, 0, 0}}, {2, 1, 4, 0, {1, 1, 2, 2}}};
static acf_scenario_buffer_data gScenarioBufferData1_APU_DRAW_SEGMENTS[] = {{4, 1, 4, 0, {1, 1, 1, 1}}, {4, 1, 2, 1, {0, 0, 0, 0}}, {4, 1, 4, 0, {1, 1, 2, 2}}};
static acf_scenario_buffer_data gScenarioBufferData2_APU_DRAW_SEGMENTS[] = {{6, 1, 4, 0, {1, 1, 1, 1}}, {6, 1, 2, 1, {0, 0, 0, 0}}, {6, 1, 4, 0, {1, 1, 2, 2}}};
static acf_scenario_buffer_data gScenarioBufferData3_APU_DRAW_SEGMENTS[] = {{8, 1, 4, 0, {1, 1, 1, 1}}, {8, 1, 2, 1, {0, 0, 0, 0}}, {8, 1, 4, 0, {1, 1, 2, 2}}};
static acf_scenario_buffer_data gScenarioBufferData4_APU_DRAW_SEGMENTS[] = {{10, 1, 4, 0, {1, 1, 1, 1}}, {10, 1, 2, 1, {0, 0, 0, 0}}, {10, 1, 4, 0, {1, 1, 2, 2}}};
static acf_scenario_buffer_data gScenarioBufferData5_APU_DRAW_SEGMENTS[] = {{12, 1, 4, 0, {1, 1, 1, 1}}, {12, 1, 2, 1, {0, 0, 0, 0}}, {12, 1, 4, 0, {1, 1, 2, 2}}};
static acf_scenario_buffer_data gScenarioBufferData6_APU_DRAW_SEGMENTS[] = {{14, 1, 4, 0, {1, 1, 1, 1}}, {14, 1, 2, 1, {0, 0, 0, 0}}, {14, 1, 4, 0, {1, 1, 2, 2}}};
static acf_scenario_buffer_data gScenarioBufferData7_APU_DRAW_SEGMENTS[] = {{16, 1, 4, 0, {1, 1, 1, 1}}, {16, 1, 2, 1, {0, 0, 0, 0}}, {16, 1, 4, 0, {1, 1, 2, 2}}};
static acf_scenario_buffer_data gScenarioBufferData8_APU_DRAW_SEGMENTS[] = {{18, 1, 4, 0, {1, 1, 1, 1}}, {18, 1, 2, 1, {0, 0, 0, 0}}, {18, 1, 4, 0, {1, 1, 2, 2}}};
static acf_scenario_buffer_data gScenarioBufferData9_APU_DRAW_SEGMENTS[] = {{20, 1, 4, 0, {1, 1, 1, 1}}, {20, 1, 2, 1, {0, 0, 0, 0}}, {20, 1, 4, 0, {1, 1, 2, 2}}};
static acf_scenario_buffer_data gScenarioBufferData10_APU_DRAW_SEGMENTS[] = {{22, 1, 4, 0, {1, 1, 1, 1}}, {22, 1, 2, 1, {0, 0, 0, 0}}, {22, 1, 4, 0, {1, 1, 2, 2}}};
static acf_scenario_buffer_data gScenarioBufferData11_APU_DRAW_SEGMENTS[] = {{24, 1, 4, 0, {1, 1, 1, 1}}, {24, 1, 2, 1, {0, 0, 0, 0}}, {24, 1, 4, 0, {1, 1, 2, 2}}};
static acf_scenario_buffer_data gScenarioBufferData12_APU_DRAW_SEGMENTS[] = {{26, 1, 4, 0, {1, 1, 1, 1}}, {26, 1, 2, 1, {0, 0, 0, 0}}, {26, 1, 4, 0, {1, 1, 2, 2}}};
static acf_scenario_buffer_data gScenarioBufferData13_APU_DRAW_SEGMENTS[] = {{28, 1, 4, 0, {1, 1, 1, 1}}, {28, 1, 2, 1, {0, 0, 0, 0}}, {28, 1, 4, 0, {1, 1, 2, 2}}};
static acf_scenario_buffer_data gScenarioBufferData14_APU_DRAW_SEGMENTS[] = {{30, 1, 4, 0, {1, 1, 1, 1}}, {30, 1, 2, 1, {0, 0, 0, 0}}, {30, 1, 4, 0, {1, 1, 2, 2}}};
static acf_scenario_buffer_data gScenarioBufferData15_APU_DRAW_SEGMENTS[] = {{32, 1, 4, 0, {1, 1, 1, 1}}, {32, 1, 2, 1, {0, 0, 0, 0}}, {32, 1, 4, 0, {1, 1, 2, 2}}};
static acf_scenario_buffer_data gScenarioBufferData16_APU_DRAW_SEGMENTS[] = {{36, 1, 4, 0, {1, 1, 1, 1}}, {36, 1, 2, 1, {0, 0, 0, 0}}, {36, 1, 4, 0, {1, 1, 2, 2}}};
static acf_scenario_buffer_data gScenarioBufferData17_APU_DRAW_SEGMENTS[] = {{40, 1, 4, 0, {1, 1, 1, 1}}, {40, 1, 2, 1, {0, 0, 0, 0}}, {40, 1, 4, 0, {1, 1, 2, 2}}};
static acf_scenario_buffer_data gScenarioBufferData18_APU_DRAW_SEGMENTS[] = {{44, 1, 4, 0, {1, 1, 1, 1}}, {44, 1, 2, 1, {0, 0, 0, 0}}, {44, 1, 4, 0, {1, 1, 2, 2}}};
static acf_scenario_buffer_data gScenarioBufferData19_APU_DRAW_SEGMENTS[] = {{48, 1, 4, 0, {1, 1, 1, 1}}, {48, 1, 2, 1, {0, 0, 0, 0}}, {48, 1, 4, 0, {1, 1, 2, 2}}};
static acf_scenario_buffer_data gScenarioBufferData20_APU_DRAW_SEGMENTS[] = {{52, 1, 4, 0, {1, 1, 1, 1}}, {52, 1, 2, 1, {0, 0, 0, 0}}, {52, 1, 4, 0, {1, 1, 2, 2}}};
static acf_scenario_buffer_data gScenarioBufferData21_APU_DRAW_SEGMENTS[] = {{56, 1, 4, 0, {1, 1, 1, 1}}, {56, 1, 2, 1, {0, 0, 0, 0}}, {56, 1, 4, 0, {1, 1, 2, 2}}};
static acf_scenario_buffer_data gScenarioBufferData22_APU_DRAW_SEGMENTS[] = {{60, 1, 4, 0, {1, 1, 1, 1}}, {60, 1, 2, 1, {0, 0, 0, 0}}, {60, 1, 4, 0, {1, 1, 2, 2}}};
static acf_scenario_buffer_data gScenarioBufferData23_APU_DRAW_SEGMENTS[] = {{64, 1, 4, 0, {1, 1, 1, 1}}, {64, 1, 2, 1, {0, 0, 0, 0}}, {64, 1, 4, 0, {1, 1, 2, 2}}};
static acf_scenario_buffer_data gScenarioBufferData24_APU_DRAW_SEGMENTS[] = {{2, 2, 4, 0, {1, 1, 1, 1}}, {2, 2, 2, 1, {0, 0, 0, 0}}, {2, 2, 4, 0, {1, 1, 2, 2}}};
static acf_scenario_buffer_data gScenarioBufferData25_APU_DRAW_SEGMENTS[] = {{4, 2, 4, 0, {1, 1, 1, 1}}, {4, 2, 2, 1, {0, 0, 0, 0}}, {4, 2, 4, 0, {1, 1, 2, 2}}};
static acf_scenario_buffer_data gScenarioBufferData26_APU_DRAW_SEGMENTS[] = {{6, 2, 4, 0, {1, 1, 1, 1}}, {6, 2, 2, 1, {0, 0, 0, 0}}, {6, 2, 4, 0, {1, 1, 2, 2}}};
static acf_scenario_buffer_data gScenarioBufferData27_APU_DRAW_SEGMENTS[] = {{8, 2, 4, 0, {1, 1, 1, 1}}, {8, 2, 2, 1, {0, 0, 0, 0}}, {8, 2, 4, 0, {1, 1, 2, 2}}};
static acf_scenario_buffer_data gScenarioBufferData28_APU_DRAW_SEGMENTS[] = {{10, 2, 4, 0, {1, 1, 1, 1}}, {10, 2, 2, 1, {0, 0, 0, 0}}, {10, 2, 4, 0, {1, 1, 2, 2}}};
static acf_scenario_buffer_data gScenarioBufferData29_APU_DRAW_SEGMENTS[] = {{12, 2, 4, 0, {1, 1, 1, 1}}, {12, 2, 2, 1, {0, 0, 0, 0}}, {12, 2, 4, 0, {1, 1, 2, 2}}};
static acf_scenario_buffer_data gScenarioBufferData30_APU_DRAW_SEGMENTS[] = {{14, 2, 4, 0, {1, 1, 1, 1}}, {14, 2, 2, 1, {0, 0, 0, 0}}, {14, 2, 4, 0, {1, 1, 2, 2}}};
static acf_scenario_buffer_data gScenarioBufferData31_APU_DRAW_SEGMENTS[] = {{16, 2, 4, 0, {1, 1, 1, 1}}, {16, 2, 2, 1, {0, 0, 0, 0}}, {16, 2, 4, 0, {1, 1, 2, 2}}};
static acf_scenario_buffer_data gScenarioBufferData32_APU_DRAW_SEGMENTS[] = {{18, 2, 4, 0, {1, 1, 1, 1}}, {18, 2, 2, 1, {0, 0, 0, 0}}, {18, 2, 4, 0, {1, 1, 2, 2}}};
static acf_scenario_buffer_data gScenarioBufferData33_APU_DRAW_SEGMENTS[] = {{20, 2, 4, 0, {1, 1, 1, 1}}, {20, 2, 2, 1, {0, 0, 0, 0}}, {20, 2, 4, 0, {1, 1, 2, 2}}};
static acf_scenario_buffer_data gScenarioBufferData34_APU_DRAW_SEGMENTS[] = {{22, 2, 4, 0, {1, 1, 1, 1}}, {22, 2, 2, 1, {0, 0, 0, 0}}, {22, 2, 4, 0, {1, 1, 2, 2}}};
static acf_scenario_buffer_data gScenarioBufferData35_APU_DRAW_SEGMENTS[] = {{24, 2, 4, 0, {1, 1, 1, 1}}, {24, 2, 2, 1, {0, 0, 0, 0}}, {24, 2, 4, 0, {1, 1, 2, 2}}};
static acf_scenario_buffer_data gScenarioBufferData36_APU_DRAW_SEGMENTS[] = {{26, 2, 4, 0, {1, 1, 1, 1}}, {26, 2, 2, 1, {0, 0, 0, 0}}, {26, 2, 4, 0, {1, 1, 2, 2}}};
static acf_scenario_buffer_data gScenarioBufferData37_APU_DRAW_SEGMENTS[] = {{28, 2, 4, 0, {1, 1, 1, 1}}, {28, 2, 2, 1, {0, 0, 0, 0}}, {28, 2, 4, 0, {1, 1, 2, 2}}};
static acf_scenario_buffer_data gScenarioBufferData38_APU_DRAW_SEGMENTS[] = {{30, 2, 4, 0, {1, 1, 1, 1}}, {30, 2, 2, 1, {0, 0, 0, 0}}, {30, 2, 4, 0, {1, 1, 2, 2}}};
static acf_scenario_buffer_data gScenarioBufferData39_APU_DRAW_SEGMENTS[] = {{32, 2, 4, 0, {1, 1, 1, 1}}, {32, 2, 2, 1, {0, 0, 0, 0}}, {32, 2, 4, 0, {1, 1, 2, 2}}};
static acf_scenario_buffer_data gScenarioBufferData40_APU_DRAW_SEGMENTS[] = {{36, 2, 4, 0, {1, 1, 1, 1}}, {36, 2, 2, 1, {0, 0, 0, 0}}, {36, 2, 4, 0, {1, 1, 2, 2}}};
static acf_scenario_buffer_data gScenarioBufferData41_APU_DRAW_SEGMENTS[] = {{40, 2, 4, 0, {1, 1, 1, 1}}, {40, 2, 2, 1, {0, 0, 0, 0}}, {40, 2, 4, 0, {1, 1, 2, 2}}};
static acf_scenario_buffer_data gScenarioBufferData42_APU_DRAW_SEGMENTS[] = {{44, 2, 4, 0, {1, 1, 1, 1}}, {44, 2, 2, 1, {0, 0, 0, 0}}, {44, 2, 4, 0, {1, 1, 2, 2}}};
static acf_scenario_buffer_data gScenarioBufferData43_APU_DRAW_SEGMENTS[] = {{48, 2, 4, 0, {1, 1, 1, 1}}, {48, 2, 2, 1, {0, 0, 0, 0}}, {48, 2, 4, 0, {1, 1, 2, 2}}};
static acf_scenario_buffer_data gScenarioBufferData44_APU_DRAW_SEGMENTS[] = {{52, 2, 4, 0, {1, 1, 1, 1}}, {52, 2, 2, 1, {0, 0, 0, 0}}, {52, 2, 4, 0, {1, 1, 2, 2}}};
static acf_scenario_buffer_data gScenarioBufferData45_APU_DRAW_SEGMENTS[] = {{56, 2, 4, 0, {1, 1, 1, 1}}, {56, 2, 2, 1, {0, 0, 0, 0}}, {56, 2, 4, 0, {1, 1, 2, 2}}};
static acf_scenario_buffer_data gScenarioBufferData46_APU_DRAW_SEGMENTS[] = {{60, 2, 4, 0, {1, 1, 1, 1}}, {60, 2, 2, 1, {0, 0, 0, 0}}, {60, 2, 4, 0, {1, 1, 2, 2}}};
static acf_scenario_buffer_data gScenarioBufferData47_APU_DRAW_SEGMENTS[] = {{64, 2, 4, 0, {1, 1, 1, 1}}, {64, 2, 2, 1, {0, 0, 0, 0}}, {64, 2, 4, 0, {1, 1, 2, 2}}};
static acf_scenario_buffer_data gScenarioBufferData48_APU_DRAW_SEGMENTS[] = {{2, 3, 4, 0, {1, 1, 1, 1}}, {2, 3, 2, 1, {0, 0, 0, 0}}, {2, 3, 4, 0, {1, 1, 2, 2}}};
static acf_scenario_buffer_data gScenarioBufferData49_APU_DRAW_SEGMENTS[] = {{4, 3, 4, 0, {1, 1, 1, 1}}, {4, 3, 2, 1, {0, 0, 0, 0}}, {4, 3, 4, 0, {1, 1, 2, 2}}};
static acf_scenario_buffer_data gScenarioBufferData50_APU_DRAW_SEGMENTS[] = {{6, 3, 4, 0, {1, 1, 1, 1}}, {6, 3, 2, 1, {0, 0, 0, 0}}, {6, 3, 4, 0, {1, 1, 2, 2}}};
static acf_scenario_buffer_data gScenarioBufferData51_APU_DRAW_SEGMENTS[] = {{8, 3, 4, 0, {1, 1, 1, 1}}, {8, 3, 2, 1, {0, 0, 0, 0}}, {8, 3, 4, 0, {1, 1, 2, 2}}};
static acf_scenario_buffer_data gScenarioBufferData52_APU_DRAW_SEGMENTS[] = {{10, 3, 4, 0, {1, 1, 1, 1}}, {10, 3, 2, 1, {0, 0, 0, 0}}, {10, 3, 4, 0, {1, 1, 2, 2}}};
static acf_scenario_buffer_data gScenarioBufferData53_APU_DRAW_SEGMENTS[] = {{12, 3, 4, 0, {1, 1, 1, 1}}, {12, 3, 2, 1, {0, 0, 0, 0}}, {12, 3, 4, 0, {1, 1, 2, 2}}};
static acf_scenario_buffer_data gScenarioBufferData54_APU_DRAW_SEGMENTS[] = {{14, 3, 4, 0, {1, 1, 1, 1}}, {14, 3, 2, 1, {0, 0, 0, 0}}, {14, 3, 4, 0, {1, 1, 2, 2}}};
static acf_scenario_buffer_data gScenarioBufferData55_APU_DRAW_SEGMENTS[] = {{16, 3, 4, 0, {1, 1, 1, 1}}, {16, 3, 2, 1, {0, 0, 0, 0}}, {16, 3, 4, 0, {1, 1, 2, 2}}};
static acf_scenario_buffer_data gScenarioBufferData56_APU_DRAW_SEGMENTS[] = {{18, 3, 4, 0, {1, 1, 1, 1}}, {18, 3, 2, 1, {0, 0, 0, 0}}, {18, 3, 4, 0, {1, 1, 2, 2}}};
static acf_scenario_buffer_data gScenarioBufferData57_APU_DRAW_SEGMENTS[] = {{20, 3, 4, 0, {1, 1, 1, 1}}, {20, 3, 2, 1, {0, 0, 0, 0}}, {20, 3, 4, 0, {1, 1, 2, 2}}};
static acf_scenario_buffer_data gScenarioBufferData58_APU_DRAW_SEGMENTS[] = {{22, 3, 4, 0, {1, 1, 1, 1}}, {22, 3, 2, 1, {0, 0, 0, 0}}, {22, 3, 4, 0, {1, 1, 2, 2}}};
static acf_scenario_buffer_data gScenarioBufferData59_APU_DRAW_SEGMENTS[] = {{24, 3, 4, 0, {1, 1, 1, 1}}, {24, 3, 2, 1, {0, 0, 0, 0}}, {24, 3, 4, 0, {1, 1, 2, 2}}};
static acf_scenario_buffer_data gScenarioBufferData60_APU_DRAW_SEGMENTS[] = {{26, 3, 4, 0, {1, 1, 1, 1}}, {26, 3, 2, 1, {0, 0, 0, 0}}, {26, 3, 4, 0, {1, 1, 2, 2}}};
static acf_scenario_buffer_data gScenarioBufferData61_APU_DRAW_SEGMENTS[] = {{28, 3, 4, 0, {1, 1, 1, 1}}, {28, 3, 2, 1, {0, 0, 0, 0}}, {28, 3, 4, 0, {1, 1, 2, 2}}};
static acf_scenario_buffer_data gScenarioBufferData62_APU_DRAW_SEGMENTS[] = {{30, 3, 4, 0, {1, 1, 1, 1}}, {30, 3, 2, 1, {0, 0, 0, 0}}, {30, 3, 4, 0, {1, 1, 2, 2}}};
static acf_scenario_buffer_data gScenarioBufferData63_APU_DRAW_SEGMENTS[] = {{32, 3, 4, 0, {1, 1, 1, 1}}, {32, 3, 2, 1, {0, 0, 0, 0}}, {32, 3, 4, 0, {1, 1, 2, 2}}};
static acf_scenario_buffer_data gScenarioBufferData64_APU_DRAW_SEGMENTS[] = {{36, 3, 4, 0, {1, 1, 1, 1}}, {36, 3, 2, 1, {0, 0, 0, 0}}, {36, 3, 4, 0, {1, 1, 2, 2}}};
static acf_scenario_buffer_data gScenarioBufferData65_APU_DRAW_SEGMENTS[] = {{40, 3, 4, 0, {1, 1, 1, 1}}, {40, 3, 2, 1, {0, 0, 0, 0}}, {40, 3, 4, 0, {1, 1, 2, 2}}};
static acf_scenario_buffer_data gScenarioBufferData66_APU_DRAW_SEGMENTS[] = {{44, 3, 4, 0, {1, 1, 1, 1}}, {44, 3, 2, 1, {0, 0, 0, 0}}, {44, 3, 4, 0, {1, 1, 2, 2}}};
static acf_scenario_buffer_data gScenarioBufferData67_APU_DRAW_SEGMENTS[] = {{48, 3, 4, 0, {1, 1, 1, 1}}, {48, 3, 2, 1, {0, 0, 0, 0}}, {48, 3, 4, 0, {1, 1, 2, 2}}};
static acf_scenario_buffer_data gScenarioBufferData68_APU_DRAW_SEGMENTS[] = {{52, 3, 4, 0, {1, 1, 1, 1}}, {52, 3, 2, 1, {0, 0, 0, 0}}, {52, 3, 4, 0, {1, 1, 2, 2}}};
static acf_scenario_buffer_data gScenarioBufferData69_APU_DRAW_SEGMENTS[] = {{56, 3, 4, 0, {1, 1, 1, 1}}, {56, 3, 2, 1, {0, 0, 0, 0}}, {56, 3, 4, 0, {1, 1, 2, 2}}};
static acf_scenario_buffer_data gScenarioBufferData70_APU_DRAW_SEGMENTS[] = {{60, 3, 4, 0, {1, 1, 1, 1}}, {60, 3, 2, 1, {0, 0, 0, 0}}, {60, 3, 4, 0, {1, 1, 2, 2}}};
static acf_scenario_buffer_data gScenarioBufferData71_APU_DRAW_SEGMENTS[] = {{64, 3, 4, 0, {1, 1, 1, 1}}, {64, 3, 2, 1, {0, 0, 0, 0}}, {64, 3, 4, 0, {1, 1, 2, 2}}};
static acf_scenario_buffer_data gScenarioBufferData72_APU_DRAW_SEGMENTS[] = {{2, 4, 4, 0, {1, 1, 1, 1}}, {2, 4, 2, 1, {0, 0, 0, 0}}, {2, 4, 4, 0, {1, 1, 2, 2}}};
static acf_scenario_buffer_data gScenarioBufferData73_APU_DRAW_SEGMENTS[] = {{4, 4, 4, 0, {1, 1, 1, 1}}, {4, 4, 2, 1, {0, 0, 0, 0}}, {4, 4, 4, 0, {1, 1, 2, 2}}};
static acf_scenario_buffer_data gScenarioBufferData74_APU_DRAW_SEGMENTS[] = {{6, 4, 4, 0, {1, 1, 1, 1}}, {6, 4, 2, 1, {0, 0, 0, 0}}, {6, 4, 4, 0, {1, 1, 2, 2}}};
static acf_scenario_buffer_data gScenarioBufferData75_APU_DRAW_SEGMENTS[] = {{8, 4, 4, 0, {1, 1, 1, 1}}, {8, 4, 2, 1, {0, 0, 0, 0}}, {8, 4, 4, 0, {1, 1, 2, 2}}};
static acf_scenario_buffer_data gScenarioBufferData76_APU_DRAW_SEGMENTS[] = {{10, 4, 4, 0, {1, 1, 1, 1}}, {10, 4, 2, 1, {0, 0, 0, 0}}, {10, 4, 4, 0, {1, 1, 2, 2}}};
static acf_scenario_buffer_data gScenarioBufferData77_APU_DRAW_SEGMENTS[] = {{12, 4, 4, 0, {1, 1, 1, 1}}, {12, 4, 2, 1, {0, 0, 0, 0}}, {12, 4, 4, 0, {1, 1, 2, 2}}};
static acf_scenario_buffer_data gScenarioBufferData78_APU_DRAW_SEGMENTS[] = {{14, 4, 4, 0, {1, 1, 1, 1}}, {14, 4, 2, 1, {0, 0, 0, 0}}, {14, 4, 4, 0, {1, 1, 2, 2}}};
static acf_scenario_buffer_data gScenarioBufferData79_APU_DRAW_SEGMENTS[] = {{16, 4, 4, 0, {1, 1, 1, 1}}, {16, 4, 2, 1, {0, 0, 0, 0}}, {16, 4, 4, 0, {1, 1, 2, 2}}};
static acf_scenario_buffer_data gScenarioBufferData80_APU_DRAW_SEGMENTS[] = {{18, 4, 4, 0, {1, 1, 1, 1}}, {18, 4, 2, 1, {0, 0, 0, 0}}, {18, 4, 4, 0, {1, 1, 2, 2}}};
static acf_scenario_buffer_data gScenarioBufferData81_APU_DRAW_SEGMENTS[] = {{20, 4, 4, 0, {1, 1, 1, 1}}, {20, 4, 2, 1, {0, 0, 0, 0}}, {20, 4, 4, 0, {1, 1, 2, 2}}};
static acf_scenario_buffer_data gScenarioBufferData82_APU_DRAW_SEGMENTS[] = {{22, 4, 4, 0, {1, 1, 1, 1}}, {22, 4, 2, 1, {0, 0, 0, 0}}, {22, 4, 4, 0, {1, 1, 2, 2}}};
static acf_scenario_buffer_data gScenarioBufferData83_APU_DRAW_SEGMENTS[] = {{24, 4, 4, 0, {1, 1, 1, 1}}, {24, 4, 2, 1, {0, 0, 0, 0}}, {24, 4, 4, 0, {1, 1, 2, 2}}};
static acf_scenario_buffer_data gScenarioBufferData84_APU_DRAW_SEGMENTS[] = {{26, 4, 4, 0, {1, 1, 1, 1}}, {26, 4, 2, 1, {0, 0, 0, 0}}, {26, 4, 4, 0, {1, 1, 2, 2}}};
static acf_scenario_buffer_data gScenarioBufferData85_APU_DRAW_SEGMENTS[] = {{28, 4, 4, 0, {1, 1, 1, 1}}, {28, 4, 2, 1, {0, 0, 0, 0}}, {28, 4, 4, 0, {1, 1, 2, 2}}};
static acf_scenario_buffer_data gScenarioBufferData86_APU_DRAW_SEGMENTS[] = {{30, 4, 4, 0, {1, 1, 1, 1}}, {30, 4, 2, 1, {0, 0, 0, 0}}, {30, 4, 4, 0, {1, 1, 2, 2}}};
static acf_scenario_buffer_data gScenarioBufferData87_APU_DRAW_SEGMENTS[] = {{32, 4, 4, 0, {1, 1, 1, 1}}, {32, 4, 2, 1, {0, 0, 0, 0}}, {32, 4, 4, 0, {1, 1, 2, 2}}};
static acf_scenario_buffer_data gScenarioBufferData88_APU_DRAW_SEGMENTS[] = {{36, 4, 4, 0, {1, 1, 1, 1}}, {36, 4, 2, 1, {0, 0, 0, 0}}, {36, 4, 4, 0, {1, 1, 2, 2}}};
static acf_scenario_buffer_data gScenarioBufferData89_APU_DRAW_SEGMENTS[] = {{40, 4, 4, 0, {1, 1, 1, 1}}, {40, 4, 2, 1, {0, 0, 0, 0}}, {40, 4, 4, 0, {1, 1, 2, 2}}};
static acf_scenario_buffer_data gScenarioBufferData90_APU_DRAW_SEGMENTS[] = {{44, 4, 4, 0, {1, 1, 1, 1}}, {44, 4, 2, 1, {0, 0, 0, 0}}, {44, 4, 4, 0, {1, 1, 2, 2}}};
static acf_scenario_buffer_data gScenarioBufferData91_APU_DRAW_SEGMENTS[] = {{48, 4, 4, 0, {1, 1, 1, 1}}, {48, 4, 2, 1, {0, 0, 0, 0}}, {48, 4, 4, 0, {1, 1, 2, 2}}};
static acf_scenario_buffer_data gScenarioBufferData92_APU_DRAW_SEGMENTS[] = {{52, 4, 4, 0, {1, 1, 1, 1}}, {52, 4, 2, 1, {0, 0, 0, 0}}, {52, 4, 4, 0, {1, 1, 2, 2}}};
static acf_scenario_buffer_data gScenarioBufferData93_APU_DRAW_SEGMENTS[] = {{56, 4, 4, 0, {1, 1, 1, 1}}, {56, 4, 2, 1, {0, 0, 0, 0}}, {56, 4, 4, 0, {1, 1, 2, 2}}};
static acf_scenario_buffer_data gScenarioBufferData94_APU_DRAW_SEGMENTS[] = {{2, 5, 4, 0, {1, 1, 1, 1}}, {2, 5, 2, 1, {0, 0, 0, 0}}, {2, 5, 4, 0, {1, 1, 2, 2}}};
static acf_scenario_buffer_data gScenarioBufferData95_APU_DRAW_SEGMENTS[] = {{4, 5, 4, 0, {1, 1, 1, 1}}, {4, 5, 2, 1, {0, 0, 0, 0}}, {4, 5, 4, 0, {1, 1, 2, 2}}};
static acf_scenario_buffer_data gScenarioBufferData96_APU_DRAW_SEGMENTS[] = {{6, 5, 4, 0, {1, 1, 1, 1}}, {6, 5, 2, 1, {0, 0, 0, 0}}, {6, 5, 4, 0, {1, 1, 2, 2}}};
static acf_scenario_buffer_data gScenarioBufferData97_APU_DRAW_SEGMENTS[] = {{8, 5, 4, 0, {1, 1, 1, 1}}, {8, 5, 2, 1, {0, 0, 0, 0}}, {8, 5, 4, 0, {1, 1, 2, 2}}};
static acf_scenario_buffer_data gScenarioBufferData98_APU_DRAW_SEGMENTS[] = {{10, 5, 4, 0, {1, 1, 1, 1}}, {10, 5, 2, 1, {0, 0, 0, 0}}, {10, 5, 4, 0, {1, 1, 2, 2}}};
static acf_scenario_buffer_data gScenarioBufferData99_APU_DRAW_SEGMENTS[] = {{12, 5, 4, 0, {1, 1, 1, 1}}, {12, 5, 2, 1, {0, 0, 0, 0}}, {12, 5, 4, 0, {1, 1, 2, 2}}};
static acf_scenario_buffer_data gScenarioBufferData100_APU_DRAW_SEGMENTS[] = {{14, 5, 4, 0, {1, 1, 1, 1}}, {14, 5, 2, 1, {0, 0, 0, 0}}, {14, 5, 4, 0, {1, 1, 2, 2}}};
static acf_scenario_buffer_data gScenarioBufferData101_APU_DRAW_SEGMENTS[] = {{16, 5, 4, 0, {1, 1, 1, 1}}, {16, 5, 2, 1, {0, 0, 0, 0}}, {16, 5, 4, 0, {1, 1, 2, 2}}};
static acf_scenario_buffer_data gScenarioBufferData102_APU_DRAW_SEGMENTS[] = {{18, 5, 4, 0, {1, 1, 1, 1}}, {18, 5, 2, 1, {0, 0, 0, 0}}, {18, 5, 4, 0, {1, 1, 2, 2}}};
static acf_scenario_buffer_data gScenarioBufferData103_APU_DRAW_SEGMENTS[] = {{20, 5, 4, 0, {1, 1, 1, 1}}, {20, 5, 2, 1, {0, 0, 0, 0}}, {20, 5, 4, 0, {1, 1, 2, 2}}};
static acf_scenario_buffer_data gScenarioBufferData104_APU_DRAW_SEGMENTS[] = {{22, 5, 4, 0, {1, 1, 1, 1}}, {22, 5, 2, 1, {0, 0, 0, 0}}, {22, 5, 4, 0, {1, 1, 2, 2}}};
static acf_scenario_buffer_data gScenarioBufferData105_APU_DRAW_SEGMENTS[] = {{24, 5, 4, 0, {1, 1, 1, 1}}, {24, 5, 2, 1, {0, 0, 0, 0}}, {24, 5, 4, 0, {1, 1, 2, 2}}};
static acf_scenario_buffer_data gScenarioBufferData106_APU_DRAW_SEGMENTS[] = {{26, 5, 4, 0, {1, 1, 1, 1}}, {26, 5, 2, 1, {0, 0, 0, 0}}, {26, 5, 4, 0, {1, 1, 2, 2}}};
static acf_scenario_buffer_data gScenarioBufferData107_APU_DRAW_SEGMENTS[] = {{28, 5, 4, 0, {1, 1, 1, 1}}, {28, 5, 2, 1, {0, 0, 0, 0}}, {28, 5, 4, 0, {1, 1, 2, 2}}};
static acf_scenario_buffer_data gScenarioBufferData108_APU_DRAW_SEGMENTS[] = {{30, 5, 4, 0, {1, 1, 1, 1}}, {30, 5, 2, 1, {0, 0, 0, 0}}, {30, 5, 4, 0, {1, 1, 2, 2}}};
static acf_scenario_buffer_data gScenarioBufferData109_APU_DRAW_SEGMENTS[] = {{32, 5, 4, 0, {1, 1, 1, 1}}, {32, 5, 2, 1, {0, 0, 0, 0}}, {32, 5, 4, 0, {1, 1, 2, 2}}};
static acf_scenario_buffer_data gScenarioBufferData110_APU_DRAW_SEGMENTS[] = {{36, 5, 4, 0, {1, 1, 1, 1}}, {36, 5, 2, 1, {0, 0, 0, 0}}, {36, 5, 4, 0, {1, 1, 2, 2}}};
static acf_scenario_buffer_data gScenarioBufferData111_APU_DRAW_SEGMENTS[] = {{40, 5, 4, 0, {1, 1, 1, 1}}, {40, 5, 2, 1, {0, 0, 0, 0}}, {40, 5, 4, 0, {1, 1, 2, 2}}};
static acf_scenario_buffer_data gScenarioBufferData112_APU_DRAW_SEGMENTS[] = {{44, 5, 4, 0, {1, 1, 1, 1}}, {44, 5, 2, 1, {0, 0, 0, 0}}, {44, 5, 4, 0, {1, 1, 2, 2}}};
static acf_scenario_buffer_data gScenarioBufferData113_APU_DRAW_SEGMENTS[] = {{48, 5, 4, 0, {1, 1, 1, 1}}, {48, 5, 2, 1, {0, 0, 0, 0}}, {48, 5, 4, 0, {1, 1, 2, 2}}};
static acf_scenario_buffer_data gScenarioBufferData114_APU_DRAW_SEGMENTS[] = {{2, 6, 4, 0, {1, 1, 1, 1}}, {2, 6, 2, 1, {0, 0, 0, 0}}, {2, 6, 4, 0, {1, 1, 2, 2}}};
static acf_scenario_buffer_data gScenarioBufferData115_APU_DRAW_SEGMENTS[] = {{4, 6, 4, 0, {1, 1, 1, 1}}, {4, 6, 2, 1, {0, 0, 0, 0}}, {4, 6, 4, 0, {1, 1, 2, 2}}};
static acf_scenario_buffer_data gScenarioBufferData116_APU_DRAW_SEGMENTS[] = {{6, 6, 4, 0, {1, 1, 1, 1}}, {6, 6, 2, 1, {0, 0, 0, 0}}, {6, 6, 4, 0, {1, 1, 2, 2}}};
static acf_scenario_buffer_data gScenarioBufferData117_APU_DRAW_SEGMENTS[] = {{8, 6, 4, 0, {1, 1, 1, 1}}, {8, 6, 2, 1, {0, 0, 0, 0}}, {8, 6, 4, 0, {1, 1, 2, 2}}};
static acf_scenario_buffer_data gScenarioBufferData118_APU_DRAW_SEGMENTS[] = {{10, 6, 4, 0, {1, 1, 1, 1}}, {10, 6, 2, 1, {0, 0, 0, 0}}, {10, 6, 4, 0, {1, 1, 2, 2}}};
static acf_scenario_buffer_data gScenarioBufferData119_APU_DRAW_SEGMENTS[] = {{12, 6, 4, 0, {1, 1, 1, 1}}, {12, 6, 2, 1, {0, 0, 0, 0}}, {12, 6, 4, 0, {1, 1, 2, 2}}};
static acf_scenario_buffer_data gScenarioBufferData120_APU_DRAW_SEGMENTS[] = {{14, 6, 4, 0, {1, 1, 1, 1}}, {14, 6, 2, 1, {0, 0, 0, 0}}, {14, 6, 4, 0, {1, 1, 2, 2}}};
static acf_scenario_buffer_data gScenarioBufferData121_APU_DRAW_SEGMENTS[] = {{16, 6, 4, 0, {1, 1, 1, 1}}, {16, 6, 2, 1, {0, 0, 0, 0}}, {16, 6, 4, 0, {1, 1, 2, 2}}};
static acf_scenario_buffer_data gScenarioBufferData122_APU_DRAW_SEGMENTS[] = {{18, 6, 4, 0, {1, 1, 1, 1}}, {18, 6, 2, 1, {0, 0, 0, 0}}, {18, 6, 4, 0, {1, 1, 2, 2}}};
static acf_scenario_buffer_data gScenarioBufferData123_APU_DRAW_SEGMENTS[] = {{20, 6, 4, 0, {1, 1, 1, 1}}, {20, 6, 2, 1, {0, 0, 0, 0}}, {20, 6, 4, 0, {1, 1, 2, 2}}};
static acf_scenario_buffer_data gScenarioBufferData124_APU_DRAW_SEGMENTS[] = {{22, 6, 4, 0, {1, 1, 1, 1}}, {22, 6, 2, 1, {0, 0, 0, 0}}, {22, 6, 4, 0, {1, 1, 2, 2}}};
static acf_scenario_buffer_data gScenarioBufferData125_APU_DRAW_SEGMENTS[] = {{24, 6, 4, 0, {1, 1, 1, 1}}, {24, 6, 2, 1, {0, 0, 0, 0}}, {24, 6, 4, 0, {1, 1, 2, 2}}};
static acf_scenario_buffer_data gScenarioBufferData126_APU_DRAW_SEGMENTS[] = {{26, 6, 4, 0, {1, 1, 1, 1}}, {26, 6, 2, 1, {0, 0, 0, 0}}, {26, 6, 4, 0, {1, 1, 2, 2}}};
static acf_scenario_buffer_data gScenarioBufferData127_APU_DRAW_SEGMENTS[] = {{28, 6, 4, 0, {1, 1, 1, 1}}, {28, 6, 2, 1, {0, 0, 0, 0}}, {28, 6, 4, 0, {1, 1, 2, 2}}};
static acf_scenario_buffer_data gScenarioBufferData128_APU_DRAW_SEGMENTS[] = {{30, 6, 4, 0, {1, 1, 1, 1}}, {30, 6, 2, 1, {0, 0, 0, 0}}, {30, 6, 4, 0, {1, 1, 2, 2}}};
static acf_scenario_buffer_data gScenarioBufferData129_APU_DRAW_SEGMENTS[] = {{32, 6, 4, 0, {1, 1, 1, 1}}, {32, 6, 2, 1, {0, 0, 0, 0}}, {32, 6, 4, 0, {1, 1, 2, 2}}};
static acf_scenario_buffer_data gScenarioBufferData130_APU_DRAW_SEGMENTS[] = {{36, 6, 4, 0, {1, 1, 1, 1}}, {36, 6, 2, 1, {0, 0, 0, 0}}, {36, 6, 4, 0, {1, 1, 2, 2}}};
static acf_scenario_buffer_data gScenarioBufferData131_APU_DRAW_SEGMENTS[] = {{40, 6, 4, 0, {1, 1, 1, 1}}, {40, 6, 2, 1, {0, 0, 0, 0}}, {40, 6, 4, 0, {1, 1, 2, 2}}};
static acf_scenario_buffer_data gScenarioBufferData132_APU_DRAW_SEGMENTS[] = {{2, 8, 4, 0, {1, 1, 1, 1}}, {2, 8, 2, 1, {0, 0, 0, 0}}, {2, 8, 4, 0, {1, 1, 2, 2}}};
static acf_scenario_buffer_data gScenarioBufferData133_APU_DRAW_SEGMENTS[] = {{4, 8, 4, 0, {1, 1, 1, 1}}, {4, 8, 2, 1, {0, 0, 0, 0}}, {4, 8, 4, 0, {1, 1, 2, 2}}};
static acf_scenario_buffer_data gScenarioBufferData134_APU_DRAW_SEGMENTS[] = {{6, 8, 4, 0, {1, 1, 1, 1}}, {6, 8, 2, 1, {0, 0, 0, 0}}, {6, 8, 4, 0, {1, 1, 2, 2}}};
static acf_scenario_buffer_data gScenarioBufferData135_APU_DRAW_SEGMENTS[] = {{8, 8, 4, 0, {1, 1, 1, 1}}, {8, 8, 2, 1, {0, 0, 0, 0}}, {8, 8, 4, 0, {1, 1, 2, 2}}};
static acf_scenario_buffer_data gScenarioBufferData136_APU_DRAW_SEGMENTS[] = {{10, 8, 4, 0, {1, 1, 1, 1}}, {10, 8, 2, 1, {0, 0, 0, 0}}, {10, 8, 4, 0, {1, 1, 2, 2}}};
static acf_scenario_buffer_data gScenarioBufferData137_APU_DRAW_SEGMENTS[] = {{12, 8, 4, 0, {1, 1, 1, 1}}, {12, 8, 2, 1, {0, 0, 0, 0}}, {12, 8, 4, 0, {1, 1, 2, 2}}};
static acf_scenario_buffer_data gScenarioBufferData138_APU_DRAW_SEGMENTS[] = {{14, 8, 4, 0, {1, 1, 1, 1}}, {14, 8, 2, 1, {0, 0, 0, 0}}, {14, 8, 4, 0, {1, 1, 2, 2}}};
static acf_scenario_buffer_data gScenarioBufferData139_APU_DRAW_SEGMENTS[] = {{16, 8, 4, 0, {1, 1, 1, 1}}, {16, 8, 2, 1, {0, 0, 0, 0}}, {16, 8, 4, 0, {1, 1, 2, 2}}};
static acf_scenario_buffer_data gScenarioBufferData140_APU_DRAW_SEGMENTS[] = {{18, 8, 4, 0, {1, 1, 1, 1}}, {18, 8, 2, 1, {0, 0, 0, 0}}, {18, 8, 4, 0, {1, 1, 2, 2}}};
static acf_scenario_buffer_data gScenarioBufferData141_APU_DRAW_SEGMENTS[] = {{20, 8, 4, 0, {1, 1, 1, 1}}, {20, 8, 2, 1, {0, 0, 0, 0}}, {20, 8, 4, 0, {1, 1, 2, 2}}};
static acf_scenario_buffer_data gScenarioBufferData142_APU_DRAW_SEGMENTS[] = {{22, 8, 4, 0, {1, 1, 1, 1}}, {22, 8, 2, 1, {0, 0, 0, 0}}, {22, 8, 4, 0, {1, 1, 2, 2}}};
static acf_scenario_buffer_data gScenarioBufferData143_APU_DRAW_SEGMENTS[] = {{24, 8, 4, 0, {1, 1, 1, 1}}, {24, 8, 2, 1, {0, 0, 0, 0}}, {24, 8, 4, 0, {1, 1, 2, 2}}};
static acf_scenario_buffer_data gScenarioBufferData144_APU_DRAW_SEGMENTS[] = {{26, 8, 4, 0, {1, 1, 1, 1}}, {26, 8, 2, 1, {0, 0, 0, 0}}, {26, 8, 4, 0, {1, 1, 2, 2}}};
static acf_scenario_buffer_data gScenarioBufferData145_APU_DRAW_SEGMENTS[] = {{28, 8, 4, 0, {1, 1, 1, 1}}, {28, 8, 2, 1, {0, 0, 0, 0}}, {28, 8, 4, 0, {1, 1, 2, 2}}};
static acf_scenario_buffer_data gScenarioBufferData146_APU_DRAW_SEGMENTS[] = {{30, 8, 4, 0, {1, 1, 1, 1}}, {30, 8, 2, 1, {0, 0, 0, 0}}, {30, 8, 4, 0, {1, 1, 2, 2}}};
static acf_scenario_buffer_data gScenarioBufferData147_APU_DRAW_SEGMENTS[] = {{2, 10, 4, 0, {1, 1, 1, 1}}, {2, 10, 2, 1, {0, 0, 0, 0}}, {2, 10, 4, 0, {1, 1, 2, 2}}};
static acf_scenario_buffer_data gScenarioBufferData148_APU_DRAW_SEGMENTS[] = {{4, 10, 4, 0, {1, 1, 1, 1}}, {4, 10, 2, 1, {0, 0, 0, 0}}, {4, 10, 4, 0, {1, 1, 2, 2}}};
static acf_scenario_buffer_data gScenarioBufferData149_APU_DRAW_SEGMENTS[] = {{6, 10, 4, 0, {1, 1, 1, 1}}, {6, 10, 2, 1, {0, 0, 0, 0}}, {6, 10, 4, 0, {1, 1, 2, 2}}};
static acf_scenario_buffer_data gScenarioBufferData150_APU_DRAW_SEGMENTS[] = {{8, 10, 4, 0, {1, 1, 1, 1}}, {8, 10, 2, 1, {0, 0, 0, 0}}, {8, 10, 4, 0, {1, 1, 2, 2}}};
static acf_scenario_buffer_data gScenarioBufferData151_APU_DRAW_SEGMENTS[] = {{10, 10, 4, 0, {1, 1, 1, 1}}, {10, 10, 2, 1, {0, 0, 0, 0}}, {10, 10, 4, 0, {1, 1, 2, 2}}};
static acf_scenario_buffer_data gScenarioBufferData152_APU_DRAW_SEGMENTS[] = {{12, 10, 4, 0, {1, 1, 1, 1}}, {12, 10, 2, 1, {0, 0, 0, 0}}, {12, 10, 4, 0, {1, 1, 2, 2}}};
static acf_scenario_buffer_data gScenarioBufferData153_APU_DRAW_SEGMENTS[] = {{14, 10, 4, 0, {1, 1, 1, 1}}, {14, 10, 2, 1, {0, 0, 0, 0}}, {14, 10, 4, 0, {1, 1, 2, 2}}};
static acf_scenario_buffer_data gScenarioBufferData154_APU_DRAW_SEGMENTS[] = {{16, 10, 4, 0, {1, 1, 1, 1}}, {16, 10, 2, 1, {0, 0, 0, 0}}, {16, 10, 4, 0, {1, 1, 2, 2}}};
static acf_scenario_buffer_data gScenarioBufferData155_APU_DRAW_SEGMENTS[] = {{18, 10, 4, 0, {1, 1, 1, 1}}, {18, 10, 2, 1, {0, 0, 0, 0}}, {18, 10, 4, 0, {1, 1, 2, 2}}};
static acf_scenario_buffer_data gScenarioBufferData156_APU_DRAW_SEGMENTS[] = {{20, 10, 4, 0, {1, 1, 1, 1}}, {20, 10, 2, 1, {0, 0, 0, 0}}, {20, 10, 4, 0, {1, 1, 2, 2}}};
static acf_scenario_buffer_data gScenarioBufferData157_APU_DRAW_SEGMENTS[] = {{22, 10, 4, 0, {1, 1, 1, 1}}, {22, 10, 2, 1, {0, 0, 0, 0}}, {22, 10, 4, 0, {1, 1, 2, 2}}};
static acf_scenario_buffer_data gScenarioBufferData158_APU_DRAW_SEGMENTS[] = {{24, 10, 4, 0, {1, 1, 1, 1}}, {24, 10, 2, 1, {0, 0, 0, 0}}, {24, 10, 4, 0, {1, 1, 2, 2}}};
static acf_scenario_buffer_data gScenarioBufferData159_APU_DRAW_SEGMENTS[] = {{2, 12, 4, 0, {1, 1, 1, 1}}, {2, 12, 2, 1, {0, 0, 0, 0}}, {2, 12, 4, 0, {1, 1, 2, 2}}};
static acf_scenario_buffer_data gScenarioBufferData160_APU_DRAW_SEGMENTS[] = {{4, 12, 4, 0, {1, 1, 1, 1}}, {4, 12, 2, 1, {0, 0, 0, 0}}, {4, 12, 4, 0, {1, 1, 2, 2}}};
static acf_scenario_buffer_data gScenarioBufferData161_APU_DRAW_SEGMENTS[] = {{6, 12, 4, 0, {1, 1, 1, 1}}, {6, 12, 2, 1, {0, 0, 0, 0}}, {6, 12, 4, 0, {1, 1, 2, 2}}};
static acf_scenario_buffer_data gScenarioBufferData162_APU_DRAW_SEGMENTS[] = {{8, 12, 4, 0, {1, 1, 1, 1}}, {8, 12, 2, 1, {0, 0, 0, 0}}, {8, 12, 4, 0, {1, 1, 2, 2}}};
static acf_scenario_buffer_data gScenarioBufferData163_APU_DRAW_SEGMENTS[] = {{10, 12, 4, 0, {1, 1, 1, 1}}, {10, 12, 2, 1, {0, 0, 0, 0}}, {10, 12, 4, 0, {1, 1, 2, 2}}};
static acf_scenario_buffer_data gScenarioBufferData164_APU_DRAW_SEGMENTS[] = {{12, 12, 4, 0, {1, 1, 1, 1}}, {12, 12, 2, 1, {0, 0, 0, 0}}, {12, 12, 4, 0, {1, 1, 2, 2}}};
static acf_scenario_buffer_data gScenarioBufferData165_APU_DRAW_SEGMENTS[] = {{14, 12, 4, 0, {1, 1, 1, 1}}, {14, 12, 2, 1, {0, 0, 0, 0}}, {14, 12, 4, 0, {1, 1, 2, 2}}};
static acf_scenario_buffer_data gScenarioBufferData166_APU_DRAW_SEGMENTS[] = {{16, 12, 4, 0, {1, 1, 1, 1}}, {16, 12, 2, 1, {0, 0, 0, 0}}, {16, 12, 4, 0, {1, 1, 2, 2}}};
static acf_scenario_buffer_data gScenarioBufferData167_APU_DRAW_SEGMENTS[] = {{18, 12, 4, 0, {1, 1, 1, 1}}, {18, 12, 2, 1, {0, 0, 0, 0}}, {18, 12, 4, 0, {1, 1, 2, 2}}};
static acf_scenario_buffer_data gScenarioBufferData168_APU_DRAW_SEGMENTS[] = {{2, 14, 4, 0, {1, 1, 1, 1}}, {2, 14, 2, 1, {0, 0, 0, 0}}, {2, 14, 4, 0, {1, 1, 2, 2}}};
static acf_scenario_buffer_data gScenarioBufferData169_APU_DRAW_SEGMENTS[] = {{4, 14, 4, 0, {1, 1, 1, 1}}, {4, 14, 2, 1, {0, 0, 0, 0}}, {4, 14, 4, 0, {1, 1, 2, 2}}};
static acf_scenario_buffer_data gScenarioBufferData170_APU_DRAW_SEGMENTS[] = {{6, 14, 4, 0, {1, 1, 1, 1}}, {6, 14, 2, 1, {0, 0, 0, 0}}, {6, 14, 4, 0, {1, 1, 2, 2}}};
static acf_scenario_buffer_data gScenarioBufferData171_APU_DRAW_SEGMENTS[] = {{8, 14, 4, 0, {1, 1, 1, 1}}, {8, 14, 2, 1, {0, 0, 0, 0}}, {8, 14, 4, 0, {1, 1, 2, 2}}};
static acf_scenario_buffer_data gScenarioBufferData172_APU_DRAW_SEGMENTS[] = {{10, 14, 4, 0, {1, 1, 1, 1}}, {10, 14, 2, 1, {0, 0, 0, 0}}, {10, 14, 4, 0, {1, 1, 2, 2}}};
static acf_scenario_buffer_data gScenarioBufferData173_APU_DRAW_SEGMENTS[] = {{12, 14, 4, 0, {1, 1, 1, 1}}, {12, 14, 2, 1, {0, 0, 0, 0}}, {12, 14, 4, 0, {1, 1, 2, 2}}};
static acf_scenario_buffer_data gScenarioBufferData174_APU_DRAW_SEGMENTS[] = {{14, 14, 4, 0, {1, 1, 1, 1}}, {14, 14, 2, 1, {0, 0, 0, 0}}, {14, 14, 4, 0, {1, 1, 2, 2}}};
static acf_scenario_buffer_data gScenarioBufferData175_APU_DRAW_SEGMENTS[] = {{16, 14, 4, 0, {1, 1, 1, 1}}, {16, 14, 2, 1, {0, 0, 0, 0}}, {16, 14, 4, 0, {1, 1, 2, 2}}};
static acf_scenario_buffer_data gScenarioBufferData176_APU_DRAW_SEGMENTS[] = {{2, 16, 4, 0, {1, 1, 1, 1}}, {2, 16, 2, 1, {0, 0, 0, 0}}, {2, 16, 4, 0, {1, 1, 2, 2}}};
static acf_scenario_buffer_data gScenarioBufferData177_APU_DRAW_SEGMENTS[] = {{4, 16, 4, 0, {1, 1, 1, 1}}, {4, 16, 2, 1, {0, 0, 0, 0}}, {4, 16, 4, 0, {1, 1, 2, 2}}};
static acf_scenario_buffer_data gScenarioBufferData178_APU_DRAW_SEGMENTS[] = {{6, 16, 4, 0, {1, 1, 1, 1}}, {6, 16, 2, 1, {0, 0, 0, 0}}, {6, 16, 4, 0, {1, 1, 2, 2}}};
static acf_scenario_buffer_data gScenarioBufferData179_APU_DRAW_SEGMENTS[] = {{8, 16, 4, 0, {1, 1, 1, 1}}, {8, 16, 2, 1, {0, 0, 0, 0}}, {8, 16, 4, 0, {1, 1, 2, 2}}};
static acf_scenario_buffer_data gScenarioBufferData180_APU_DRAW_SEGMENTS[] = {{10, 16, 4, 0, {1, 1, 1, 1}}, {10, 16, 2, 1, {0, 0, 0, 0}}, {10, 16, 4, 0, {1, 1, 2, 2}}};
static acf_scenario_buffer_data gScenarioBufferData181_APU_DRAW_SEGMENTS[] = {{12, 16, 4, 0, {1, 1, 1, 1}}, {12, 16, 2, 1, {0, 0, 0, 0}}, {12, 16, 4, 0, {1, 1, 2, 2}}};
static acf_scenario_buffer_data gScenarioBufferData182_APU_DRAW_SEGMENTS[] = {{14, 16, 4, 0, {1, 1, 1, 1}}, {14, 16, 2, 1, {0, 0, 0, 0}}, {14, 16, 4, 0, {1, 1, 2, 2}}};
static acf_scenario_buffer_data gScenarioBufferData183_APU_DRAW_SEGMENTS[] = {{2, 18, 4, 0, {1, 1, 1, 1}}, {2, 18, 2, 1, {0, 0, 0, 0}}, {2, 18, 4, 0, {1, 1, 2, 2}}};
static acf_scenario_buffer_data gScenarioBufferData184_APU_DRAW_SEGMENTS[] = {{4, 18, 4, 0, {1, 1, 1, 1}}, {4, 18, 2, 1, {0, 0, 0, 0}}, {4, 18, 4, 0, {1, 1, 2, 2}}};
static acf_scenario_buffer_data gScenarioBufferData185_APU_DRAW_SEGMENTS[] = {{6, 18, 4, 0, {1, 1, 1, 1}}, {6, 18, 2, 1, {0, 0, 0, 0}}, {6, 18, 4, 0, {1, 1, 2, 2}}};
static acf_scenario_buffer_data gScenarioBufferData186_APU_DRAW_SEGMENTS[] = {{8, 18, 4, 0, {1, 1, 1, 1}}, {8, 18, 2, 1, {0, 0, 0, 0}}, {8, 18, 4, 0, {1, 1, 2, 2}}};
static acf_scenario_buffer_data gScenarioBufferData187_APU_DRAW_SEGMENTS[] = {{10, 18, 4, 0, {1, 1, 1, 1}}, {10, 18, 2, 1, {0, 0, 0, 0}}, {10, 18, 4, 0, {1, 1, 2, 2}}};
static acf_scenario_buffer_data gScenarioBufferData188_APU_DRAW_SEGMENTS[] = {{12, 18, 4, 0, {1, 1, 1, 1}}, {12, 18, 2, 1, {0, 0, 0, 0}}, {12, 18, 4, 0, {1, 1, 2, 2}}};
static acf_scenario_buffer_data gScenarioBufferData189_APU_DRAW_SEGMENTS[] = {{2, 20, 4, 0, {1, 1, 1, 1}}, {2, 20, 2, 1, {0, 0, 0, 0}}, {2, 20, 4, 0, {1, 1, 2, 2}}};
static acf_scenario_buffer_data gScenarioBufferData190_APU_DRAW_SEGMENTS[] = {{4, 20, 4, 0, {1, 1, 1, 1}}, {4, 20, 2, 1, {0, 0, 0, 0}}, {4, 20, 4, 0, {1, 1, 2, 2}}};
static acf_scenario_buffer_data gScenarioBufferData191_APU_DRAW_SEGMENTS[] = {{6, 20, 4, 0, {1, 1, 1, 1}}, {6, 20, 2, 1, {0, 0, 0, 0}}, {6, 20, 4, 0, {1, 1, 2, 2}}};
static acf_scenario_buffer_data gScenarioBufferData192_APU_DRAW_SEGMENTS[] = {{8, 20, 4, 0, {1, 1, 1, 1}}, {8, 20, 2, 1, {0, 0, 0, 0}}, {8, 20, 4, 0, {1, 1, 2, 2}}};
static acf_scenario_buffer_data gScenarioBufferData193_APU_DRAW_SEGMENTS[] = {{10, 20, 4, 0, {1, 1, 1, 1}}, {10, 20, 2, 1, {0, 0, 0, 0}}, {10, 20, 4, 0, {1, 1, 2, 2}}};
static acf_scenario_buffer_data gScenarioBufferData194_APU_DRAW_SEGMENTS[] = {{2, 22, 4, 0, {1, 1, 1, 1}}, {2, 22, 2, 1, {0, 0, 0, 0}}, {2, 22, 4, 0, {1, 1, 2, 2}}};
static acf_scenario_buffer_data gScenarioBufferData195_APU_DRAW_SEGMENTS[] = {{4, 22, 4, 0, {1, 1, 1, 1}}, {4, 22, 2, 1, {0, 0, 0, 0}}, {4, 22, 4, 0, {1, 1, 2, 2}}};
static acf_scenario_buffer_data gScenarioBufferData196_APU_DRAW_SEGMENTS[] = {{6, 22, 4, 0, {1, 1, 1, 1}}, {6, 22, 2, 1, {0, 0, 0, 0}}, {6, 22, 4, 0, {1, 1, 2, 2}}};
static acf_scenario_buffer_data gScenarioBufferData197_APU_DRAW_SEGMENTS[] = {{8, 22, 4, 0, {1, 1, 1, 1}}, {8, 22, 2, 1, {0, 0, 0, 0}}, {8, 22, 4, 0, {1, 1, 2, 2}}};
static acf_scenario_buffer_data gScenarioBufferData198_APU_DRAW_SEGMENTS[] = {{2, 24, 4, 0, {1, 1, 1, 1}}, {2, 24, 2, 1, {0, 0, 0, 0}}, {2, 24, 4, 0, {1, 1, 2, 2}}};
static acf_scenario_buffer_data gScenarioBufferData199_APU_DRAW_SEGMENTS[] = {{4, 24, 4, 0, {1, 1, 1, 1}}, {4, 24, 2, 1, {0, 0, 0, 0}}, {4, 24, 4, 0, {1, 1, 2, 2}}};
static acf_scenario_buffer_data gScenarioBufferData200_APU_DRAW_SEGMENTS[] = {{6, 24, 4, 0, {1, 1, 1, 1}}, {6, 24, 2, 1, {0, 0, 0, 0}}, {6, 24, 4, 0, {1, 1, 2, 2}}};
static acf_scenario_buffer_data gScenarioBufferData201_APU_DRAW_SEGMENTS[] = {{8, 24, 4, 0, {1, 1, 1, 1}}, {8, 24, 2, 1, {0, 0, 0, 0}}, {8, 24, 4, 0, {1, 1, 2, 2}}};
static acf_scenario_buffer_data gScenarioBufferData202_APU_DRAW_SEGMENTS[] = {{2, 26, 4, 0, {1, 1, 1, 1}}, {2, 26, 2, 1, {0, 0, 0, 0}}, {2, 26, 4, 0, {1, 1, 2, 2}}};
static acf_scenario_buffer_data gScenarioBufferData203_APU_DRAW_SEGMENTS[] = {{4, 26, 4, 0, {1, 1, 1, 1}}, {4, 26, 2, 1, {0, 0, 0, 0}}, {4, 26, 4, 0, {1, 1, 2, 2}}};
static acf_scenario_buffer_data gScenarioBufferData204_APU_DRAW_SEGMENTS[] = {{6, 26, 4, 0, {1, 1, 1, 1}}, {6, 26, 2, 1, {0, 0, 0, 0}}, {6, 26, 4, 0, {1, 1, 2, 2}}};
static acf_scenario_buffer_data gScenarioBufferData205_APU_DRAW_SEGMENTS[] = {{8, 26, 4, 0, {1, 1, 1, 1}}, {8, 26, 2, 1, {0, 0, 0, 0}}, {8, 26, 4, 0, {1, 1, 2, 2}}};
static acf_scenario_buffer_data gScenarioBufferData206_APU_DRAW_SEGMENTS[] = {{2, 28, 4, 0, {1, 1, 1, 1}}, {2, 28, 2, 1, {0, 0, 0, 0}}, {2, 28, 4, 0, {1, 1, 2, 2}}};
static acf_scenario_buffer_data gScenarioBufferData207_APU_DRAW_SEGMENTS[] = {{4, 28, 4, 0, {1, 1, 1, 1}}, {4, 28, 2, 1, {0, 0, 0, 0}}, {4, 28, 4, 0, {1, 1, 2, 2}}};
static acf_scenario_buffer_data gScenarioBufferData208_APU_DRAW_SEGMENTS[] = {{6, 28, 4, 0, {1, 1, 1, 1}}, {6, 28, 2, 1, {0, 0, 0, 0}}, {6, 28, 4, 0, {1, 1, 2, 2}}};
static acf_scenario_buffer_data gScenarioBufferData209_APU_DRAW_SEGMENTS[] = {{2, 30, 4, 0, {1, 1, 1, 1}}, {2, 30, 2, 1, {0, 0, 0, 0}}, {2, 30, 4, 0, {1, 1, 2, 2}}};
static acf_scenario_buffer_data gScenarioBufferData210_APU_DRAW_SEGMENTS[] = {{4, 30, 4, 0, {1, 1, 1, 1}}, {4, 30, 2, 1, {0, 0, 0, 0}}, {4, 30, 4, 0, {1, 1, 2, 2}}};
static acf_scenario_buffer_data gScenarioBufferData211_APU_DRAW_SEGMENTS[] = {{6, 30, 4, 0, {1, 1, 1, 1}}, {6, 30, 2, 1, {0, 0, 0, 0}}, {6, 30, 4, 0, {1, 1, 2, 2}}};
static acf_scenario_buffer_data gScenarioBufferData212_APU_DRAW_SEGMENTS[] = {{2, 32, 4, 0, {1, 1, 1, 1}}, {2, 32, 2, 1, {0, 0, 0, 0}}, {2, 32, 4, 0, {1, 1, 2, 2}}};
static acf_scenario_buffer_data gScenarioBufferData213_APU_DRAW_SEGMENTS[] = {{4, 32, 4, 0, {1, 1, 1, 1}}, {4, 32, 2, 1, {0, 0, 0, 0}}, {4, 32, 4, 0, {1, 1, 2, 2}}};
static acf_scenario_buffer_data gScenarioBufferData214_APU_DRAW_SEGMENTS[] = {{6, 32, 4, 0, {1, 1, 1, 1}}, {6, 32, 2, 1, {0, 0, 0, 0}}, {6, 32, 4, 0, {1, 1, 2, 2}}};

static acf_scenario_kernel_data gScenarioKernelData0_APU_DRAW_SEGMENTS[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData1_APU_DRAW_SEGMENTS[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData2_APU_DRAW_SEGMENTS[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData3_APU_DRAW_SEGMENTS[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData4_APU_DRAW_SEGMENTS[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData5_APU_DRAW_SEGMENTS[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData6_APU_DRAW_SEGMENTS[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData7_APU_DRAW_SEGMENTS[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData8_APU_DRAW_SEGMENTS[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData9_APU_DRAW_SEGMENTS[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData10_APU_DRAW_SEGMENTS[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData11_APU_DRAW_SEGMENTS[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData12_APU_DRAW_SEGMENTS[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData13_APU_DRAW_SEGMENTS[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData14_APU_DRAW_SEGMENTS[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData15_APU_DRAW_SEGMENTS[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData16_APU_DRAW_SEGMENTS[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData17_APU_DRAW_SEGMENTS[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData18_APU_DRAW_SEGMENTS[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData19_APU_DRAW_SEGMENTS[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData20_APU_DRAW_SEGMENTS[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData21_APU_DRAW_SEGMENTS[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData22_APU_DRAW_SEGMENTS[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData23_APU_DRAW_SEGMENTS[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData24_APU_DRAW_SEGMENTS[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData25_APU_DRAW_SEGMENTS[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData26_APU_DRAW_SEGMENTS[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData27_APU_DRAW_SEGMENTS[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData28_APU_DRAW_SEGMENTS[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData29_APU_DRAW_SEGMENTS[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData30_APU_DRAW_SEGMENTS[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData31_APU_DRAW_SEGMENTS[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData32_APU_DRAW_SEGMENTS[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData33_APU_DRAW_SEGMENTS[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData34_APU_DRAW_SEGMENTS[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData35_APU_DRAW_SEGMENTS[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData36_APU_DRAW_SEGMENTS[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData37_APU_DRAW_SEGMENTS[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData38_APU_DRAW_SEGMENTS[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData39_APU_DRAW_SEGMENTS[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData40_APU_DRAW_SEGMENTS[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData41_APU_DRAW_SEGMENTS[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData42_APU_DRAW_SEGMENTS[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData43_APU_DRAW_SEGMENTS[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData44_APU_DRAW_SEGMENTS[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData45_APU_DRAW_SEGMENTS[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData46_APU_DRAW_SEGMENTS[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData47_APU_DRAW_SEGMENTS[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData48_APU_DRAW_SEGMENTS[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData49_APU_DRAW_SEGMENTS[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData50_APU_DRAW_SEGMENTS[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData51_APU_DRAW_SEGMENTS[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData52_APU_DRAW_SEGMENTS[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData53_APU_DRAW_SEGMENTS[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData54_APU_DRAW_SEGMENTS[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData55_APU_DRAW_SEGMENTS[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData56_APU_DRAW_SEGMENTS[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData57_APU_DRAW_SEGMENTS[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData58_APU_DRAW_SEGMENTS[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData59_APU_DRAW_SEGMENTS[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData60_APU_DRAW_SEGMENTS[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData61_APU_DRAW_SEGMENTS[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData62_APU_DRAW_SEGMENTS[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData63_APU_DRAW_SEGMENTS[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData64_APU_DRAW_SEGMENTS[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData65_APU_DRAW_SEGMENTS[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData66_APU_DRAW_SEGMENTS[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData67_APU_DRAW_SEGMENTS[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData68_APU_DRAW_SEGMENTS[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData69_APU_DRAW_SEGMENTS[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData70_APU_DRAW_SEGMENTS[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData71_APU_DRAW_SEGMENTS[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData72_APU_DRAW_SEGMENTS[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData73_APU_DRAW_SEGMENTS[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData74_APU_DRAW_SEGMENTS[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData75_APU_DRAW_SEGMENTS[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData76_APU_DRAW_SEGMENTS[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData77_APU_DRAW_SEGMENTS[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData78_APU_DRAW_SEGMENTS[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData79_APU_DRAW_SEGMENTS[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData80_APU_DRAW_SEGMENTS[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData81_APU_DRAW_SEGMENTS[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData82_APU_DRAW_SEGMENTS[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData83_APU_DRAW_SEGMENTS[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData84_APU_DRAW_SEGMENTS[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData85_APU_DRAW_SEGMENTS[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData86_APU_DRAW_SEGMENTS[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData87_APU_DRAW_SEGMENTS[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData88_APU_DRAW_SEGMENTS[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData89_APU_DRAW_SEGMENTS[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData90_APU_DRAW_SEGMENTS[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData91_APU_DRAW_SEGMENTS[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData92_APU_DRAW_SEGMENTS[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData93_APU_DRAW_SEGMENTS[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData94_APU_DRAW_SEGMENTS[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData95_APU_DRAW_SEGMENTS[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData96_APU_DRAW_SEGMENTS[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData97_APU_DRAW_SEGMENTS[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData98_APU_DRAW_SEGMENTS[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData99_APU_DRAW_SEGMENTS[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData100_APU_DRAW_SEGMENTS[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData101_APU_DRAW_SEGMENTS[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData102_APU_DRAW_SEGMENTS[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData103_APU_DRAW_SEGMENTS[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData104_APU_DRAW_SEGMENTS[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData105_APU_DRAW_SEGMENTS[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData106_APU_DRAW_SEGMENTS[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData107_APU_DRAW_SEGMENTS[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData108_APU_DRAW_SEGMENTS[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData109_APU_DRAW_SEGMENTS[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData110_APU_DRAW_SEGMENTS[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData111_APU_DRAW_SEGMENTS[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData112_APU_DRAW_SEGMENTS[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData113_APU_DRAW_SEGMENTS[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData114_APU_DRAW_SEGMENTS[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData115_APU_DRAW_SEGMENTS[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData116_APU_DRAW_SEGMENTS[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData117_APU_DRAW_SEGMENTS[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData118_APU_DRAW_SEGMENTS[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData119_APU_DRAW_SEGMENTS[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData120_APU_DRAW_SEGMENTS[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData121_APU_DRAW_SEGMENTS[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData122_APU_DRAW_SEGMENTS[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData123_APU_DRAW_SEGMENTS[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData124_APU_DRAW_SEGMENTS[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData125_APU_DRAW_SEGMENTS[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData126_APU_DRAW_SEGMENTS[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData127_APU_DRAW_SEGMENTS[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData128_APU_DRAW_SEGMENTS[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData129_APU_DRAW_SEGMENTS[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData130_APU_DRAW_SEGMENTS[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData131_APU_DRAW_SEGMENTS[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData132_APU_DRAW_SEGMENTS[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData133_APU_DRAW_SEGMENTS[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData134_APU_DRAW_SEGMENTS[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData135_APU_DRAW_SEGMENTS[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData136_APU_DRAW_SEGMENTS[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData137_APU_DRAW_SEGMENTS[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData138_APU_DRAW_SEGMENTS[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData139_APU_DRAW_SEGMENTS[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData140_APU_DRAW_SEGMENTS[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData141_APU_DRAW_SEGMENTS[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData142_APU_DRAW_SEGMENTS[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData143_APU_DRAW_SEGMENTS[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData144_APU_DRAW_SEGMENTS[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData145_APU_DRAW_SEGMENTS[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData146_APU_DRAW_SEGMENTS[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData147_APU_DRAW_SEGMENTS[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData148_APU_DRAW_SEGMENTS[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData149_APU_DRAW_SEGMENTS[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData150_APU_DRAW_SEGMENTS[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData151_APU_DRAW_SEGMENTS[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData152_APU_DRAW_SEGMENTS[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData153_APU_DRAW_SEGMENTS[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData154_APU_DRAW_SEGMENTS[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData155_APU_DRAW_SEGMENTS[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData156_APU_DRAW_SEGMENTS[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData157_APU_DRAW_SEGMENTS[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData158_APU_DRAW_SEGMENTS[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData159_APU_DRAW_SEGMENTS[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData160_APU_DRAW_SEGMENTS[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData161_APU_DRAW_SEGMENTS[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData162_APU_DRAW_SEGMENTS[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData163_APU_DRAW_SEGMENTS[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData164_APU_DRAW_SEGMENTS[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData165_APU_DRAW_SEGMENTS[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData166_APU_DRAW_SEGMENTS[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData167_APU_DRAW_SEGMENTS[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData168_APU_DRAW_SEGMENTS[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData169_APU_DRAW_SEGMENTS[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData170_APU_DRAW_SEGMENTS[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData171_APU_DRAW_SEGMENTS[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData172_APU_DRAW_SEGMENTS[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData173_APU_DRAW_SEGMENTS[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData174_APU_DRAW_SEGMENTS[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData175_APU_DRAW_SEGMENTS[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData176_APU_DRAW_SEGMENTS[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData177_APU_DRAW_SEGMENTS[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData178_APU_DRAW_SEGMENTS[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData179_APU_DRAW_SEGMENTS[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData180_APU_DRAW_SEGMENTS[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData181_APU_DRAW_SEGMENTS[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData182_APU_DRAW_SEGMENTS[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData183_APU_DRAW_SEGMENTS[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData184_APU_DRAW_SEGMENTS[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData185_APU_DRAW_SEGMENTS[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData186_APU_DRAW_SEGMENTS[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData187_APU_DRAW_SEGMENTS[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData188_APU_DRAW_SEGMENTS[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData189_APU_DRAW_SEGMENTS[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData190_APU_DRAW_SEGMENTS[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData191_APU_DRAW_SEGMENTS[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData192_APU_DRAW_SEGMENTS[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData193_APU_DRAW_SEGMENTS[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData194_APU_DRAW_SEGMENTS[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData195_APU_DRAW_SEGMENTS[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData196_APU_DRAW_SEGMENTS[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData197_APU_DRAW_SEGMENTS[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData198_APU_DRAW_SEGMENTS[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData199_APU_DRAW_SEGMENTS[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData200_APU_DRAW_SEGMENTS[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData201_APU_DRAW_SEGMENTS[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData202_APU_DRAW_SEGMENTS[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData203_APU_DRAW_SEGMENTS[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData204_APU_DRAW_SEGMENTS[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData205_APU_DRAW_SEGMENTS[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData206_APU_DRAW_SEGMENTS[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData207_APU_DRAW_SEGMENTS[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData208_APU_DRAW_SEGMENTS[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData209_APU_DRAW_SEGMENTS[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData210_APU_DRAW_SEGMENTS[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData211_APU_DRAW_SEGMENTS[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData212_APU_DRAW_SEGMENTS[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData213_APU_DRAW_SEGMENTS[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData214_APU_DRAW_SEGMENTS[] = {{1, 0}};

static acf_scenario gScenarioArray_APU_DRAW_SEGMENTS[] = {
{2, 1, 88, 0, 1, gScenarioBufferData0_APU_DRAW_SEGMENTS, 48, gScenarioKernelData0_APU_DRAW_SEGMENTS, 4},
{4, 1, 128, 0, 1, gScenarioBufferData1_APU_DRAW_SEGMENTS, 48, gScenarioKernelData1_APU_DRAW_SEGMENTS, 4},
{6, 1, 168, 0, 1, gScenarioBufferData2_APU_DRAW_SEGMENTS, 48, gScenarioKernelData2_APU_DRAW_SEGMENTS, 4},
{8, 1, 208, 0, 1, gScenarioBufferData3_APU_DRAW_SEGMENTS, 48, gScenarioKernelData3_APU_DRAW_SEGMENTS, 4},
{10, 1, 248, 0, 1, gScenarioBufferData4_APU_DRAW_SEGMENTS, 48, gScenarioKernelData4_APU_DRAW_SEGMENTS, 4},
{12, 1, 288, 0, 1, gScenarioBufferData5_APU_DRAW_SEGMENTS, 48, gScenarioKernelData5_APU_DRAW_SEGMENTS, 4},
{14, 1, 328, 0, 1, gScenarioBufferData6_APU_DRAW_SEGMENTS, 48, gScenarioKernelData6_APU_DRAW_SEGMENTS, 4},
{16, 1, 368, 0, 1, gScenarioBufferData7_APU_DRAW_SEGMENTS, 48, gScenarioKernelData7_APU_DRAW_SEGMENTS, 4},
{18, 1, 408, 0, 1, gScenarioBufferData8_APU_DRAW_SEGMENTS, 48, gScenarioKernelData8_APU_DRAW_SEGMENTS, 4},
{20, 1, 448, 0, 1, gScenarioBufferData9_APU_DRAW_SEGMENTS, 48, gScenarioKernelData9_APU_DRAW_SEGMENTS, 4},
{22, 1, 488, 0, 1, gScenarioBufferData10_APU_DRAW_SEGMENTS, 48, gScenarioKernelData10_APU_DRAW_SEGMENTS, 4},
{24, 1, 528, 0, 1, gScenarioBufferData11_APU_DRAW_SEGMENTS, 48, gScenarioKernelData11_APU_DRAW_SEGMENTS, 4},
{26, 1, 568, 0, 1, gScenarioBufferData12_APU_DRAW_SEGMENTS, 48, gScenarioKernelData12_APU_DRAW_SEGMENTS, 4},
{28, 1, 608, 0, 1, gScenarioBufferData13_APU_DRAW_SEGMENTS, 48, gScenarioKernelData13_APU_DRAW_SEGMENTS, 4},
{30, 1, 648, 0, 1, gScenarioBufferData14_APU_DRAW_SEGMENTS, 48, gScenarioKernelData14_APU_DRAW_SEGMENTS, 4},
{32, 1, 688, 0, 1, gScenarioBufferData15_APU_DRAW_SEGMENTS, 48, gScenarioKernelData15_APU_DRAW_SEGMENTS, 4},
{36, 1, 768, 0, 1, gScenarioBufferData16_APU_DRAW_SEGMENTS, 48, gScenarioKernelData16_APU_DRAW_SEGMENTS, 4},
{40, 1, 848, 0, 1, gScenarioBufferData17_APU_DRAW_SEGMENTS, 48, gScenarioKernelData17_APU_DRAW_SEGMENTS, 4},
{44, 1, 928, 0, 1, gScenarioBufferData18_APU_DRAW_SEGMENTS, 48, gScenarioKernelData18_APU_DRAW_SEGMENTS, 4},
{48, 1, 1008, 0, 1, gScenarioBufferData19_APU_DRAW_SEGMENTS, 48, gScenarioKernelData19_APU_DRAW_SEGMENTS, 4},
{52, 1, 1088, 0, 1, gScenarioBufferData20_APU_DRAW_SEGMENTS, 48, gScenarioKernelData20_APU_DRAW_SEGMENTS, 4},
{56, 1, 1168, 0, 1, gScenarioBufferData21_APU_DRAW_SEGMENTS, 48, gScenarioKernelData21_APU_DRAW_SEGMENTS, 4},
{60, 1, 1248, 0, 1, gScenarioBufferData22_APU_DRAW_SEGMENTS, 48, gScenarioKernelData22_APU_DRAW_SEGMENTS, 4},
{64, 1, 1328, 0, 1, gScenarioBufferData23_APU_DRAW_SEGMENTS, 48, gScenarioKernelData23_APU_DRAW_SEGMENTS, 4},
{2, 2, 148, 0, 1, gScenarioBufferData24_APU_DRAW_SEGMENTS, 48, gScenarioKernelData24_APU_DRAW_SEGMENTS, 4},
{4, 2, 216, 0, 1, gScenarioBufferData25_APU_DRAW_SEGMENTS, 48, gScenarioKernelData25_APU_DRAW_SEGMENTS, 4},
{6, 2, 284, 0, 1, gScenarioBufferData26_APU_DRAW_SEGMENTS, 48, gScenarioKernelData26_APU_DRAW_SEGMENTS, 4},
{8, 2, 352, 0, 1, gScenarioBufferData27_APU_DRAW_SEGMENTS, 48, gScenarioKernelData27_APU_DRAW_SEGMENTS, 4},
{10, 2, 420, 0, 1, gScenarioBufferData28_APU_DRAW_SEGMENTS, 48, gScenarioKernelData28_APU_DRAW_SEGMENTS, 4},
{12, 2, 488, 0, 1, gScenarioBufferData29_APU_DRAW_SEGMENTS, 48, gScenarioKernelData29_APU_DRAW_SEGMENTS, 4},
{14, 2, 556, 0, 1, gScenarioBufferData30_APU_DRAW_SEGMENTS, 48, gScenarioKernelData30_APU_DRAW_SEGMENTS, 4},
{16, 2, 624, 0, 1, gScenarioBufferData31_APU_DRAW_SEGMENTS, 48, gScenarioKernelData31_APU_DRAW_SEGMENTS, 4},
{18, 2, 692, 0, 1, gScenarioBufferData32_APU_DRAW_SEGMENTS, 48, gScenarioKernelData32_APU_DRAW_SEGMENTS, 4},
{20, 2, 760, 0, 1, gScenarioBufferData33_APU_DRAW_SEGMENTS, 48, gScenarioKernelData33_APU_DRAW_SEGMENTS, 4},
{22, 2, 828, 0, 1, gScenarioBufferData34_APU_DRAW_SEGMENTS, 48, gScenarioKernelData34_APU_DRAW_SEGMENTS, 4},
{24, 2, 896, 0, 1, gScenarioBufferData35_APU_DRAW_SEGMENTS, 48, gScenarioKernelData35_APU_DRAW_SEGMENTS, 4},
{26, 2, 964, 0, 1, gScenarioBufferData36_APU_DRAW_SEGMENTS, 48, gScenarioKernelData36_APU_DRAW_SEGMENTS, 4},
{28, 2, 1032, 0, 1, gScenarioBufferData37_APU_DRAW_SEGMENTS, 48, gScenarioKernelData37_APU_DRAW_SEGMENTS, 4},
{30, 2, 1100, 0, 1, gScenarioBufferData38_APU_DRAW_SEGMENTS, 48, gScenarioKernelData38_APU_DRAW_SEGMENTS, 4},
{32, 2, 1168, 0, 1, gScenarioBufferData39_APU_DRAW_SEGMENTS, 48, gScenarioKernelData39_APU_DRAW_SEGMENTS, 4},
{36, 2, 1304, 0, 1, gScenarioBufferData40_APU_DRAW_SEGMENTS, 48, gScenarioKernelData40_APU_DRAW_SEGMENTS, 4},
{40, 2, 1440, 0, 1, gScenarioBufferData41_APU_DRAW_SEGMENTS, 48, gScenarioKernelData41_APU_DRAW_SEGMENTS, 4},
{44, 2, 1576, 0, 1, gScenarioBufferData42_APU_DRAW_SEGMENTS, 48, gScenarioKernelData42_APU_DRAW_SEGMENTS, 4},
{48, 2, 1712, 0, 1, gScenarioBufferData43_APU_DRAW_SEGMENTS, 48, gScenarioKernelData43_APU_DRAW_SEGMENTS, 4},
{52, 2, 1848, 0, 1, gScenarioBufferData44_APU_DRAW_SEGMENTS, 48, gScenarioKernelData44_APU_DRAW_SEGMENTS, 4},
{56, 2, 1984, 0, 1, gScenarioBufferData45_APU_DRAW_SEGMENTS, 48, gScenarioKernelData45_APU_DRAW_SEGMENTS, 4},
{60, 2, 2120, 0, 1, gScenarioBufferData46_APU_DRAW_SEGMENTS, 48, gScenarioKernelData46_APU_DRAW_SEGMENTS, 4},
{64, 2, 2256, 0, 1, gScenarioBufferData47_APU_DRAW_SEGMENTS, 48, gScenarioKernelData47_APU_DRAW_SEGMENTS, 4},
{2, 3, 208, 0, 1, gScenarioBufferData48_APU_DRAW_SEGMENTS, 48, gScenarioKernelData48_APU_DRAW_SEGMENTS, 4},
{4, 3, 304, 0, 1, gScenarioBufferData49_APU_DRAW_SEGMENTS, 48, gScenarioKernelData49_APU_DRAW_SEGMENTS, 4},
{6, 3, 400, 0, 1, gScenarioBufferData50_APU_DRAW_SEGMENTS, 48, gScenarioKernelData50_APU_DRAW_SEGMENTS, 4},
{8, 3, 496, 0, 1, gScenarioBufferData51_APU_DRAW_SEGMENTS, 48, gScenarioKernelData51_APU_DRAW_SEGMENTS, 4},
{10, 3, 592, 0, 1, gScenarioBufferData52_APU_DRAW_SEGMENTS, 48, gScenarioKernelData52_APU_DRAW_SEGMENTS, 4},
{12, 3, 688, 0, 1, gScenarioBufferData53_APU_DRAW_SEGMENTS, 48, gScenarioKernelData53_APU_DRAW_SEGMENTS, 4},
{14, 3, 784, 0, 1, gScenarioBufferData54_APU_DRAW_SEGMENTS, 48, gScenarioKernelData54_APU_DRAW_SEGMENTS, 4},
{16, 3, 880, 0, 1, gScenarioBufferData55_APU_DRAW_SEGMENTS, 48, gScenarioKernelData55_APU_DRAW_SEGMENTS, 4},
{18, 3, 976, 0, 1, gScenarioBufferData56_APU_DRAW_SEGMENTS, 48, gScenarioKernelData56_APU_DRAW_SEGMENTS, 4},
{20, 3, 1072, 0, 1, gScenarioBufferData57_APU_DRAW_SEGMENTS, 48, gScenarioKernelData57_APU_DRAW_SEGMENTS, 4},
{22, 3, 1168, 0, 1, gScenarioBufferData58_APU_DRAW_SEGMENTS, 48, gScenarioKernelData58_APU_DRAW_SEGMENTS, 4},
{24, 3, 1264, 0, 1, gScenarioBufferData59_APU_DRAW_SEGMENTS, 48, gScenarioKernelData59_APU_DRAW_SEGMENTS, 4},
{26, 3, 1360, 0, 1, gScenarioBufferData60_APU_DRAW_SEGMENTS, 48, gScenarioKernelData60_APU_DRAW_SEGMENTS, 4},
{28, 3, 1456, 0, 1, gScenarioBufferData61_APU_DRAW_SEGMENTS, 48, gScenarioKernelData61_APU_DRAW_SEGMENTS, 4},
{30, 3, 1552, 0, 1, gScenarioBufferData62_APU_DRAW_SEGMENTS, 48, gScenarioKernelData62_APU_DRAW_SEGMENTS, 4},
{32, 3, 1648, 0, 1, gScenarioBufferData63_APU_DRAW_SEGMENTS, 48, gScenarioKernelData63_APU_DRAW_SEGMENTS, 4},
{36, 3, 1840, 0, 1, gScenarioBufferData64_APU_DRAW_SEGMENTS, 48, gScenarioKernelData64_APU_DRAW_SEGMENTS, 4},
{40, 3, 2032, 0, 1, gScenarioBufferData65_APU_DRAW_SEGMENTS, 48, gScenarioKernelData65_APU_DRAW_SEGMENTS, 4},
{44, 3, 2224, 0, 1, gScenarioBufferData66_APU_DRAW_SEGMENTS, 48, gScenarioKernelData66_APU_DRAW_SEGMENTS, 4},
{48, 3, 2416, 0, 1, gScenarioBufferData67_APU_DRAW_SEGMENTS, 48, gScenarioKernelData67_APU_DRAW_SEGMENTS, 4},
{52, 3, 2608, 0, 1, gScenarioBufferData68_APU_DRAW_SEGMENTS, 48, gScenarioKernelData68_APU_DRAW_SEGMENTS, 4},
{56, 3, 2800, 0, 1, gScenarioBufferData69_APU_DRAW_SEGMENTS, 48, gScenarioKernelData69_APU_DRAW_SEGMENTS, 4},
{60, 3, 2992, 0, 1, gScenarioBufferData70_APU_DRAW_SEGMENTS, 48, gScenarioKernelData70_APU_DRAW_SEGMENTS, 4},
{64, 3, 3184, 0, 1, gScenarioBufferData71_APU_DRAW_SEGMENTS, 48, gScenarioKernelData71_APU_DRAW_SEGMENTS, 4},
{2, 4, 268, 0, 1, gScenarioBufferData72_APU_DRAW_SEGMENTS, 48, gScenarioKernelData72_APU_DRAW_SEGMENTS, 4},
{4, 4, 392, 0, 1, gScenarioBufferData73_APU_DRAW_SEGMENTS, 48, gScenarioKernelData73_APU_DRAW_SEGMENTS, 4},
{6, 4, 516, 0, 1, gScenarioBufferData74_APU_DRAW_SEGMENTS, 48, gScenarioKernelData74_APU_DRAW_SEGMENTS, 4},
{8, 4, 640, 0, 1, gScenarioBufferData75_APU_DRAW_SEGMENTS, 48, gScenarioKernelData75_APU_DRAW_SEGMENTS, 4},
{10, 4, 764, 0, 1, gScenarioBufferData76_APU_DRAW_SEGMENTS, 48, gScenarioKernelData76_APU_DRAW_SEGMENTS, 4},
{12, 4, 888, 0, 1, gScenarioBufferData77_APU_DRAW_SEGMENTS, 48, gScenarioKernelData77_APU_DRAW_SEGMENTS, 4},
{14, 4, 1012, 0, 1, gScenarioBufferData78_APU_DRAW_SEGMENTS, 48, gScenarioKernelData78_APU_DRAW_SEGMENTS, 4},
{16, 4, 1136, 0, 1, gScenarioBufferData79_APU_DRAW_SEGMENTS, 48, gScenarioKernelData79_APU_DRAW_SEGMENTS, 4},
{18, 4, 1260, 0, 1, gScenarioBufferData80_APU_DRAW_SEGMENTS, 48, gScenarioKernelData80_APU_DRAW_SEGMENTS, 4},
{20, 4, 1384, 0, 1, gScenarioBufferData81_APU_DRAW_SEGMENTS, 48, gScenarioKernelData81_APU_DRAW_SEGMENTS, 4},
{22, 4, 1508, 0, 1, gScenarioBufferData82_APU_DRAW_SEGMENTS, 48, gScenarioKernelData82_APU_DRAW_SEGMENTS, 4},
{24, 4, 1632, 0, 1, gScenarioBufferData83_APU_DRAW_SEGMENTS, 48, gScenarioKernelData83_APU_DRAW_SEGMENTS, 4},
{26, 4, 1756, 0, 1, gScenarioBufferData84_APU_DRAW_SEGMENTS, 48, gScenarioKernelData84_APU_DRAW_SEGMENTS, 4},
{28, 4, 1880, 0, 1, gScenarioBufferData85_APU_DRAW_SEGMENTS, 48, gScenarioKernelData85_APU_DRAW_SEGMENTS, 4},
{30, 4, 2004, 0, 1, gScenarioBufferData86_APU_DRAW_SEGMENTS, 48, gScenarioKernelData86_APU_DRAW_SEGMENTS, 4},
{32, 4, 2128, 0, 1, gScenarioBufferData87_APU_DRAW_SEGMENTS, 48, gScenarioKernelData87_APU_DRAW_SEGMENTS, 4},
{36, 4, 2376, 0, 1, gScenarioBufferData88_APU_DRAW_SEGMENTS, 48, gScenarioKernelData88_APU_DRAW_SEGMENTS, 4},
{40, 4, 2624, 0, 1, gScenarioBufferData89_APU_DRAW_SEGMENTS, 48, gScenarioKernelData89_APU_DRAW_SEGMENTS, 4},
{44, 4, 2872, 0, 1, gScenarioBufferData90_APU_DRAW_SEGMENTS, 48, gScenarioKernelData90_APU_DRAW_SEGMENTS, 4},
{48, 4, 3120, 0, 1, gScenarioBufferData91_APU_DRAW_SEGMENTS, 48, gScenarioKernelData91_APU_DRAW_SEGMENTS, 4},
{52, 4, 3368, 0, 1, gScenarioBufferData92_APU_DRAW_SEGMENTS, 48, gScenarioKernelData92_APU_DRAW_SEGMENTS, 4},
{56, 4, 3616, 0, 1, gScenarioBufferData93_APU_DRAW_SEGMENTS, 48, gScenarioKernelData93_APU_DRAW_SEGMENTS, 4},
{2, 5, 328, 0, 1, gScenarioBufferData94_APU_DRAW_SEGMENTS, 48, gScenarioKernelData94_APU_DRAW_SEGMENTS, 4},
{4, 5, 480, 0, 1, gScenarioBufferData95_APU_DRAW_SEGMENTS, 48, gScenarioKernelData95_APU_DRAW_SEGMENTS, 4},
{6, 5, 632, 0, 1, gScenarioBufferData96_APU_DRAW_SEGMENTS, 48, gScenarioKernelData96_APU_DRAW_SEGMENTS, 4},
{8, 5, 784, 0, 1, gScenarioBufferData97_APU_DRAW_SEGMENTS, 48, gScenarioKernelData97_APU_DRAW_SEGMENTS, 4},
{10, 5, 936, 0, 1, gScenarioBufferData98_APU_DRAW_SEGMENTS, 48, gScenarioKernelData98_APU_DRAW_SEGMENTS, 4},
{12, 5, 1088, 0, 1, gScenarioBufferData99_APU_DRAW_SEGMENTS, 48, gScenarioKernelData99_APU_DRAW_SEGMENTS, 4},
{14, 5, 1240, 0, 1, gScenarioBufferData100_APU_DRAW_SEGMENTS, 48, gScenarioKernelData100_APU_DRAW_SEGMENTS, 4},
{16, 5, 1392, 0, 1, gScenarioBufferData101_APU_DRAW_SEGMENTS, 48, gScenarioKernelData101_APU_DRAW_SEGMENTS, 4},
{18, 5, 1544, 0, 1, gScenarioBufferData102_APU_DRAW_SEGMENTS, 48, gScenarioKernelData102_APU_DRAW_SEGMENTS, 4},
{20, 5, 1696, 0, 1, gScenarioBufferData103_APU_DRAW_SEGMENTS, 48, gScenarioKernelData103_APU_DRAW_SEGMENTS, 4},
{22, 5, 1848, 0, 1, gScenarioBufferData104_APU_DRAW_SEGMENTS, 48, gScenarioKernelData104_APU_DRAW_SEGMENTS, 4},
{24, 5, 2000, 0, 1, gScenarioBufferData105_APU_DRAW_SEGMENTS, 48, gScenarioKernelData105_APU_DRAW_SEGMENTS, 4},
{26, 5, 2152, 0, 1, gScenarioBufferData106_APU_DRAW_SEGMENTS, 48, gScenarioKernelData106_APU_DRAW_SEGMENTS, 4},
{28, 5, 2304, 0, 1, gScenarioBufferData107_APU_DRAW_SEGMENTS, 48, gScenarioKernelData107_APU_DRAW_SEGMENTS, 4},
{30, 5, 2456, 0, 1, gScenarioBufferData108_APU_DRAW_SEGMENTS, 48, gScenarioKernelData108_APU_DRAW_SEGMENTS, 4},
{32, 5, 2608, 0, 1, gScenarioBufferData109_APU_DRAW_SEGMENTS, 48, gScenarioKernelData109_APU_DRAW_SEGMENTS, 4},
{36, 5, 2912, 0, 1, gScenarioBufferData110_APU_DRAW_SEGMENTS, 48, gScenarioKernelData110_APU_DRAW_SEGMENTS, 4},
{40, 5, 3216, 0, 1, gScenarioBufferData111_APU_DRAW_SEGMENTS, 48, gScenarioKernelData111_APU_DRAW_SEGMENTS, 4},
{44, 5, 3520, 0, 1, gScenarioBufferData112_APU_DRAW_SEGMENTS, 48, gScenarioKernelData112_APU_DRAW_SEGMENTS, 4},
{48, 5, 3824, 0, 1, gScenarioBufferData113_APU_DRAW_SEGMENTS, 48, gScenarioKernelData113_APU_DRAW_SEGMENTS, 4},
{2, 6, 388, 0, 1, gScenarioBufferData114_APU_DRAW_SEGMENTS, 48, gScenarioKernelData114_APU_DRAW_SEGMENTS, 4},
{4, 6, 568, 0, 1, gScenarioBufferData115_APU_DRAW_SEGMENTS, 48, gScenarioKernelData115_APU_DRAW_SEGMENTS, 4},
{6, 6, 748, 0, 1, gScenarioBufferData116_APU_DRAW_SEGMENTS, 48, gScenarioKernelData116_APU_DRAW_SEGMENTS, 4},
{8, 6, 928, 0, 1, gScenarioBufferData117_APU_DRAW_SEGMENTS, 48, gScenarioKernelData117_APU_DRAW_SEGMENTS, 4},
{10, 6, 1108, 0, 1, gScenarioBufferData118_APU_DRAW_SEGMENTS, 48, gScenarioKernelData118_APU_DRAW_SEGMENTS, 4},
{12, 6, 1288, 0, 1, gScenarioBufferData119_APU_DRAW_SEGMENTS, 48, gScenarioKernelData119_APU_DRAW_SEGMENTS, 4},
{14, 6, 1468, 0, 1, gScenarioBufferData120_APU_DRAW_SEGMENTS, 48, gScenarioKernelData120_APU_DRAW_SEGMENTS, 4},
{16, 6, 1648, 0, 1, gScenarioBufferData121_APU_DRAW_SEGMENTS, 48, gScenarioKernelData121_APU_DRAW_SEGMENTS, 4},
{18, 6, 1828, 0, 1, gScenarioBufferData122_APU_DRAW_SEGMENTS, 48, gScenarioKernelData122_APU_DRAW_SEGMENTS, 4},
{20, 6, 2008, 0, 1, gScenarioBufferData123_APU_DRAW_SEGMENTS, 48, gScenarioKernelData123_APU_DRAW_SEGMENTS, 4},
{22, 6, 2188, 0, 1, gScenarioBufferData124_APU_DRAW_SEGMENTS, 48, gScenarioKernelData124_APU_DRAW_SEGMENTS, 4},
{24, 6, 2368, 0, 1, gScenarioBufferData125_APU_DRAW_SEGMENTS, 48, gScenarioKernelData125_APU_DRAW_SEGMENTS, 4},
{26, 6, 2548, 0, 1, gScenarioBufferData126_APU_DRAW_SEGMENTS, 48, gScenarioKernelData126_APU_DRAW_SEGMENTS, 4},
{28, 6, 2728, 0, 1, gScenarioBufferData127_APU_DRAW_SEGMENTS, 48, gScenarioKernelData127_APU_DRAW_SEGMENTS, 4},
{30, 6, 2908, 0, 1, gScenarioBufferData128_APU_DRAW_SEGMENTS, 48, gScenarioKernelData128_APU_DRAW_SEGMENTS, 4},
{32, 6, 3088, 0, 1, gScenarioBufferData129_APU_DRAW_SEGMENTS, 48, gScenarioKernelData129_APU_DRAW_SEGMENTS, 4},
{36, 6, 3448, 0, 1, gScenarioBufferData130_APU_DRAW_SEGMENTS, 48, gScenarioKernelData130_APU_DRAW_SEGMENTS, 4},
{40, 6, 3808, 0, 1, gScenarioBufferData131_APU_DRAW_SEGMENTS, 48, gScenarioKernelData131_APU_DRAW_SEGMENTS, 4},
{2, 8, 508, 0, 1, gScenarioBufferData132_APU_DRAW_SEGMENTS, 48, gScenarioKernelData132_APU_DRAW_SEGMENTS, 4},
{4, 8, 744, 0, 1, gScenarioBufferData133_APU_DRAW_SEGMENTS, 48, gScenarioKernelData133_APU_DRAW_SEGMENTS, 4},
{6, 8, 980, 0, 1, gScenarioBufferData134_APU_DRAW_SEGMENTS, 48, gScenarioKernelData134_APU_DRAW_SEGMENTS, 4},
{8, 8, 1216, 0, 1, gScenarioBufferData135_APU_DRAW_SEGMENTS, 48, gScenarioKernelData135_APU_DRAW_SEGMENTS, 4},
{10, 8, 1452, 0, 1, gScenarioBufferData136_APU_DRAW_SEGMENTS, 48, gScenarioKernelData136_APU_DRAW_SEGMENTS, 4},
{12, 8, 1688, 0, 1, gScenarioBufferData137_APU_DRAW_SEGMENTS, 48, gScenarioKernelData137_APU_DRAW_SEGMENTS, 4},
{14, 8, 1924, 0, 1, gScenarioBufferData138_APU_DRAW_SEGMENTS, 48, gScenarioKernelData138_APU_DRAW_SEGMENTS, 4},
{16, 8, 2160, 0, 1, gScenarioBufferData139_APU_DRAW_SEGMENTS, 48, gScenarioKernelData139_APU_DRAW_SEGMENTS, 4},
{18, 8, 2396, 0, 1, gScenarioBufferData140_APU_DRAW_SEGMENTS, 48, gScenarioKernelData140_APU_DRAW_SEGMENTS, 4},
{20, 8, 2632, 0, 1, gScenarioBufferData141_APU_DRAW_SEGMENTS, 48, gScenarioKernelData141_APU_DRAW_SEGMENTS, 4},
{22, 8, 2868, 0, 1, gScenarioBufferData142_APU_DRAW_SEGMENTS, 48, gScenarioKernelData142_APU_DRAW_SEGMENTS, 4},
{24, 8, 3104, 0, 1, gScenarioBufferData143_APU_DRAW_SEGMENTS, 48, gScenarioKernelData143_APU_DRAW_SEGMENTS, 4},
{26, 8, 3340, 0, 1, gScenarioBufferData144_APU_DRAW_SEGMENTS, 48, gScenarioKernelData144_APU_DRAW_SEGMENTS, 4},
{28, 8, 3576, 0, 1, gScenarioBufferData145_APU_DRAW_SEGMENTS, 48, gScenarioKernelData145_APU_DRAW_SEGMENTS, 4},
{30, 8, 3812, 0, 1, gScenarioBufferData146_APU_DRAW_SEGMENTS, 48, gScenarioKernelData146_APU_DRAW_SEGMENTS, 4},
{2, 10, 628, 0, 1, gScenarioBufferData147_APU_DRAW_SEGMENTS, 48, gScenarioKernelData147_APU_DRAW_SEGMENTS, 4},
{4, 10, 920, 0, 1, gScenarioBufferData148_APU_DRAW_SEGMENTS, 48, gScenarioKernelData148_APU_DRAW_SEGMENTS, 4},
{6, 10, 1212, 0, 1, gScenarioBufferData149_APU_DRAW_SEGMENTS, 48, gScenarioKernelData149_APU_DRAW_SEGMENTS, 4},
{8, 10, 1504, 0, 1, gScenarioBufferData150_APU_DRAW_SEGMENTS, 48, gScenarioKernelData150_APU_DRAW_SEGMENTS, 4},
{10, 10, 1796, 0, 1, gScenarioBufferData151_APU_DRAW_SEGMENTS, 48, gScenarioKernelData151_APU_DRAW_SEGMENTS, 4},
{12, 10, 2088, 0, 1, gScenarioBufferData152_APU_DRAW_SEGMENTS, 48, gScenarioKernelData152_APU_DRAW_SEGMENTS, 4},
{14, 10, 2380, 0, 1, gScenarioBufferData153_APU_DRAW_SEGMENTS, 48, gScenarioKernelData153_APU_DRAW_SEGMENTS, 4},
{16, 10, 2672, 0, 1, gScenarioBufferData154_APU_DRAW_SEGMENTS, 48, gScenarioKernelData154_APU_DRAW_SEGMENTS, 4},
{18, 10, 2964, 0, 1, gScenarioBufferData155_APU_DRAW_SEGMENTS, 48, gScenarioKernelData155_APU_DRAW_SEGMENTS, 4},
{20, 10, 3256, 0, 1, gScenarioBufferData156_APU_DRAW_SEGMENTS, 48, gScenarioKernelData156_APU_DRAW_SEGMENTS, 4},
{22, 10, 3548, 0, 1, gScenarioBufferData157_APU_DRAW_SEGMENTS, 48, gScenarioKernelData157_APU_DRAW_SEGMENTS, 4},
{24, 10, 3840, 0, 1, gScenarioBufferData158_APU_DRAW_SEGMENTS, 48, gScenarioKernelData158_APU_DRAW_SEGMENTS, 4},
{2, 12, 748, 0, 1, gScenarioBufferData159_APU_DRAW_SEGMENTS, 48, gScenarioKernelData159_APU_DRAW_SEGMENTS, 4},
{4, 12, 1096, 0, 1, gScenarioBufferData160_APU_DRAW_SEGMENTS, 48, gScenarioKernelData160_APU_DRAW_SEGMENTS, 4},
{6, 12, 1444, 0, 1, gScenarioBufferData161_APU_DRAW_SEGMENTS, 48, gScenarioKernelData161_APU_DRAW_SEGMENTS, 4},
{8, 12, 1792, 0, 1, gScenarioBufferData162_APU_DRAW_SEGMENTS, 48, gScenarioKernelData162_APU_DRAW_SEGMENTS, 4},
{10, 12, 2140, 0, 1, gScenarioBufferData163_APU_DRAW_SEGMENTS, 48, gScenarioKernelData163_APU_DRAW_SEGMENTS, 4},
{12, 12, 2488, 0, 1, gScenarioBufferData164_APU_DRAW_SEGMENTS, 48, gScenarioKernelData164_APU_DRAW_SEGMENTS, 4},
{14, 12, 2836, 0, 1, gScenarioBufferData165_APU_DRAW_SEGMENTS, 48, gScenarioKernelData165_APU_DRAW_SEGMENTS, 4},
{16, 12, 3184, 0, 1, gScenarioBufferData166_APU_DRAW_SEGMENTS, 48, gScenarioKernelData166_APU_DRAW_SEGMENTS, 4},
{18, 12, 3532, 0, 1, gScenarioBufferData167_APU_DRAW_SEGMENTS, 48, gScenarioKernelData167_APU_DRAW_SEGMENTS, 4},
{2, 14, 868, 0, 1, gScenarioBufferData168_APU_DRAW_SEGMENTS, 48, gScenarioKernelData168_APU_DRAW_SEGMENTS, 4},
{4, 14, 1272, 0, 1, gScenarioBufferData169_APU_DRAW_SEGMENTS, 48, gScenarioKernelData169_APU_DRAW_SEGMENTS, 4},
{6, 14, 1676, 0, 1, gScenarioBufferData170_APU_DRAW_SEGMENTS, 48, gScenarioKernelData170_APU_DRAW_SEGMENTS, 4},
{8, 14, 2080, 0, 1, gScenarioBufferData171_APU_DRAW_SEGMENTS, 48, gScenarioKernelData171_APU_DRAW_SEGMENTS, 4},
{10, 14, 2484, 0, 1, gScenarioBufferData172_APU_DRAW_SEGMENTS, 48, gScenarioKernelData172_APU_DRAW_SEGMENTS, 4},
{12, 14, 2888, 0, 1, gScenarioBufferData173_APU_DRAW_SEGMENTS, 48, gScenarioKernelData173_APU_DRAW_SEGMENTS, 4},
{14, 14, 3292, 0, 1, gScenarioBufferData174_APU_DRAW_SEGMENTS, 48, gScenarioKernelData174_APU_DRAW_SEGMENTS, 4},
{16, 14, 3696, 0, 1, gScenarioBufferData175_APU_DRAW_SEGMENTS, 48, gScenarioKernelData175_APU_DRAW_SEGMENTS, 4},
{2, 16, 988, 0, 1, gScenarioBufferData176_APU_DRAW_SEGMENTS, 48, gScenarioKernelData176_APU_DRAW_SEGMENTS, 4},
{4, 16, 1448, 0, 1, gScenarioBufferData177_APU_DRAW_SEGMENTS, 48, gScenarioKernelData177_APU_DRAW_SEGMENTS, 4},
{6, 16, 1908, 0, 1, gScenarioBufferData178_APU_DRAW_SEGMENTS, 48, gScenarioKernelData178_APU_DRAW_SEGMENTS, 4},
{8, 16, 2368, 0, 1, gScenarioBufferData179_APU_DRAW_SEGMENTS, 48, gScenarioKernelData179_APU_DRAW_SEGMENTS, 4},
{10, 16, 2828, 0, 1, gScenarioBufferData180_APU_DRAW_SEGMENTS, 48, gScenarioKernelData180_APU_DRAW_SEGMENTS, 4},
{12, 16, 3288, 0, 1, gScenarioBufferData181_APU_DRAW_SEGMENTS, 48, gScenarioKernelData181_APU_DRAW_SEGMENTS, 4},
{14, 16, 3748, 0, 1, gScenarioBufferData182_APU_DRAW_SEGMENTS, 48, gScenarioKernelData182_APU_DRAW_SEGMENTS, 4},
{2, 18, 1108, 0, 1, gScenarioBufferData183_APU_DRAW_SEGMENTS, 48, gScenarioKernelData183_APU_DRAW_SEGMENTS, 4},
{4, 18, 1624, 0, 1, gScenarioBufferData184_APU_DRAW_SEGMENTS, 48, gScenarioKernelData184_APU_DRAW_SEGMENTS, 4},
{6, 18, 2140, 0, 1, gScenarioBufferData185_APU_DRAW_SEGMENTS, 48, gScenarioKernelData185_APU_DRAW_SEGMENTS, 4},
{8, 18, 2656, 0, 1, gScenarioBufferData186_APU_DRAW_SEGMENTS, 48, gScenarioKernelData186_APU_DRAW_SEGMENTS, 4},
{10, 18, 3172, 0, 1, gScenarioBufferData187_APU_DRAW_SEGMENTS, 48, gScenarioKernelData187_APU_DRAW_SEGMENTS, 4},
{12, 18, 3688, 0, 1, gScenarioBufferData188_APU_DRAW_SEGMENTS, 48, gScenarioKernelData188_APU_DRAW_SEGMENTS, 4},
{2, 20, 1228, 0, 1, gScenarioBufferData189_APU_DRAW_SEGMENTS, 48, gScenarioKernelData189_APU_DRAW_SEGMENTS, 4},
{4, 20, 1800, 0, 1, gScenarioBufferData190_APU_DRAW_SEGMENTS, 48, gScenarioKernelData190_APU_DRAW_SEGMENTS, 4},
{6, 20, 2372, 0, 1, gScenarioBufferData191_APU_DRAW_SEGMENTS, 48, gScenarioKernelData191_APU_DRAW_SEGMENTS, 4},
{8, 20, 2944, 0, 1, gScenarioBufferData192_APU_DRAW_SEGMENTS, 48, gScenarioKernelData192_APU_DRAW_SEGMENTS, 4},
{10, 20, 3516, 0, 1, gScenarioBufferData193_APU_DRAW_SEGMENTS, 48, gScenarioKernelData193_APU_DRAW_SEGMENTS, 4},
{2, 22, 1348, 0, 1, gScenarioBufferData194_APU_DRAW_SEGMENTS, 48, gScenarioKernelData194_APU_DRAW_SEGMENTS, 4},
{4, 22, 1976, 0, 1, gScenarioBufferData195_APU_DRAW_SEGMENTS, 48, gScenarioKernelData195_APU_DRAW_SEGMENTS, 4},
{6, 22, 2604, 0, 1, gScenarioBufferData196_APU_DRAW_SEGMENTS, 48, gScenarioKernelData196_APU_DRAW_SEGMENTS, 4},
{8, 22, 3232, 0, 1, gScenarioBufferData197_APU_DRAW_SEGMENTS, 48, gScenarioKernelData197_APU_DRAW_SEGMENTS, 4},
{2, 24, 1468, 0, 1, gScenarioBufferData198_APU_DRAW_SEGMENTS, 48, gScenarioKernelData198_APU_DRAW_SEGMENTS, 4},
{4, 24, 2152, 0, 1, gScenarioBufferData199_APU_DRAW_SEGMENTS, 48, gScenarioKernelData199_APU_DRAW_SEGMENTS, 4},
{6, 24, 2836, 0, 1, gScenarioBufferData200_APU_DRAW_SEGMENTS, 48, gScenarioKernelData200_APU_DRAW_SEGMENTS, 4},
{8, 24, 3520, 0, 1, gScenarioBufferData201_APU_DRAW_SEGMENTS, 48, gScenarioKernelData201_APU_DRAW_SEGMENTS, 4},
{2, 26, 1588, 0, 1, gScenarioBufferData202_APU_DRAW_SEGMENTS, 48, gScenarioKernelData202_APU_DRAW_SEGMENTS, 4},
{4, 26, 2328, 0, 1, gScenarioBufferData203_APU_DRAW_SEGMENTS, 48, gScenarioKernelData203_APU_DRAW_SEGMENTS, 4},
{6, 26, 3068, 0, 1, gScenarioBufferData204_APU_DRAW_SEGMENTS, 48, gScenarioKernelData204_APU_DRAW_SEGMENTS, 4},
{8, 26, 3808, 0, 1, gScenarioBufferData205_APU_DRAW_SEGMENTS, 48, gScenarioKernelData205_APU_DRAW_SEGMENTS, 4},
{2, 28, 1708, 0, 1, gScenarioBufferData206_APU_DRAW_SEGMENTS, 48, gScenarioKernelData206_APU_DRAW_SEGMENTS, 4},
{4, 28, 2504, 0, 1, gScenarioBufferData207_APU_DRAW_SEGMENTS, 48, gScenarioKernelData207_APU_DRAW_SEGMENTS, 4},
{6, 28, 3300, 0, 1, gScenarioBufferData208_APU_DRAW_SEGMENTS, 48, gScenarioKernelData208_APU_DRAW_SEGMENTS, 4},
{2, 30, 1828, 0, 1, gScenarioBufferData209_APU_DRAW_SEGMENTS, 48, gScenarioKernelData209_APU_DRAW_SEGMENTS, 4},
{4, 30, 2680, 0, 1, gScenarioBufferData210_APU_DRAW_SEGMENTS, 48, gScenarioKernelData210_APU_DRAW_SEGMENTS, 4},
{6, 30, 3532, 0, 1, gScenarioBufferData211_APU_DRAW_SEGMENTS, 48, gScenarioKernelData211_APU_DRAW_SEGMENTS, 4},
{2, 32, 1948, 0, 1, gScenarioBufferData212_APU_DRAW_SEGMENTS, 48, gScenarioKernelData212_APU_DRAW_SEGMENTS, 4},
{4, 32, 2856, 0, 1, gScenarioBufferData213_APU_DRAW_SEGMENTS, 48, gScenarioKernelData213_APU_DRAW_SEGMENTS, 4},
{6, 32, 3764, 0, 1, gScenarioBufferData214_APU_DRAW_SEGMENTS, 48, gScenarioKernelData214_APU_DRAW_SEGMENTS, 4}
};

static acf_scenario_list gScenarioList_APU_DRAW_SEGMENTS = {
215, //number of scenarios
gScenarioArray_APU_DRAW_SEGMENTS};
//**************************************************************

class APU_DRAW_SEGMENTS : public ACF_Process_APU
{

public:
   APU_DRAW_SEGMENTS(int32_t apex_id = 0) : ACF_Process_APU(apex_id)
   {} 

   int32_t Initialize()
   {
      int32_t lRetVal = 0;

      if (!Initialized()) //initialization steps that only need to occur once
      {
         SetProcessIdentifier("APU_DRAW_SEGMENTS");
         SetApuLoadInfo(APU_DRAW_SEGMENTS_LOAD_SEGMENTS,
                        APU_DRAW_SEGMENTS_LOAD_PMEM, APU_DRAW_SEGMENTS_LOAD_PMEM_SIZE,
                        APU_DRAW_SEGMENTS_LOAD_DMEM, APU_DRAW_SEGMENTS_LOAD_DMEM_SIZE,
                        0, 0); //assuming _LOAD_CMEM does not exist

         lRetVal = NonParametricInitialization();
         if (lRetVal != 0) return lRetVal;

         FlagSpatialDep();

         //***NOTE: the order in which the following ports are added is meaningful; do not change!

         AddPort("INPUT_0", icp::DATATYPE_16U, 1, 1, 2, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0);
         AddPort("INPUT_1", icp::DATATYPE_08U, 1, 1, 2, 1, 0, 0, 0, 0, 0, 0, 2, 1, 1, 1, 1, 0);
         AddPort("OUTPUT_0", icp::DATATYPE_08U, 1, 1, 2, 1, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0);

         CfgScenarios(&gScenarioList_APU_DRAW_SEGMENTS);

         FlagAsInitialized();
      }

      lRetVal = SelectApuConfiguration(mApuCfg, mApexId); //by default mApuCfg = ACF_APU_CFG__APEX0_DEFAULT and mApexId = 0

      return lRetVal;
   }
};

#endif //_ACF_PROCESS_APU_APU_DRAW_SEGMENTS
