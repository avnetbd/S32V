#ifndef _ACF_PROCESS_APU_APU_DRAW_SEGMENTS_RGB
#define _ACF_PROCESS_APU_APU_DRAW_SEGMENTS_RGB

#include <acf_process_apu.h>
#include <APU_DRAW_SEGMENTS_RGB_APU_LOAD.h> //APU load associated with this process
#include <stdint.h>


//SCENARIO LIST*************************************************
static acf_scenario_buffer_data gScenarioBufferData0_APU_DRAW_SEGMENTS_RGB[] = {{2, 1, 4, 0, {1, 1, 1, 1}}, {2, 1, 2, 1, {0, 0, 0, 0}}, {2, 1, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData1_APU_DRAW_SEGMENTS_RGB[] = {{4, 1, 4, 0, {1, 1, 1, 1}}, {4, 1, 2, 1, {0, 0, 0, 0}}, {4, 1, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData2_APU_DRAW_SEGMENTS_RGB[] = {{6, 1, 4, 0, {1, 1, 1, 1}}, {6, 1, 2, 1, {0, 0, 0, 0}}, {6, 1, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData3_APU_DRAW_SEGMENTS_RGB[] = {{8, 1, 4, 0, {1, 1, 1, 1}}, {8, 1, 2, 1, {0, 0, 0, 0}}, {8, 1, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData4_APU_DRAW_SEGMENTS_RGB[] = {{10, 1, 4, 0, {1, 1, 1, 1}}, {10, 1, 2, 1, {0, 0, 0, 0}}, {10, 1, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData5_APU_DRAW_SEGMENTS_RGB[] = {{12, 1, 4, 0, {1, 1, 1, 1}}, {12, 1, 2, 1, {0, 0, 0, 0}}, {12, 1, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData6_APU_DRAW_SEGMENTS_RGB[] = {{16, 1, 4, 0, {1, 1, 1, 1}}, {16, 1, 2, 1, {0, 0, 0, 0}}, {16, 1, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData7_APU_DRAW_SEGMENTS_RGB[] = {{20, 1, 4, 0, {1, 1, 1, 1}}, {20, 1, 2, 1, {0, 0, 0, 0}}, {20, 1, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData8_APU_DRAW_SEGMENTS_RGB[] = {{24, 1, 4, 0, {1, 1, 1, 1}}, {24, 1, 2, 1, {0, 0, 0, 0}}, {24, 1, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData9_APU_DRAW_SEGMENTS_RGB[] = {{32, 1, 4, 0, {1, 1, 1, 1}}, {32, 1, 2, 1, {0, 0, 0, 0}}, {32, 1, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData10_APU_DRAW_SEGMENTS_RGB[] = {{40, 1, 4, 0, {1, 1, 1, 1}}, {40, 1, 2, 1, {0, 0, 0, 0}}, {40, 1, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData11_APU_DRAW_SEGMENTS_RGB[] = {{2, 2, 4, 0, {1, 1, 1, 1}}, {2, 2, 2, 1, {0, 0, 0, 0}}, {2, 2, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData12_APU_DRAW_SEGMENTS_RGB[] = {{4, 2, 4, 0, {1, 1, 1, 1}}, {4, 2, 2, 1, {0, 0, 0, 0}}, {4, 2, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData13_APU_DRAW_SEGMENTS_RGB[] = {{6, 2, 4, 0, {1, 1, 1, 1}}, {6, 2, 2, 1, {0, 0, 0, 0}}, {6, 2, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData14_APU_DRAW_SEGMENTS_RGB[] = {{8, 2, 4, 0, {1, 1, 1, 1}}, {8, 2, 2, 1, {0, 0, 0, 0}}, {8, 2, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData15_APU_DRAW_SEGMENTS_RGB[] = {{10, 2, 4, 0, {1, 1, 1, 1}}, {10, 2, 2, 1, {0, 0, 0, 0}}, {10, 2, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData16_APU_DRAW_SEGMENTS_RGB[] = {{12, 2, 4, 0, {1, 1, 1, 1}}, {12, 2, 2, 1, {0, 0, 0, 0}}, {12, 2, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData17_APU_DRAW_SEGMENTS_RGB[] = {{16, 2, 4, 0, {1, 1, 1, 1}}, {16, 2, 2, 1, {0, 0, 0, 0}}, {16, 2, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData18_APU_DRAW_SEGMENTS_RGB[] = {{20, 2, 4, 0, {1, 1, 1, 1}}, {20, 2, 2, 1, {0, 0, 0, 0}}, {20, 2, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData19_APU_DRAW_SEGMENTS_RGB[] = {{24, 2, 4, 0, {1, 1, 1, 1}}, {24, 2, 2, 1, {0, 0, 0, 0}}, {24, 2, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData20_APU_DRAW_SEGMENTS_RGB[] = {{32, 2, 4, 0, {1, 1, 1, 1}}, {32, 2, 2, 1, {0, 0, 0, 0}}, {32, 2, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData21_APU_DRAW_SEGMENTS_RGB[] = {{40, 2, 4, 0, {1, 1, 1, 1}}, {40, 2, 2, 1, {0, 0, 0, 0}}, {40, 2, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData22_APU_DRAW_SEGMENTS_RGB[] = {{2, 3, 4, 0, {1, 1, 1, 1}}, {2, 3, 2, 1, {0, 0, 0, 0}}, {2, 3, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData23_APU_DRAW_SEGMENTS_RGB[] = {{4, 3, 4, 0, {1, 1, 1, 1}}, {4, 3, 2, 1, {0, 0, 0, 0}}, {4, 3, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData24_APU_DRAW_SEGMENTS_RGB[] = {{6, 3, 4, 0, {1, 1, 1, 1}}, {6, 3, 2, 1, {0, 0, 0, 0}}, {6, 3, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData25_APU_DRAW_SEGMENTS_RGB[] = {{8, 3, 4, 0, {1, 1, 1, 1}}, {8, 3, 2, 1, {0, 0, 0, 0}}, {8, 3, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData26_APU_DRAW_SEGMENTS_RGB[] = {{10, 3, 4, 0, {1, 1, 1, 1}}, {10, 3, 2, 1, {0, 0, 0, 0}}, {10, 3, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData27_APU_DRAW_SEGMENTS_RGB[] = {{12, 3, 4, 0, {1, 1, 1, 1}}, {12, 3, 2, 1, {0, 0, 0, 0}}, {12, 3, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData28_APU_DRAW_SEGMENTS_RGB[] = {{16, 3, 4, 0, {1, 1, 1, 1}}, {16, 3, 2, 1, {0, 0, 0, 0}}, {16, 3, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData29_APU_DRAW_SEGMENTS_RGB[] = {{20, 3, 4, 0, {1, 1, 1, 1}}, {20, 3, 2, 1, {0, 0, 0, 0}}, {20, 3, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData30_APU_DRAW_SEGMENTS_RGB[] = {{24, 3, 4, 0, {1, 1, 1, 1}}, {24, 3, 2, 1, {0, 0, 0, 0}}, {24, 3, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData31_APU_DRAW_SEGMENTS_RGB[] = {{32, 3, 4, 0, {1, 1, 1, 1}}, {32, 3, 2, 1, {0, 0, 0, 0}}, {32, 3, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData32_APU_DRAW_SEGMENTS_RGB[] = {{40, 3, 4, 0, {1, 1, 1, 1}}, {40, 3, 2, 1, {0, 0, 0, 0}}, {40, 3, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData33_APU_DRAW_SEGMENTS_RGB[] = {{2, 4, 4, 0, {1, 1, 1, 1}}, {2, 4, 2, 1, {0, 0, 0, 0}}, {2, 4, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData34_APU_DRAW_SEGMENTS_RGB[] = {{4, 4, 4, 0, {1, 1, 1, 1}}, {4, 4, 2, 1, {0, 0, 0, 0}}, {4, 4, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData35_APU_DRAW_SEGMENTS_RGB[] = {{6, 4, 4, 0, {1, 1, 1, 1}}, {6, 4, 2, 1, {0, 0, 0, 0}}, {6, 4, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData36_APU_DRAW_SEGMENTS_RGB[] = {{8, 4, 4, 0, {1, 1, 1, 1}}, {8, 4, 2, 1, {0, 0, 0, 0}}, {8, 4, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData37_APU_DRAW_SEGMENTS_RGB[] = {{10, 4, 4, 0, {1, 1, 1, 1}}, {10, 4, 2, 1, {0, 0, 0, 0}}, {10, 4, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData38_APU_DRAW_SEGMENTS_RGB[] = {{12, 4, 4, 0, {1, 1, 1, 1}}, {12, 4, 2, 1, {0, 0, 0, 0}}, {12, 4, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData39_APU_DRAW_SEGMENTS_RGB[] = {{16, 4, 4, 0, {1, 1, 1, 1}}, {16, 4, 2, 1, {0, 0, 0, 0}}, {16, 4, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData40_APU_DRAW_SEGMENTS_RGB[] = {{20, 4, 4, 0, {1, 1, 1, 1}}, {20, 4, 2, 1, {0, 0, 0, 0}}, {20, 4, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData41_APU_DRAW_SEGMENTS_RGB[] = {{24, 4, 4, 0, {1, 1, 1, 1}}, {24, 4, 2, 1, {0, 0, 0, 0}}, {24, 4, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData42_APU_DRAW_SEGMENTS_RGB[] = {{32, 4, 4, 0, {1, 1, 1, 1}}, {32, 4, 2, 1, {0, 0, 0, 0}}, {32, 4, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData43_APU_DRAW_SEGMENTS_RGB[] = {{40, 4, 4, 0, {1, 1, 1, 1}}, {40, 4, 2, 1, {0, 0, 0, 0}}, {40, 4, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData44_APU_DRAW_SEGMENTS_RGB[] = {{2, 5, 4, 0, {1, 1, 1, 1}}, {2, 5, 2, 1, {0, 0, 0, 0}}, {2, 5, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData45_APU_DRAW_SEGMENTS_RGB[] = {{4, 5, 4, 0, {1, 1, 1, 1}}, {4, 5, 2, 1, {0, 0, 0, 0}}, {4, 5, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData46_APU_DRAW_SEGMENTS_RGB[] = {{6, 5, 4, 0, {1, 1, 1, 1}}, {6, 5, 2, 1, {0, 0, 0, 0}}, {6, 5, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData47_APU_DRAW_SEGMENTS_RGB[] = {{8, 5, 4, 0, {1, 1, 1, 1}}, {8, 5, 2, 1, {0, 0, 0, 0}}, {8, 5, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData48_APU_DRAW_SEGMENTS_RGB[] = {{10, 5, 4, 0, {1, 1, 1, 1}}, {10, 5, 2, 1, {0, 0, 0, 0}}, {10, 5, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData49_APU_DRAW_SEGMENTS_RGB[] = {{12, 5, 4, 0, {1, 1, 1, 1}}, {12, 5, 2, 1, {0, 0, 0, 0}}, {12, 5, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData50_APU_DRAW_SEGMENTS_RGB[] = {{16, 5, 4, 0, {1, 1, 1, 1}}, {16, 5, 2, 1, {0, 0, 0, 0}}, {16, 5, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData51_APU_DRAW_SEGMENTS_RGB[] = {{20, 5, 4, 0, {1, 1, 1, 1}}, {20, 5, 2, 1, {0, 0, 0, 0}}, {20, 5, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData52_APU_DRAW_SEGMENTS_RGB[] = {{24, 5, 4, 0, {1, 1, 1, 1}}, {24, 5, 2, 1, {0, 0, 0, 0}}, {24, 5, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData53_APU_DRAW_SEGMENTS_RGB[] = {{32, 5, 4, 0, {1, 1, 1, 1}}, {32, 5, 2, 1, {0, 0, 0, 0}}, {32, 5, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData54_APU_DRAW_SEGMENTS_RGB[] = {{2, 6, 4, 0, {1, 1, 1, 1}}, {2, 6, 2, 1, {0, 0, 0, 0}}, {2, 6, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData55_APU_DRAW_SEGMENTS_RGB[] = {{4, 6, 4, 0, {1, 1, 1, 1}}, {4, 6, 2, 1, {0, 0, 0, 0}}, {4, 6, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData56_APU_DRAW_SEGMENTS_RGB[] = {{6, 6, 4, 0, {1, 1, 1, 1}}, {6, 6, 2, 1, {0, 0, 0, 0}}, {6, 6, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData57_APU_DRAW_SEGMENTS_RGB[] = {{8, 6, 4, 0, {1, 1, 1, 1}}, {8, 6, 2, 1, {0, 0, 0, 0}}, {8, 6, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData58_APU_DRAW_SEGMENTS_RGB[] = {{10, 6, 4, 0, {1, 1, 1, 1}}, {10, 6, 2, 1, {0, 0, 0, 0}}, {10, 6, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData59_APU_DRAW_SEGMENTS_RGB[] = {{12, 6, 4, 0, {1, 1, 1, 1}}, {12, 6, 2, 1, {0, 0, 0, 0}}, {12, 6, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData60_APU_DRAW_SEGMENTS_RGB[] = {{16, 6, 4, 0, {1, 1, 1, 1}}, {16, 6, 2, 1, {0, 0, 0, 0}}, {16, 6, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData61_APU_DRAW_SEGMENTS_RGB[] = {{20, 6, 4, 0, {1, 1, 1, 1}}, {20, 6, 2, 1, {0, 0, 0, 0}}, {20, 6, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData62_APU_DRAW_SEGMENTS_RGB[] = {{24, 6, 4, 0, {1, 1, 1, 1}}, {24, 6, 2, 1, {0, 0, 0, 0}}, {24, 6, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData63_APU_DRAW_SEGMENTS_RGB[] = {{2, 8, 4, 0, {1, 1, 1, 1}}, {2, 8, 2, 1, {0, 0, 0, 0}}, {2, 8, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData64_APU_DRAW_SEGMENTS_RGB[] = {{4, 8, 4, 0, {1, 1, 1, 1}}, {4, 8, 2, 1, {0, 0, 0, 0}}, {4, 8, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData65_APU_DRAW_SEGMENTS_RGB[] = {{6, 8, 4, 0, {1, 1, 1, 1}}, {6, 8, 2, 1, {0, 0, 0, 0}}, {6, 8, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData66_APU_DRAW_SEGMENTS_RGB[] = {{8, 8, 4, 0, {1, 1, 1, 1}}, {8, 8, 2, 1, {0, 0, 0, 0}}, {8, 8, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData67_APU_DRAW_SEGMENTS_RGB[] = {{10, 8, 4, 0, {1, 1, 1, 1}}, {10, 8, 2, 1, {0, 0, 0, 0}}, {10, 8, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData68_APU_DRAW_SEGMENTS_RGB[] = {{12, 8, 4, 0, {1, 1, 1, 1}}, {12, 8, 2, 1, {0, 0, 0, 0}}, {12, 8, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData69_APU_DRAW_SEGMENTS_RGB[] = {{16, 8, 4, 0, {1, 1, 1, 1}}, {16, 8, 2, 1, {0, 0, 0, 0}}, {16, 8, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData70_APU_DRAW_SEGMENTS_RGB[] = {{20, 8, 4, 0, {1, 1, 1, 1}}, {20, 8, 2, 1, {0, 0, 0, 0}}, {20, 8, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData71_APU_DRAW_SEGMENTS_RGB[] = {{2, 10, 4, 0, {1, 1, 1, 1}}, {2, 10, 2, 1, {0, 0, 0, 0}}, {2, 10, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData72_APU_DRAW_SEGMENTS_RGB[] = {{4, 10, 4, 0, {1, 1, 1, 1}}, {4, 10, 2, 1, {0, 0, 0, 0}}, {4, 10, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData73_APU_DRAW_SEGMENTS_RGB[] = {{6, 10, 4, 0, {1, 1, 1, 1}}, {6, 10, 2, 1, {0, 0, 0, 0}}, {6, 10, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData74_APU_DRAW_SEGMENTS_RGB[] = {{8, 10, 4, 0, {1, 1, 1, 1}}, {8, 10, 2, 1, {0, 0, 0, 0}}, {8, 10, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData75_APU_DRAW_SEGMENTS_RGB[] = {{10, 10, 4, 0, {1, 1, 1, 1}}, {10, 10, 2, 1, {0, 0, 0, 0}}, {10, 10, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData76_APU_DRAW_SEGMENTS_RGB[] = {{12, 10, 4, 0, {1, 1, 1, 1}}, {12, 10, 2, 1, {0, 0, 0, 0}}, {12, 10, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData77_APU_DRAW_SEGMENTS_RGB[] = {{16, 10, 4, 0, {1, 1, 1, 1}}, {16, 10, 2, 1, {0, 0, 0, 0}}, {16, 10, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData78_APU_DRAW_SEGMENTS_RGB[] = {{2, 12, 4, 0, {1, 1, 1, 1}}, {2, 12, 2, 1, {0, 0, 0, 0}}, {2, 12, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData79_APU_DRAW_SEGMENTS_RGB[] = {{4, 12, 4, 0, {1, 1, 1, 1}}, {4, 12, 2, 1, {0, 0, 0, 0}}, {4, 12, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData80_APU_DRAW_SEGMENTS_RGB[] = {{6, 12, 4, 0, {1, 1, 1, 1}}, {6, 12, 2, 1, {0, 0, 0, 0}}, {6, 12, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData81_APU_DRAW_SEGMENTS_RGB[] = {{8, 12, 4, 0, {1, 1, 1, 1}}, {8, 12, 2, 1, {0, 0, 0, 0}}, {8, 12, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData82_APU_DRAW_SEGMENTS_RGB[] = {{10, 12, 4, 0, {1, 1, 1, 1}}, {10, 12, 2, 1, {0, 0, 0, 0}}, {10, 12, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData83_APU_DRAW_SEGMENTS_RGB[] = {{12, 12, 4, 0, {1, 1, 1, 1}}, {12, 12, 2, 1, {0, 0, 0, 0}}, {12, 12, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData84_APU_DRAW_SEGMENTS_RGB[] = {{2, 14, 4, 0, {1, 1, 1, 1}}, {2, 14, 2, 1, {0, 0, 0, 0}}, {2, 14, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData85_APU_DRAW_SEGMENTS_RGB[] = {{4, 14, 4, 0, {1, 1, 1, 1}}, {4, 14, 2, 1, {0, 0, 0, 0}}, {4, 14, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData86_APU_DRAW_SEGMENTS_RGB[] = {{6, 14, 4, 0, {1, 1, 1, 1}}, {6, 14, 2, 1, {0, 0, 0, 0}}, {6, 14, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData87_APU_DRAW_SEGMENTS_RGB[] = {{8, 14, 4, 0, {1, 1, 1, 1}}, {8, 14, 2, 1, {0, 0, 0, 0}}, {8, 14, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData88_APU_DRAW_SEGMENTS_RGB[] = {{10, 14, 4, 0, {1, 1, 1, 1}}, {10, 14, 2, 1, {0, 0, 0, 0}}, {10, 14, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData89_APU_DRAW_SEGMENTS_RGB[] = {{12, 14, 4, 0, {1, 1, 1, 1}}, {12, 14, 2, 1, {0, 0, 0, 0}}, {12, 14, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData90_APU_DRAW_SEGMENTS_RGB[] = {{2, 16, 4, 0, {1, 1, 1, 1}}, {2, 16, 2, 1, {0, 0, 0, 0}}, {2, 16, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData91_APU_DRAW_SEGMENTS_RGB[] = {{4, 16, 4, 0, {1, 1, 1, 1}}, {4, 16, 2, 1, {0, 0, 0, 0}}, {4, 16, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData92_APU_DRAW_SEGMENTS_RGB[] = {{6, 16, 4, 0, {1, 1, 1, 1}}, {6, 16, 2, 1, {0, 0, 0, 0}}, {6, 16, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData93_APU_DRAW_SEGMENTS_RGB[] = {{8, 16, 4, 0, {1, 1, 1, 1}}, {8, 16, 2, 1, {0, 0, 0, 0}}, {8, 16, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData94_APU_DRAW_SEGMENTS_RGB[] = {{10, 16, 4, 0, {1, 1, 1, 1}}, {10, 16, 2, 1, {0, 0, 0, 0}}, {10, 16, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData95_APU_DRAW_SEGMENTS_RGB[] = {{2, 18, 4, 0, {1, 1, 1, 1}}, {2, 18, 2, 1, {0, 0, 0, 0}}, {2, 18, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData96_APU_DRAW_SEGMENTS_RGB[] = {{4, 18, 4, 0, {1, 1, 1, 1}}, {4, 18, 2, 1, {0, 0, 0, 0}}, {4, 18, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData97_APU_DRAW_SEGMENTS_RGB[] = {{6, 18, 4, 0, {1, 1, 1, 1}}, {6, 18, 2, 1, {0, 0, 0, 0}}, {6, 18, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData98_APU_DRAW_SEGMENTS_RGB[] = {{8, 18, 4, 0, {1, 1, 1, 1}}, {8, 18, 2, 1, {0, 0, 0, 0}}, {8, 18, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData99_APU_DRAW_SEGMENTS_RGB[] = {{2, 20, 4, 0, {1, 1, 1, 1}}, {2, 20, 2, 1, {0, 0, 0, 0}}, {2, 20, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData100_APU_DRAW_SEGMENTS_RGB[] = {{4, 20, 4, 0, {1, 1, 1, 1}}, {4, 20, 2, 1, {0, 0, 0, 0}}, {4, 20, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData101_APU_DRAW_SEGMENTS_RGB[] = {{6, 20, 4, 0, {1, 1, 1, 1}}, {6, 20, 2, 1, {0, 0, 0, 0}}, {6, 20, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData102_APU_DRAW_SEGMENTS_RGB[] = {{8, 20, 4, 0, {1, 1, 1, 1}}, {8, 20, 2, 1, {0, 0, 0, 0}}, {8, 20, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData103_APU_DRAW_SEGMENTS_RGB[] = {{2, 22, 4, 0, {1, 1, 1, 1}}, {2, 22, 2, 1, {0, 0, 0, 0}}, {2, 22, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData104_APU_DRAW_SEGMENTS_RGB[] = {{4, 22, 4, 0, {1, 1, 1, 1}}, {4, 22, 2, 1, {0, 0, 0, 0}}, {4, 22, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData105_APU_DRAW_SEGMENTS_RGB[] = {{6, 22, 4, 0, {1, 1, 1, 1}}, {6, 22, 2, 1, {0, 0, 0, 0}}, {6, 22, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData106_APU_DRAW_SEGMENTS_RGB[] = {{2, 24, 4, 0, {1, 1, 1, 1}}, {2, 24, 2, 1, {0, 0, 0, 0}}, {2, 24, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData107_APU_DRAW_SEGMENTS_RGB[] = {{4, 24, 4, 0, {1, 1, 1, 1}}, {4, 24, 2, 1, {0, 0, 0, 0}}, {4, 24, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData108_APU_DRAW_SEGMENTS_RGB[] = {{6, 24, 4, 0, {1, 1, 1, 1}}, {6, 24, 2, 1, {0, 0, 0, 0}}, {6, 24, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData109_APU_DRAW_SEGMENTS_RGB[] = {{2, 26, 4, 0, {1, 1, 1, 1}}, {2, 26, 2, 1, {0, 0, 0, 0}}, {2, 26, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData110_APU_DRAW_SEGMENTS_RGB[] = {{4, 26, 4, 0, {1, 1, 1, 1}}, {4, 26, 2, 1, {0, 0, 0, 0}}, {4, 26, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData111_APU_DRAW_SEGMENTS_RGB[] = {{6, 26, 4, 0, {1, 1, 1, 1}}, {6, 26, 2, 1, {0, 0, 0, 0}}, {6, 26, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData112_APU_DRAW_SEGMENTS_RGB[] = {{2, 28, 4, 0, {1, 1, 1, 1}}, {2, 28, 2, 1, {0, 0, 0, 0}}, {2, 28, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData113_APU_DRAW_SEGMENTS_RGB[] = {{4, 28, 4, 0, {1, 1, 1, 1}}, {4, 28, 2, 1, {0, 0, 0, 0}}, {4, 28, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData114_APU_DRAW_SEGMENTS_RGB[] = {{6, 28, 4, 0, {1, 1, 1, 1}}, {6, 28, 2, 1, {0, 0, 0, 0}}, {6, 28, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData115_APU_DRAW_SEGMENTS_RGB[] = {{2, 30, 4, 0, {1, 1, 1, 1}}, {2, 30, 2, 1, {0, 0, 0, 0}}, {2, 30, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData116_APU_DRAW_SEGMENTS_RGB[] = {{4, 30, 4, 0, {1, 1, 1, 1}}, {4, 30, 2, 1, {0, 0, 0, 0}}, {4, 30, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData117_APU_DRAW_SEGMENTS_RGB[] = {{2, 32, 4, 0, {1, 1, 1, 1}}, {2, 32, 2, 1, {0, 0, 0, 0}}, {2, 32, 2, 1, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData118_APU_DRAW_SEGMENTS_RGB[] = {{4, 32, 4, 0, {1, 1, 1, 1}}, {4, 32, 2, 1, {0, 0, 0, 0}}, {4, 32, 2, 1, {0, 0, 0, 0}}};

static acf_scenario_kernel_data gScenarioKernelData0_APU_DRAW_SEGMENTS_RGB[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData1_APU_DRAW_SEGMENTS_RGB[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData2_APU_DRAW_SEGMENTS_RGB[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData3_APU_DRAW_SEGMENTS_RGB[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData4_APU_DRAW_SEGMENTS_RGB[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData5_APU_DRAW_SEGMENTS_RGB[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData6_APU_DRAW_SEGMENTS_RGB[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData7_APU_DRAW_SEGMENTS_RGB[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData8_APU_DRAW_SEGMENTS_RGB[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData9_APU_DRAW_SEGMENTS_RGB[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData10_APU_DRAW_SEGMENTS_RGB[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData11_APU_DRAW_SEGMENTS_RGB[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData12_APU_DRAW_SEGMENTS_RGB[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData13_APU_DRAW_SEGMENTS_RGB[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData14_APU_DRAW_SEGMENTS_RGB[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData15_APU_DRAW_SEGMENTS_RGB[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData16_APU_DRAW_SEGMENTS_RGB[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData17_APU_DRAW_SEGMENTS_RGB[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData18_APU_DRAW_SEGMENTS_RGB[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData19_APU_DRAW_SEGMENTS_RGB[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData20_APU_DRAW_SEGMENTS_RGB[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData21_APU_DRAW_SEGMENTS_RGB[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData22_APU_DRAW_SEGMENTS_RGB[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData23_APU_DRAW_SEGMENTS_RGB[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData24_APU_DRAW_SEGMENTS_RGB[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData25_APU_DRAW_SEGMENTS_RGB[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData26_APU_DRAW_SEGMENTS_RGB[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData27_APU_DRAW_SEGMENTS_RGB[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData28_APU_DRAW_SEGMENTS_RGB[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData29_APU_DRAW_SEGMENTS_RGB[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData30_APU_DRAW_SEGMENTS_RGB[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData31_APU_DRAW_SEGMENTS_RGB[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData32_APU_DRAW_SEGMENTS_RGB[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData33_APU_DRAW_SEGMENTS_RGB[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData34_APU_DRAW_SEGMENTS_RGB[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData35_APU_DRAW_SEGMENTS_RGB[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData36_APU_DRAW_SEGMENTS_RGB[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData37_APU_DRAW_SEGMENTS_RGB[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData38_APU_DRAW_SEGMENTS_RGB[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData39_APU_DRAW_SEGMENTS_RGB[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData40_APU_DRAW_SEGMENTS_RGB[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData41_APU_DRAW_SEGMENTS_RGB[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData42_APU_DRAW_SEGMENTS_RGB[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData43_APU_DRAW_SEGMENTS_RGB[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData44_APU_DRAW_SEGMENTS_RGB[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData45_APU_DRAW_SEGMENTS_RGB[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData46_APU_DRAW_SEGMENTS_RGB[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData47_APU_DRAW_SEGMENTS_RGB[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData48_APU_DRAW_SEGMENTS_RGB[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData49_APU_DRAW_SEGMENTS_RGB[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData50_APU_DRAW_SEGMENTS_RGB[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData51_APU_DRAW_SEGMENTS_RGB[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData52_APU_DRAW_SEGMENTS_RGB[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData53_APU_DRAW_SEGMENTS_RGB[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData54_APU_DRAW_SEGMENTS_RGB[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData55_APU_DRAW_SEGMENTS_RGB[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData56_APU_DRAW_SEGMENTS_RGB[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData57_APU_DRAW_SEGMENTS_RGB[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData58_APU_DRAW_SEGMENTS_RGB[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData59_APU_DRAW_SEGMENTS_RGB[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData60_APU_DRAW_SEGMENTS_RGB[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData61_APU_DRAW_SEGMENTS_RGB[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData62_APU_DRAW_SEGMENTS_RGB[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData63_APU_DRAW_SEGMENTS_RGB[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData64_APU_DRAW_SEGMENTS_RGB[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData65_APU_DRAW_SEGMENTS_RGB[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData66_APU_DRAW_SEGMENTS_RGB[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData67_APU_DRAW_SEGMENTS_RGB[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData68_APU_DRAW_SEGMENTS_RGB[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData69_APU_DRAW_SEGMENTS_RGB[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData70_APU_DRAW_SEGMENTS_RGB[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData71_APU_DRAW_SEGMENTS_RGB[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData72_APU_DRAW_SEGMENTS_RGB[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData73_APU_DRAW_SEGMENTS_RGB[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData74_APU_DRAW_SEGMENTS_RGB[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData75_APU_DRAW_SEGMENTS_RGB[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData76_APU_DRAW_SEGMENTS_RGB[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData77_APU_DRAW_SEGMENTS_RGB[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData78_APU_DRAW_SEGMENTS_RGB[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData79_APU_DRAW_SEGMENTS_RGB[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData80_APU_DRAW_SEGMENTS_RGB[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData81_APU_DRAW_SEGMENTS_RGB[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData82_APU_DRAW_SEGMENTS_RGB[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData83_APU_DRAW_SEGMENTS_RGB[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData84_APU_DRAW_SEGMENTS_RGB[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData85_APU_DRAW_SEGMENTS_RGB[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData86_APU_DRAW_SEGMENTS_RGB[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData87_APU_DRAW_SEGMENTS_RGB[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData88_APU_DRAW_SEGMENTS_RGB[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData89_APU_DRAW_SEGMENTS_RGB[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData90_APU_DRAW_SEGMENTS_RGB[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData91_APU_DRAW_SEGMENTS_RGB[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData92_APU_DRAW_SEGMENTS_RGB[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData93_APU_DRAW_SEGMENTS_RGB[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData94_APU_DRAW_SEGMENTS_RGB[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData95_APU_DRAW_SEGMENTS_RGB[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData96_APU_DRAW_SEGMENTS_RGB[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData97_APU_DRAW_SEGMENTS_RGB[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData98_APU_DRAW_SEGMENTS_RGB[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData99_APU_DRAW_SEGMENTS_RGB[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData100_APU_DRAW_SEGMENTS_RGB[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData101_APU_DRAW_SEGMENTS_RGB[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData102_APU_DRAW_SEGMENTS_RGB[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData103_APU_DRAW_SEGMENTS_RGB[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData104_APU_DRAW_SEGMENTS_RGB[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData105_APU_DRAW_SEGMENTS_RGB[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData106_APU_DRAW_SEGMENTS_RGB[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData107_APU_DRAW_SEGMENTS_RGB[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData108_APU_DRAW_SEGMENTS_RGB[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData109_APU_DRAW_SEGMENTS_RGB[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData110_APU_DRAW_SEGMENTS_RGB[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData111_APU_DRAW_SEGMENTS_RGB[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData112_APU_DRAW_SEGMENTS_RGB[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData113_APU_DRAW_SEGMENTS_RGB[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData114_APU_DRAW_SEGMENTS_RGB[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData115_APU_DRAW_SEGMENTS_RGB[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData116_APU_DRAW_SEGMENTS_RGB[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData117_APU_DRAW_SEGMENTS_RGB[] = {{1, 0}};
static acf_scenario_kernel_data gScenarioKernelData118_APU_DRAW_SEGMENTS_RGB[] = {{1, 0}};

static acf_scenario gScenarioArray_APU_DRAW_SEGMENTS_RGB[] = {
{2, 1, 72, 0, 1, gScenarioBufferData0_APU_DRAW_SEGMENTS_RGB, 48, gScenarioKernelData0_APU_DRAW_SEGMENTS_RGB, 4},
{4, 1, 120, 0, 1, gScenarioBufferData1_APU_DRAW_SEGMENTS_RGB, 48, gScenarioKernelData1_APU_DRAW_SEGMENTS_RGB, 4},
{6, 1, 168, 0, 1, gScenarioBufferData2_APU_DRAW_SEGMENTS_RGB, 48, gScenarioKernelData2_APU_DRAW_SEGMENTS_RGB, 4},
{8, 1, 216, 0, 1, gScenarioBufferData3_APU_DRAW_SEGMENTS_RGB, 48, gScenarioKernelData3_APU_DRAW_SEGMENTS_RGB, 4},
{10, 1, 264, 0, 1, gScenarioBufferData4_APU_DRAW_SEGMENTS_RGB, 48, gScenarioKernelData4_APU_DRAW_SEGMENTS_RGB, 4},
{12, 1, 312, 0, 1, gScenarioBufferData5_APU_DRAW_SEGMENTS_RGB, 48, gScenarioKernelData5_APU_DRAW_SEGMENTS_RGB, 4},
{16, 1, 408, 0, 1, gScenarioBufferData6_APU_DRAW_SEGMENTS_RGB, 48, gScenarioKernelData6_APU_DRAW_SEGMENTS_RGB, 4},
{20, 1, 504, 0, 1, gScenarioBufferData7_APU_DRAW_SEGMENTS_RGB, 48, gScenarioKernelData7_APU_DRAW_SEGMENTS_RGB, 4},
{24, 1, 600, 0, 1, gScenarioBufferData8_APU_DRAW_SEGMENTS_RGB, 48, gScenarioKernelData8_APU_DRAW_SEGMENTS_RGB, 4},
{32, 1, 792, 0, 1, gScenarioBufferData9_APU_DRAW_SEGMENTS_RGB, 48, gScenarioKernelData9_APU_DRAW_SEGMENTS_RGB, 4},
{40, 1, 984, 0, 1, gScenarioBufferData10_APU_DRAW_SEGMENTS_RGB, 48, gScenarioKernelData10_APU_DRAW_SEGMENTS_RGB, 4},
{2, 2, 128, 0, 1, gScenarioBufferData11_APU_DRAW_SEGMENTS_RGB, 48, gScenarioKernelData11_APU_DRAW_SEGMENTS_RGB, 4},
{4, 2, 216, 0, 1, gScenarioBufferData12_APU_DRAW_SEGMENTS_RGB, 48, gScenarioKernelData12_APU_DRAW_SEGMENTS_RGB, 4},
{6, 2, 304, 0, 1, gScenarioBufferData13_APU_DRAW_SEGMENTS_RGB, 48, gScenarioKernelData13_APU_DRAW_SEGMENTS_RGB, 4},
{8, 2, 392, 0, 1, gScenarioBufferData14_APU_DRAW_SEGMENTS_RGB, 48, gScenarioKernelData14_APU_DRAW_SEGMENTS_RGB, 4},
{10, 2, 480, 0, 1, gScenarioBufferData15_APU_DRAW_SEGMENTS_RGB, 48, gScenarioKernelData15_APU_DRAW_SEGMENTS_RGB, 4},
{12, 2, 568, 0, 1, gScenarioBufferData16_APU_DRAW_SEGMENTS_RGB, 48, gScenarioKernelData16_APU_DRAW_SEGMENTS_RGB, 4},
{16, 2, 744, 0, 1, gScenarioBufferData17_APU_DRAW_SEGMENTS_RGB, 48, gScenarioKernelData17_APU_DRAW_SEGMENTS_RGB, 4},
{20, 2, 920, 0, 1, gScenarioBufferData18_APU_DRAW_SEGMENTS_RGB, 48, gScenarioKernelData18_APU_DRAW_SEGMENTS_RGB, 4},
{24, 2, 1096, 0, 1, gScenarioBufferData19_APU_DRAW_SEGMENTS_RGB, 48, gScenarioKernelData19_APU_DRAW_SEGMENTS_RGB, 4},
{32, 2, 1448, 0, 1, gScenarioBufferData20_APU_DRAW_SEGMENTS_RGB, 48, gScenarioKernelData20_APU_DRAW_SEGMENTS_RGB, 4},
{40, 2, 1800, 0, 1, gScenarioBufferData21_APU_DRAW_SEGMENTS_RGB, 48, gScenarioKernelData21_APU_DRAW_SEGMENTS_RGB, 4},
{2, 3, 184, 0, 1, gScenarioBufferData22_APU_DRAW_SEGMENTS_RGB, 48, gScenarioKernelData22_APU_DRAW_SEGMENTS_RGB, 4},
{4, 3, 312, 0, 1, gScenarioBufferData23_APU_DRAW_SEGMENTS_RGB, 48, gScenarioKernelData23_APU_DRAW_SEGMENTS_RGB, 4},
{6, 3, 440, 0, 1, gScenarioBufferData24_APU_DRAW_SEGMENTS_RGB, 48, gScenarioKernelData24_APU_DRAW_SEGMENTS_RGB, 4},
{8, 3, 568, 0, 1, gScenarioBufferData25_APU_DRAW_SEGMENTS_RGB, 48, gScenarioKernelData25_APU_DRAW_SEGMENTS_RGB, 4},
{10, 3, 696, 0, 1, gScenarioBufferData26_APU_DRAW_SEGMENTS_RGB, 48, gScenarioKernelData26_APU_DRAW_SEGMENTS_RGB, 4},
{12, 3, 824, 0, 1, gScenarioBufferData27_APU_DRAW_SEGMENTS_RGB, 48, gScenarioKernelData27_APU_DRAW_SEGMENTS_RGB, 4},
{16, 3, 1080, 0, 1, gScenarioBufferData28_APU_DRAW_SEGMENTS_RGB, 48, gScenarioKernelData28_APU_DRAW_SEGMENTS_RGB, 4},
{20, 3, 1336, 0, 1, gScenarioBufferData29_APU_DRAW_SEGMENTS_RGB, 48, gScenarioKernelData29_APU_DRAW_SEGMENTS_RGB, 4},
{24, 3, 1592, 0, 1, gScenarioBufferData30_APU_DRAW_SEGMENTS_RGB, 48, gScenarioKernelData30_APU_DRAW_SEGMENTS_RGB, 4},
{32, 3, 2104, 0, 1, gScenarioBufferData31_APU_DRAW_SEGMENTS_RGB, 48, gScenarioKernelData31_APU_DRAW_SEGMENTS_RGB, 4},
{40, 3, 2616, 0, 1, gScenarioBufferData32_APU_DRAW_SEGMENTS_RGB, 48, gScenarioKernelData32_APU_DRAW_SEGMENTS_RGB, 4},
{2, 4, 240, 0, 1, gScenarioBufferData33_APU_DRAW_SEGMENTS_RGB, 48, gScenarioKernelData33_APU_DRAW_SEGMENTS_RGB, 4},
{4, 4, 408, 0, 1, gScenarioBufferData34_APU_DRAW_SEGMENTS_RGB, 48, gScenarioKernelData34_APU_DRAW_SEGMENTS_RGB, 4},
{6, 4, 576, 0, 1, gScenarioBufferData35_APU_DRAW_SEGMENTS_RGB, 48, gScenarioKernelData35_APU_DRAW_SEGMENTS_RGB, 4},
{8, 4, 744, 0, 1, gScenarioBufferData36_APU_DRAW_SEGMENTS_RGB, 48, gScenarioKernelData36_APU_DRAW_SEGMENTS_RGB, 4},
{10, 4, 912, 0, 1, gScenarioBufferData37_APU_DRAW_SEGMENTS_RGB, 48, gScenarioKernelData37_APU_DRAW_SEGMENTS_RGB, 4},
{12, 4, 1080, 0, 1, gScenarioBufferData38_APU_DRAW_SEGMENTS_RGB, 48, gScenarioKernelData38_APU_DRAW_SEGMENTS_RGB, 4},
{16, 4, 1416, 0, 1, gScenarioBufferData39_APU_DRAW_SEGMENTS_RGB, 48, gScenarioKernelData39_APU_DRAW_SEGMENTS_RGB, 4},
{20, 4, 1752, 0, 1, gScenarioBufferData40_APU_DRAW_SEGMENTS_RGB, 48, gScenarioKernelData40_APU_DRAW_SEGMENTS_RGB, 4},
{24, 4, 2088, 0, 1, gScenarioBufferData41_APU_DRAW_SEGMENTS_RGB, 48, gScenarioKernelData41_APU_DRAW_SEGMENTS_RGB, 4},
{32, 4, 2760, 0, 1, gScenarioBufferData42_APU_DRAW_SEGMENTS_RGB, 48, gScenarioKernelData42_APU_DRAW_SEGMENTS_RGB, 4},
{40, 4, 3432, 0, 1, gScenarioBufferData43_APU_DRAW_SEGMENTS_RGB, 48, gScenarioKernelData43_APU_DRAW_SEGMENTS_RGB, 4},
{2, 5, 296, 0, 1, gScenarioBufferData44_APU_DRAW_SEGMENTS_RGB, 48, gScenarioKernelData44_APU_DRAW_SEGMENTS_RGB, 4},
{4, 5, 504, 0, 1, gScenarioBufferData45_APU_DRAW_SEGMENTS_RGB, 48, gScenarioKernelData45_APU_DRAW_SEGMENTS_RGB, 4},
{6, 5, 712, 0, 1, gScenarioBufferData46_APU_DRAW_SEGMENTS_RGB, 48, gScenarioKernelData46_APU_DRAW_SEGMENTS_RGB, 4},
{8, 5, 920, 0, 1, gScenarioBufferData47_APU_DRAW_SEGMENTS_RGB, 48, gScenarioKernelData47_APU_DRAW_SEGMENTS_RGB, 4},
{10, 5, 1128, 0, 1, gScenarioBufferData48_APU_DRAW_SEGMENTS_RGB, 48, gScenarioKernelData48_APU_DRAW_SEGMENTS_RGB, 4},
{12, 5, 1336, 0, 1, gScenarioBufferData49_APU_DRAW_SEGMENTS_RGB, 48, gScenarioKernelData49_APU_DRAW_SEGMENTS_RGB, 4},
{16, 5, 1752, 0, 1, gScenarioBufferData50_APU_DRAW_SEGMENTS_RGB, 48, gScenarioKernelData50_APU_DRAW_SEGMENTS_RGB, 4},
{20, 5, 2168, 0, 1, gScenarioBufferData51_APU_DRAW_SEGMENTS_RGB, 48, gScenarioKernelData51_APU_DRAW_SEGMENTS_RGB, 4},
{24, 5, 2584, 0, 1, gScenarioBufferData52_APU_DRAW_SEGMENTS_RGB, 48, gScenarioKernelData52_APU_DRAW_SEGMENTS_RGB, 4},
{32, 5, 3416, 0, 1, gScenarioBufferData53_APU_DRAW_SEGMENTS_RGB, 48, gScenarioKernelData53_APU_DRAW_SEGMENTS_RGB, 4},
{2, 6, 352, 0, 1, gScenarioBufferData54_APU_DRAW_SEGMENTS_RGB, 48, gScenarioKernelData54_APU_DRAW_SEGMENTS_RGB, 4},
{4, 6, 600, 0, 1, gScenarioBufferData55_APU_DRAW_SEGMENTS_RGB, 48, gScenarioKernelData55_APU_DRAW_SEGMENTS_RGB, 4},
{6, 6, 848, 0, 1, gScenarioBufferData56_APU_DRAW_SEGMENTS_RGB, 48, gScenarioKernelData56_APU_DRAW_SEGMENTS_RGB, 4},
{8, 6, 1096, 0, 1, gScenarioBufferData57_APU_DRAW_SEGMENTS_RGB, 48, gScenarioKernelData57_APU_DRAW_SEGMENTS_RGB, 4},
{10, 6, 1344, 0, 1, gScenarioBufferData58_APU_DRAW_SEGMENTS_RGB, 48, gScenarioKernelData58_APU_DRAW_SEGMENTS_RGB, 4},
{12, 6, 1592, 0, 1, gScenarioBufferData59_APU_DRAW_SEGMENTS_RGB, 48, gScenarioKernelData59_APU_DRAW_SEGMENTS_RGB, 4},
{16, 6, 2088, 0, 1, gScenarioBufferData60_APU_DRAW_SEGMENTS_RGB, 48, gScenarioKernelData60_APU_DRAW_SEGMENTS_RGB, 4},
{20, 6, 2584, 0, 1, gScenarioBufferData61_APU_DRAW_SEGMENTS_RGB, 48, gScenarioKernelData61_APU_DRAW_SEGMENTS_RGB, 4},
{24, 6, 3080, 0, 1, gScenarioBufferData62_APU_DRAW_SEGMENTS_RGB, 48, gScenarioKernelData62_APU_DRAW_SEGMENTS_RGB, 4},
{2, 8, 464, 0, 1, gScenarioBufferData63_APU_DRAW_SEGMENTS_RGB, 48, gScenarioKernelData63_APU_DRAW_SEGMENTS_RGB, 4},
{4, 8, 792, 0, 1, gScenarioBufferData64_APU_DRAW_SEGMENTS_RGB, 48, gScenarioKernelData64_APU_DRAW_SEGMENTS_RGB, 4},
{6, 8, 1120, 0, 1, gScenarioBufferData65_APU_DRAW_SEGMENTS_RGB, 48, gScenarioKernelData65_APU_DRAW_SEGMENTS_RGB, 4},
{8, 8, 1448, 0, 1, gScenarioBufferData66_APU_DRAW_SEGMENTS_RGB, 48, gScenarioKernelData66_APU_DRAW_SEGMENTS_RGB, 4},
{10, 8, 1776, 0, 1, gScenarioBufferData67_APU_DRAW_SEGMENTS_RGB, 48, gScenarioKernelData67_APU_DRAW_SEGMENTS_RGB, 4},
{12, 8, 2104, 0, 1, gScenarioBufferData68_APU_DRAW_SEGMENTS_RGB, 48, gScenarioKernelData68_APU_DRAW_SEGMENTS_RGB, 4},
{16, 8, 2760, 0, 1, gScenarioBufferData69_APU_DRAW_SEGMENTS_RGB, 48, gScenarioKernelData69_APU_DRAW_SEGMENTS_RGB, 4},
{20, 8, 3416, 0, 1, gScenarioBufferData70_APU_DRAW_SEGMENTS_RGB, 48, gScenarioKernelData70_APU_DRAW_SEGMENTS_RGB, 4},
{2, 10, 576, 0, 1, gScenarioBufferData71_APU_DRAW_SEGMENTS_RGB, 48, gScenarioKernelData71_APU_DRAW_SEGMENTS_RGB, 4},
{4, 10, 984, 0, 1, gScenarioBufferData72_APU_DRAW_SEGMENTS_RGB, 48, gScenarioKernelData72_APU_DRAW_SEGMENTS_RGB, 4},
{6, 10, 1392, 0, 1, gScenarioBufferData73_APU_DRAW_SEGMENTS_RGB, 48, gScenarioKernelData73_APU_DRAW_SEGMENTS_RGB, 4},
{8, 10, 1800, 0, 1, gScenarioBufferData74_APU_DRAW_SEGMENTS_RGB, 48, gScenarioKernelData74_APU_DRAW_SEGMENTS_RGB, 4},
{10, 10, 2208, 0, 1, gScenarioBufferData75_APU_DRAW_SEGMENTS_RGB, 48, gScenarioKernelData75_APU_DRAW_SEGMENTS_RGB, 4},
{12, 10, 2616, 0, 1, gScenarioBufferData76_APU_DRAW_SEGMENTS_RGB, 48, gScenarioKernelData76_APU_DRAW_SEGMENTS_RGB, 4},
{16, 10, 3432, 0, 1, gScenarioBufferData77_APU_DRAW_SEGMENTS_RGB, 48, gScenarioKernelData77_APU_DRAW_SEGMENTS_RGB, 4},
{2, 12, 688, 0, 1, gScenarioBufferData78_APU_DRAW_SEGMENTS_RGB, 48, gScenarioKernelData78_APU_DRAW_SEGMENTS_RGB, 4},
{4, 12, 1176, 0, 1, gScenarioBufferData79_APU_DRAW_SEGMENTS_RGB, 48, gScenarioKernelData79_APU_DRAW_SEGMENTS_RGB, 4},
{6, 12, 1664, 0, 1, gScenarioBufferData80_APU_DRAW_SEGMENTS_RGB, 48, gScenarioKernelData80_APU_DRAW_SEGMENTS_RGB, 4},
{8, 12, 2152, 0, 1, gScenarioBufferData81_APU_DRAW_SEGMENTS_RGB, 48, gScenarioKernelData81_APU_DRAW_SEGMENTS_RGB, 4},
{10, 12, 2640, 0, 1, gScenarioBufferData82_APU_DRAW_SEGMENTS_RGB, 48, gScenarioKernelData82_APU_DRAW_SEGMENTS_RGB, 4},
{12, 12, 3128, 0, 1, gScenarioBufferData83_APU_DRAW_SEGMENTS_RGB, 48, gScenarioKernelData83_APU_DRAW_SEGMENTS_RGB, 4},
{2, 14, 800, 0, 1, gScenarioBufferData84_APU_DRAW_SEGMENTS_RGB, 48, gScenarioKernelData84_APU_DRAW_SEGMENTS_RGB, 4},
{4, 14, 1368, 0, 1, gScenarioBufferData85_APU_DRAW_SEGMENTS_RGB, 48, gScenarioKernelData85_APU_DRAW_SEGMENTS_RGB, 4},
{6, 14, 1936, 0, 1, gScenarioBufferData86_APU_DRAW_SEGMENTS_RGB, 48, gScenarioKernelData86_APU_DRAW_SEGMENTS_RGB, 4},
{8, 14, 2504, 0, 1, gScenarioBufferData87_APU_DRAW_SEGMENTS_RGB, 48, gScenarioKernelData87_APU_DRAW_SEGMENTS_RGB, 4},
{10, 14, 3072, 0, 1, gScenarioBufferData88_APU_DRAW_SEGMENTS_RGB, 48, gScenarioKernelData88_APU_DRAW_SEGMENTS_RGB, 4},
{12, 14, 3640, 0, 1, gScenarioBufferData89_APU_DRAW_SEGMENTS_RGB, 48, gScenarioKernelData89_APU_DRAW_SEGMENTS_RGB, 4},
{2, 16, 912, 0, 1, gScenarioBufferData90_APU_DRAW_SEGMENTS_RGB, 48, gScenarioKernelData90_APU_DRAW_SEGMENTS_RGB, 4},
{4, 16, 1560, 0, 1, gScenarioBufferData91_APU_DRAW_SEGMENTS_RGB, 48, gScenarioKernelData91_APU_DRAW_SEGMENTS_RGB, 4},
{6, 16, 2208, 0, 1, gScenarioBufferData92_APU_DRAW_SEGMENTS_RGB, 48, gScenarioKernelData92_APU_DRAW_SEGMENTS_RGB, 4},
{8, 16, 2856, 0, 1, gScenarioBufferData93_APU_DRAW_SEGMENTS_RGB, 48, gScenarioKernelData93_APU_DRAW_SEGMENTS_RGB, 4},
{10, 16, 3504, 0, 1, gScenarioBufferData94_APU_DRAW_SEGMENTS_RGB, 48, gScenarioKernelData94_APU_DRAW_SEGMENTS_RGB, 4},
{2, 18, 1024, 0, 1, gScenarioBufferData95_APU_DRAW_SEGMENTS_RGB, 48, gScenarioKernelData95_APU_DRAW_SEGMENTS_RGB, 4},
{4, 18, 1752, 0, 1, gScenarioBufferData96_APU_DRAW_SEGMENTS_RGB, 48, gScenarioKernelData96_APU_DRAW_SEGMENTS_RGB, 4},
{6, 18, 2480, 0, 1, gScenarioBufferData97_APU_DRAW_SEGMENTS_RGB, 48, gScenarioKernelData97_APU_DRAW_SEGMENTS_RGB, 4},
{8, 18, 3208, 0, 1, gScenarioBufferData98_APU_DRAW_SEGMENTS_RGB, 48, gScenarioKernelData98_APU_DRAW_SEGMENTS_RGB, 4},
{2, 20, 1136, 0, 1, gScenarioBufferData99_APU_DRAW_SEGMENTS_RGB, 48, gScenarioKernelData99_APU_DRAW_SEGMENTS_RGB, 4},
{4, 20, 1944, 0, 1, gScenarioBufferData100_APU_DRAW_SEGMENTS_RGB, 48, gScenarioKernelData100_APU_DRAW_SEGMENTS_RGB, 4},
{6, 20, 2752, 0, 1, gScenarioBufferData101_APU_DRAW_SEGMENTS_RGB, 48, gScenarioKernelData101_APU_DRAW_SEGMENTS_RGB, 4},
{8, 20, 3560, 0, 1, gScenarioBufferData102_APU_DRAW_SEGMENTS_RGB, 48, gScenarioKernelData102_APU_DRAW_SEGMENTS_RGB, 4},
{2, 22, 1248, 0, 1, gScenarioBufferData103_APU_DRAW_SEGMENTS_RGB, 48, gScenarioKernelData103_APU_DRAW_SEGMENTS_RGB, 4},
{4, 22, 2136, 0, 1, gScenarioBufferData104_APU_DRAW_SEGMENTS_RGB, 48, gScenarioKernelData104_APU_DRAW_SEGMENTS_RGB, 4},
{6, 22, 3024, 0, 1, gScenarioBufferData105_APU_DRAW_SEGMENTS_RGB, 48, gScenarioKernelData105_APU_DRAW_SEGMENTS_RGB, 4},
{2, 24, 1360, 0, 1, gScenarioBufferData106_APU_DRAW_SEGMENTS_RGB, 48, gScenarioKernelData106_APU_DRAW_SEGMENTS_RGB, 4},
{4, 24, 2328, 0, 1, gScenarioBufferData107_APU_DRAW_SEGMENTS_RGB, 48, gScenarioKernelData107_APU_DRAW_SEGMENTS_RGB, 4},
{6, 24, 3296, 0, 1, gScenarioBufferData108_APU_DRAW_SEGMENTS_RGB, 48, gScenarioKernelData108_APU_DRAW_SEGMENTS_RGB, 4},
{2, 26, 1472, 0, 1, gScenarioBufferData109_APU_DRAW_SEGMENTS_RGB, 48, gScenarioKernelData109_APU_DRAW_SEGMENTS_RGB, 4},
{4, 26, 2520, 0, 1, gScenarioBufferData110_APU_DRAW_SEGMENTS_RGB, 48, gScenarioKernelData110_APU_DRAW_SEGMENTS_RGB, 4},
{6, 26, 3568, 0, 1, gScenarioBufferData111_APU_DRAW_SEGMENTS_RGB, 48, gScenarioKernelData111_APU_DRAW_SEGMENTS_RGB, 4},
{2, 28, 1584, 0, 1, gScenarioBufferData112_APU_DRAW_SEGMENTS_RGB, 48, gScenarioKernelData112_APU_DRAW_SEGMENTS_RGB, 4},
{4, 28, 2712, 0, 1, gScenarioBufferData113_APU_DRAW_SEGMENTS_RGB, 48, gScenarioKernelData113_APU_DRAW_SEGMENTS_RGB, 4},
{6, 28, 3840, 0, 1, gScenarioBufferData114_APU_DRAW_SEGMENTS_RGB, 48, gScenarioKernelData114_APU_DRAW_SEGMENTS_RGB, 4},
{2, 30, 1696, 0, 1, gScenarioBufferData115_APU_DRAW_SEGMENTS_RGB, 48, gScenarioKernelData115_APU_DRAW_SEGMENTS_RGB, 4},
{4, 30, 2904, 0, 1, gScenarioBufferData116_APU_DRAW_SEGMENTS_RGB, 48, gScenarioKernelData116_APU_DRAW_SEGMENTS_RGB, 4},
{2, 32, 1808, 0, 1, gScenarioBufferData117_APU_DRAW_SEGMENTS_RGB, 48, gScenarioKernelData117_APU_DRAW_SEGMENTS_RGB, 4},
{4, 32, 3096, 0, 1, gScenarioBufferData118_APU_DRAW_SEGMENTS_RGB, 48, gScenarioKernelData118_APU_DRAW_SEGMENTS_RGB, 4}
};

static acf_scenario_list gScenarioList_APU_DRAW_SEGMENTS_RGB = {
119, //number of scenarios
gScenarioArray_APU_DRAW_SEGMENTS_RGB};
//**************************************************************

class APU_DRAW_SEGMENTS_RGB : public ACF_Process_APU
{

public:
   APU_DRAW_SEGMENTS_RGB(int32_t apex_id = 0) : ACF_Process_APU(apex_id)
   {} 

   int32_t Initialize()
   {
      int32_t lRetVal = 0;

      if (!Initialized()) //initialization steps that only need to occur once
      {
         SetProcessIdentifier("APU_DRAW_SEGMENTS_RGB");
         SetApuLoadInfo(APU_DRAW_SEGMENTS_RGB_LOAD_SEGMENTS,
                        APU_DRAW_SEGMENTS_RGB_LOAD_PMEM, APU_DRAW_SEGMENTS_RGB_LOAD_PMEM_SIZE,
                        APU_DRAW_SEGMENTS_RGB_LOAD_DMEM, APU_DRAW_SEGMENTS_RGB_LOAD_DMEM_SIZE,
                        0, 0); //assuming _LOAD_CMEM does not exist

         lRetVal = NonParametricInitialization();
         if (lRetVal != 0) return lRetVal;

         FlagSpatialDep();

         //***NOTE: the order in which the following ports are added is meaningful; do not change!

         AddPort("INPUT_0", icp::DATATYPE_16U, 1, 1, 2, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0);
         AddPort("INPUT_1", icp::DATATYPE_08U, 3, 1, 2, 1, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0);
         AddPort("OUTPUT_0", icp::DATATYPE_08U, 3, 1, 2, 1, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0);

         CfgScenarios(&gScenarioList_APU_DRAW_SEGMENTS_RGB);

         FlagAsInitialized();
      }

      lRetVal = SelectApuConfiguration(mApuCfg, mApexId); //by default mApuCfg = ACF_APU_CFG__APEX0_DEFAULT and mApexId = 0

      return lRetVal;
   }
};

#endif //_ACF_PROCESS_APU_APU_DRAW_SEGMENTS_RGB
