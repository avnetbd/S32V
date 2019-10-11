#ifndef _ACF_PROCESS_APU_APU_FAST9_COLOR
#define _ACF_PROCESS_APU_APU_FAST9_COLOR

#include <acf_process_apu.h>
#include <APU_FAST9_COLOR_APU_LOAD.h> //APU load associated with this process
#include <stdint.h>


//SCENARIO LIST*************************************************
static acf_scenario_buffer_data gScenarioBufferData0_APU_FAST9_COLOR[] = {{4, 1, 11, 0, {0, 0, 0, 0}}, {4, 1, 4, 0, {1, 1, 2, 2}}, {4, 1, 10, 1, {3, 3, 4, 4}}, {4, 1, 4, 4, {1, 1, 2, 2}}, {4, 1, 7, 5, {2, 2, 2, 2}}, {4, 1, 7, 7, {2, 2, 2, 2}}, {4, 1, 1, 9, {0, 0, 0, 0}}, {4, 1, 1, 9, {0, 0, 0, 0}}, {4, 1, 2, 9, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData1_APU_FAST9_COLOR[] = {{6, 1, 11, 0, {0, 0, 0, 0}}, {6, 1, 4, 0, {1, 1, 2, 2}}, {6, 1, 10, 1, {3, 3, 4, 4}}, {6, 1, 4, 4, {1, 1, 2, 2}}, {6, 1, 7, 5, {2, 2, 2, 2}}, {6, 1, 7, 7, {2, 2, 2, 2}}, {6, 1, 1, 9, {0, 0, 0, 0}}, {6, 1, 1, 9, {0, 0, 0, 0}}, {6, 1, 2, 9, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData2_APU_FAST9_COLOR[] = {{8, 1, 11, 0, {0, 0, 0, 0}}, {8, 1, 4, 0, {1, 1, 2, 2}}, {8, 1, 10, 1, {3, 3, 4, 4}}, {8, 1, 4, 4, {1, 1, 2, 2}}, {8, 1, 7, 5, {2, 2, 2, 2}}, {8, 1, 7, 7, {2, 2, 2, 2}}, {8, 1, 1, 9, {0, 0, 0, 0}}, {8, 1, 1, 9, {0, 0, 0, 0}}, {8, 1, 2, 9, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData3_APU_FAST9_COLOR[] = {{10, 1, 11, 0, {0, 0, 0, 0}}, {10, 1, 4, 0, {1, 1, 2, 2}}, {10, 1, 10, 1, {3, 3, 4, 4}}, {10, 1, 4, 4, {1, 1, 2, 2}}, {10, 1, 7, 5, {2, 2, 2, 2}}, {10, 1, 7, 7, {2, 2, 2, 2}}, {10, 1, 1, 9, {0, 0, 0, 0}}, {10, 1, 1, 9, {0, 0, 0, 0}}, {10, 1, 2, 9, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData4_APU_FAST9_COLOR[] = {{12, 1, 11, 0, {0, 0, 0, 0}}, {12, 1, 4, 0, {1, 1, 2, 2}}, {12, 1, 10, 1, {3, 3, 4, 4}}, {12, 1, 4, 4, {1, 1, 2, 2}}, {12, 1, 7, 5, {2, 2, 2, 2}}, {12, 1, 7, 7, {2, 2, 2, 2}}, {12, 1, 1, 9, {0, 0, 0, 0}}, {12, 1, 1, 9, {0, 0, 0, 0}}, {12, 1, 2, 9, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData5_APU_FAST9_COLOR[] = {{16, 1, 11, 0, {0, 0, 0, 0}}, {16, 1, 4, 0, {1, 1, 2, 2}}, {16, 1, 10, 1, {3, 3, 4, 4}}, {16, 1, 4, 4, {1, 1, 2, 2}}, {16, 1, 7, 5, {2, 2, 2, 2}}, {16, 1, 7, 7, {2, 2, 2, 2}}, {16, 1, 1, 9, {0, 0, 0, 0}}, {16, 1, 1, 9, {0, 0, 0, 0}}, {16, 1, 2, 9, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData6_APU_FAST9_COLOR[] = {{20, 1, 11, 0, {0, 0, 0, 0}}, {20, 1, 4, 0, {1, 1, 2, 2}}, {20, 1, 10, 1, {3, 3, 4, 4}}, {20, 1, 4, 4, {1, 1, 2, 2}}, {20, 1, 7, 5, {2, 2, 2, 2}}, {20, 1, 7, 7, {2, 2, 2, 2}}, {20, 1, 1, 9, {0, 0, 0, 0}}, {20, 1, 1, 9, {0, 0, 0, 0}}, {20, 1, 2, 9, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData7_APU_FAST9_COLOR[] = {{24, 1, 11, 0, {0, 0, 0, 0}}, {24, 1, 4, 0, {1, 1, 2, 2}}, {24, 1, 10, 1, {3, 3, 4, 4}}, {24, 1, 4, 4, {1, 1, 2, 2}}, {24, 1, 7, 5, {2, 2, 2, 2}}, {24, 1, 7, 7, {2, 2, 2, 2}}, {24, 1, 1, 9, {0, 0, 0, 0}}, {24, 1, 1, 9, {0, 0, 0, 0}}, {24, 1, 2, 9, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData8_APU_FAST9_COLOR[] = {{32, 1, 11, 0, {0, 0, 0, 0}}, {32, 1, 4, 0, {1, 1, 2, 2}}, {32, 1, 10, 1, {3, 3, 4, 4}}, {32, 1, 4, 4, {1, 1, 2, 2}}, {32, 1, 7, 5, {2, 2, 2, 2}}, {32, 1, 7, 7, {2, 2, 2, 2}}, {32, 1, 1, 9, {0, 0, 0, 0}}, {32, 1, 1, 9, {0, 0, 0, 0}}, {32, 1, 2, 9, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData9_APU_FAST9_COLOR[] = {{4, 2, 8, 0, {0, 0, 0, 0}}, {4, 2, 4, 0, {1, 1, 2, 2}}, {4, 2, 7, 1, {3, 3, 4, 4}}, {4, 2, 4, 3, {1, 1, 2, 2}}, {4, 2, 4, 4, {2, 2, 2, 2}}, {4, 2, 4, 5, {2, 2, 2, 2}}, {4, 2, 1, 6, {0, 0, 0, 0}}, {4, 2, 1, 6, {0, 0, 0, 0}}, {4, 2, 2, 6, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData10_APU_FAST9_COLOR[] = {{6, 2, 8, 0, {0, 0, 0, 0}}, {6, 2, 4, 0, {1, 1, 2, 2}}, {6, 2, 7, 1, {3, 3, 4, 4}}, {6, 2, 4, 3, {1, 1, 2, 2}}, {6, 2, 4, 4, {2, 2, 2, 2}}, {6, 2, 4, 5, {2, 2, 2, 2}}, {6, 2, 1, 6, {0, 0, 0, 0}}, {6, 2, 1, 6, {0, 0, 0, 0}}, {6, 2, 2, 6, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData11_APU_FAST9_COLOR[] = {{8, 2, 8, 0, {0, 0, 0, 0}}, {8, 2, 4, 0, {1, 1, 2, 2}}, {8, 2, 7, 1, {3, 3, 4, 4}}, {8, 2, 4, 3, {1, 1, 2, 2}}, {8, 2, 4, 4, {2, 2, 2, 2}}, {8, 2, 4, 5, {2, 2, 2, 2}}, {8, 2, 1, 6, {0, 0, 0, 0}}, {8, 2, 1, 6, {0, 0, 0, 0}}, {8, 2, 2, 6, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData12_APU_FAST9_COLOR[] = {{10, 2, 8, 0, {0, 0, 0, 0}}, {10, 2, 4, 0, {1, 1, 2, 2}}, {10, 2, 7, 1, {3, 3, 4, 4}}, {10, 2, 4, 3, {1, 1, 2, 2}}, {10, 2, 4, 4, {2, 2, 2, 2}}, {10, 2, 4, 5, {2, 2, 2, 2}}, {10, 2, 1, 6, {0, 0, 0, 0}}, {10, 2, 1, 6, {0, 0, 0, 0}}, {10, 2, 2, 6, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData13_APU_FAST9_COLOR[] = {{12, 2, 8, 0, {0, 0, 0, 0}}, {12, 2, 4, 0, {1, 1, 2, 2}}, {12, 2, 7, 1, {3, 3, 4, 4}}, {12, 2, 4, 3, {1, 1, 2, 2}}, {12, 2, 4, 4, {2, 2, 2, 2}}, {12, 2, 4, 5, {2, 2, 2, 2}}, {12, 2, 1, 6, {0, 0, 0, 0}}, {12, 2, 1, 6, {0, 0, 0, 0}}, {12, 2, 2, 6, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData14_APU_FAST9_COLOR[] = {{16, 2, 8, 0, {0, 0, 0, 0}}, {16, 2, 4, 0, {1, 1, 2, 2}}, {16, 2, 7, 1, {3, 3, 4, 4}}, {16, 2, 4, 3, {1, 1, 2, 2}}, {16, 2, 4, 4, {2, 2, 2, 2}}, {16, 2, 4, 5, {2, 2, 2, 2}}, {16, 2, 1, 6, {0, 0, 0, 0}}, {16, 2, 1, 6, {0, 0, 0, 0}}, {16, 2, 2, 6, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData15_APU_FAST9_COLOR[] = {{20, 2, 8, 0, {0, 0, 0, 0}}, {20, 2, 4, 0, {1, 1, 2, 2}}, {20, 2, 7, 1, {3, 3, 4, 4}}, {20, 2, 4, 3, {1, 1, 2, 2}}, {20, 2, 4, 4, {2, 2, 2, 2}}, {20, 2, 4, 5, {2, 2, 2, 2}}, {20, 2, 1, 6, {0, 0, 0, 0}}, {20, 2, 1, 6, {0, 0, 0, 0}}, {20, 2, 2, 6, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData16_APU_FAST9_COLOR[] = {{24, 2, 8, 0, {0, 0, 0, 0}}, {24, 2, 4, 0, {1, 1, 2, 2}}, {24, 2, 7, 1, {3, 3, 4, 4}}, {24, 2, 4, 3, {1, 1, 2, 2}}, {24, 2, 4, 4, {2, 2, 2, 2}}, {24, 2, 4, 5, {2, 2, 2, 2}}, {24, 2, 1, 6, {0, 0, 0, 0}}, {24, 2, 1, 6, {0, 0, 0, 0}}, {24, 2, 2, 6, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData17_APU_FAST9_COLOR[] = {{4, 3, 7, 0, {0, 0, 0, 0}}, {4, 3, 4, 0, {1, 1, 2, 2}}, {4, 3, 4, 1, {3, 3, 4, 4}}, {4, 3, 4, 2, {1, 1, 2, 2}}, {4, 3, 4, 3, {2, 2, 2, 2}}, {4, 3, 4, 4, {2, 2, 2, 2}}, {4, 3, 1, 5, {0, 0, 0, 0}}, {4, 3, 1, 5, {0, 0, 0, 0}}, {4, 3, 2, 5, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData18_APU_FAST9_COLOR[] = {{6, 3, 7, 0, {0, 0, 0, 0}}, {6, 3, 4, 0, {1, 1, 2, 2}}, {6, 3, 4, 1, {3, 3, 4, 4}}, {6, 3, 4, 2, {1, 1, 2, 2}}, {6, 3, 4, 3, {2, 2, 2, 2}}, {6, 3, 4, 4, {2, 2, 2, 2}}, {6, 3, 1, 5, {0, 0, 0, 0}}, {6, 3, 1, 5, {0, 0, 0, 0}}, {6, 3, 2, 5, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData19_APU_FAST9_COLOR[] = {{8, 3, 7, 0, {0, 0, 0, 0}}, {8, 3, 4, 0, {1, 1, 2, 2}}, {8, 3, 4, 1, {3, 3, 4, 4}}, {8, 3, 4, 2, {1, 1, 2, 2}}, {8, 3, 4, 3, {2, 2, 2, 2}}, {8, 3, 4, 4, {2, 2, 2, 2}}, {8, 3, 1, 5, {0, 0, 0, 0}}, {8, 3, 1, 5, {0, 0, 0, 0}}, {8, 3, 2, 5, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData20_APU_FAST9_COLOR[] = {{10, 3, 7, 0, {0, 0, 0, 0}}, {10, 3, 4, 0, {1, 1, 2, 2}}, {10, 3, 4, 1, {3, 3, 4, 4}}, {10, 3, 4, 2, {1, 1, 2, 2}}, {10, 3, 4, 3, {2, 2, 2, 2}}, {10, 3, 4, 4, {2, 2, 2, 2}}, {10, 3, 1, 5, {0, 0, 0, 0}}, {10, 3, 1, 5, {0, 0, 0, 0}}, {10, 3, 2, 5, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData21_APU_FAST9_COLOR[] = {{12, 3, 7, 0, {0, 0, 0, 0}}, {12, 3, 4, 0, {1, 1, 2, 2}}, {12, 3, 4, 1, {3, 3, 4, 4}}, {12, 3, 4, 2, {1, 1, 2, 2}}, {12, 3, 4, 3, {2, 2, 2, 2}}, {12, 3, 4, 4, {2, 2, 2, 2}}, {12, 3, 1, 5, {0, 0, 0, 0}}, {12, 3, 1, 5, {0, 0, 0, 0}}, {12, 3, 2, 5, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData22_APU_FAST9_COLOR[] = {{16, 3, 7, 0, {0, 0, 0, 0}}, {16, 3, 4, 0, {1, 1, 2, 2}}, {16, 3, 4, 1, {3, 3, 4, 4}}, {16, 3, 4, 2, {1, 1, 2, 2}}, {16, 3, 4, 3, {2, 2, 2, 2}}, {16, 3, 4, 4, {2, 2, 2, 2}}, {16, 3, 1, 5, {0, 0, 0, 0}}, {16, 3, 1, 5, {0, 0, 0, 0}}, {16, 3, 2, 5, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData23_APU_FAST9_COLOR[] = {{20, 3, 7, 0, {0, 0, 0, 0}}, {20, 3, 4, 0, {1, 1, 2, 2}}, {20, 3, 4, 1, {3, 3, 4, 4}}, {20, 3, 4, 2, {1, 1, 2, 2}}, {20, 3, 4, 3, {2, 2, 2, 2}}, {20, 3, 4, 4, {2, 2, 2, 2}}, {20, 3, 1, 5, {0, 0, 0, 0}}, {20, 3, 1, 5, {0, 0, 0, 0}}, {20, 3, 2, 5, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData24_APU_FAST9_COLOR[] = {{4, 4, 7, 0, {0, 0, 0, 0}}, {4, 4, 4, 0, {1, 1, 2, 2}}, {4, 4, 4, 1, {3, 3, 4, 4}}, {4, 4, 4, 2, {1, 1, 2, 2}}, {4, 4, 4, 3, {2, 2, 2, 2}}, {4, 4, 4, 4, {2, 2, 2, 2}}, {4, 4, 1, 5, {0, 0, 0, 0}}, {4, 4, 1, 5, {0, 0, 0, 0}}, {4, 4, 2, 5, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData25_APU_FAST9_COLOR[] = {{6, 4, 7, 0, {0, 0, 0, 0}}, {6, 4, 4, 0, {1, 1, 2, 2}}, {6, 4, 4, 1, {3, 3, 4, 4}}, {6, 4, 4, 2, {1, 1, 2, 2}}, {6, 4, 4, 3, {2, 2, 2, 2}}, {6, 4, 4, 4, {2, 2, 2, 2}}, {6, 4, 1, 5, {0, 0, 0, 0}}, {6, 4, 1, 5, {0, 0, 0, 0}}, {6, 4, 2, 5, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData26_APU_FAST9_COLOR[] = {{8, 4, 7, 0, {0, 0, 0, 0}}, {8, 4, 4, 0, {1, 1, 2, 2}}, {8, 4, 4, 1, {3, 3, 4, 4}}, {8, 4, 4, 2, {1, 1, 2, 2}}, {8, 4, 4, 3, {2, 2, 2, 2}}, {8, 4, 4, 4, {2, 2, 2, 2}}, {8, 4, 1, 5, {0, 0, 0, 0}}, {8, 4, 1, 5, {0, 0, 0, 0}}, {8, 4, 2, 5, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData27_APU_FAST9_COLOR[] = {{10, 4, 7, 0, {0, 0, 0, 0}}, {10, 4, 4, 0, {1, 1, 2, 2}}, {10, 4, 4, 1, {3, 3, 4, 4}}, {10, 4, 4, 2, {1, 1, 2, 2}}, {10, 4, 4, 3, {2, 2, 2, 2}}, {10, 4, 4, 4, {2, 2, 2, 2}}, {10, 4, 1, 5, {0, 0, 0, 0}}, {10, 4, 1, 5, {0, 0, 0, 0}}, {10, 4, 2, 5, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData28_APU_FAST9_COLOR[] = {{12, 4, 7, 0, {0, 0, 0, 0}}, {12, 4, 4, 0, {1, 1, 2, 2}}, {12, 4, 4, 1, {3, 3, 4, 4}}, {12, 4, 4, 2, {1, 1, 2, 2}}, {12, 4, 4, 3, {2, 2, 2, 2}}, {12, 4, 4, 4, {2, 2, 2, 2}}, {12, 4, 1, 5, {0, 0, 0, 0}}, {12, 4, 1, 5, {0, 0, 0, 0}}, {12, 4, 2, 5, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData29_APU_FAST9_COLOR[] = {{4, 5, 7, 0, {0, 0, 0, 0}}, {4, 5, 4, 0, {1, 1, 2, 2}}, {4, 5, 4, 1, {3, 3, 4, 4}}, {4, 5, 4, 2, {1, 1, 2, 2}}, {4, 5, 4, 3, {2, 2, 2, 2}}, {4, 5, 4, 4, {2, 2, 2, 2}}, {4, 5, 1, 5, {0, 0, 0, 0}}, {4, 5, 1, 5, {0, 0, 0, 0}}, {4, 5, 2, 5, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData30_APU_FAST9_COLOR[] = {{6, 5, 7, 0, {0, 0, 0, 0}}, {6, 5, 4, 0, {1, 1, 2, 2}}, {6, 5, 4, 1, {3, 3, 4, 4}}, {6, 5, 4, 2, {1, 1, 2, 2}}, {6, 5, 4, 3, {2, 2, 2, 2}}, {6, 5, 4, 4, {2, 2, 2, 2}}, {6, 5, 1, 5, {0, 0, 0, 0}}, {6, 5, 1, 5, {0, 0, 0, 0}}, {6, 5, 2, 5, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData31_APU_FAST9_COLOR[] = {{8, 5, 7, 0, {0, 0, 0, 0}}, {8, 5, 4, 0, {1, 1, 2, 2}}, {8, 5, 4, 1, {3, 3, 4, 4}}, {8, 5, 4, 2, {1, 1, 2, 2}}, {8, 5, 4, 3, {2, 2, 2, 2}}, {8, 5, 4, 4, {2, 2, 2, 2}}, {8, 5, 1, 5, {0, 0, 0, 0}}, {8, 5, 1, 5, {0, 0, 0, 0}}, {8, 5, 2, 5, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData32_APU_FAST9_COLOR[] = {{10, 5, 7, 0, {0, 0, 0, 0}}, {10, 5, 4, 0, {1, 1, 2, 2}}, {10, 5, 4, 1, {3, 3, 4, 4}}, {10, 5, 4, 2, {1, 1, 2, 2}}, {10, 5, 4, 3, {2, 2, 2, 2}}, {10, 5, 4, 4, {2, 2, 2, 2}}, {10, 5, 1, 5, {0, 0, 0, 0}}, {10, 5, 1, 5, {0, 0, 0, 0}}, {10, 5, 2, 5, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData33_APU_FAST9_COLOR[] = {{12, 5, 7, 0, {0, 0, 0, 0}}, {12, 5, 4, 0, {1, 1, 2, 2}}, {12, 5, 4, 1, {3, 3, 4, 4}}, {12, 5, 4, 2, {1, 1, 2, 2}}, {12, 5, 4, 3, {2, 2, 2, 2}}, {12, 5, 4, 4, {2, 2, 2, 2}}, {12, 5, 1, 5, {0, 0, 0, 0}}, {12, 5, 1, 5, {0, 0, 0, 0}}, {12, 5, 2, 5, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData34_APU_FAST9_COLOR[] = {{4, 6, 7, 0, {0, 0, 0, 0}}, {4, 6, 4, 0, {1, 1, 2, 2}}, {4, 6, 4, 1, {3, 3, 4, 4}}, {4, 6, 4, 2, {1, 1, 2, 2}}, {4, 6, 4, 3, {2, 2, 2, 2}}, {4, 6, 4, 4, {2, 2, 2, 2}}, {4, 6, 1, 5, {0, 0, 0, 0}}, {4, 6, 1, 5, {0, 0, 0, 0}}, {4, 6, 2, 5, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData35_APU_FAST9_COLOR[] = {{6, 6, 7, 0, {0, 0, 0, 0}}, {6, 6, 4, 0, {1, 1, 2, 2}}, {6, 6, 4, 1, {3, 3, 4, 4}}, {6, 6, 4, 2, {1, 1, 2, 2}}, {6, 6, 4, 3, {2, 2, 2, 2}}, {6, 6, 4, 4, {2, 2, 2, 2}}, {6, 6, 1, 5, {0, 0, 0, 0}}, {6, 6, 1, 5, {0, 0, 0, 0}}, {6, 6, 2, 5, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData36_APU_FAST9_COLOR[] = {{8, 6, 7, 0, {0, 0, 0, 0}}, {8, 6, 4, 0, {1, 1, 2, 2}}, {8, 6, 4, 1, {3, 3, 4, 4}}, {8, 6, 4, 2, {1, 1, 2, 2}}, {8, 6, 4, 3, {2, 2, 2, 2}}, {8, 6, 4, 4, {2, 2, 2, 2}}, {8, 6, 1, 5, {0, 0, 0, 0}}, {8, 6, 1, 5, {0, 0, 0, 0}}, {8, 6, 2, 5, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData37_APU_FAST9_COLOR[] = {{10, 6, 7, 0, {0, 0, 0, 0}}, {10, 6, 4, 0, {1, 1, 2, 2}}, {10, 6, 4, 1, {3, 3, 4, 4}}, {10, 6, 4, 2, {1, 1, 2, 2}}, {10, 6, 4, 3, {2, 2, 2, 2}}, {10, 6, 4, 4, {2, 2, 2, 2}}, {10, 6, 1, 5, {0, 0, 0, 0}}, {10, 6, 1, 5, {0, 0, 0, 0}}, {10, 6, 2, 5, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData38_APU_FAST9_COLOR[] = {{4, 8, 7, 0, {0, 0, 0, 0}}, {4, 8, 4, 0, {1, 1, 2, 2}}, {4, 8, 4, 1, {3, 3, 4, 4}}, {4, 8, 4, 2, {1, 1, 2, 2}}, {4, 8, 4, 3, {2, 2, 2, 2}}, {4, 8, 4, 4, {2, 2, 2, 2}}, {4, 8, 1, 5, {0, 0, 0, 0}}, {4, 8, 1, 5, {0, 0, 0, 0}}, {4, 8, 2, 5, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData39_APU_FAST9_COLOR[] = {{6, 8, 7, 0, {0, 0, 0, 0}}, {6, 8, 4, 0, {1, 1, 2, 2}}, {6, 8, 4, 1, {3, 3, 4, 4}}, {6, 8, 4, 2, {1, 1, 2, 2}}, {6, 8, 4, 3, {2, 2, 2, 2}}, {6, 8, 4, 4, {2, 2, 2, 2}}, {6, 8, 1, 5, {0, 0, 0, 0}}, {6, 8, 1, 5, {0, 0, 0, 0}}, {6, 8, 2, 5, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData40_APU_FAST9_COLOR[] = {{4, 10, 7, 0, {0, 0, 0, 0}}, {4, 10, 4, 0, {1, 1, 2, 2}}, {4, 10, 4, 1, {3, 3, 4, 4}}, {4, 10, 4, 2, {1, 1, 2, 2}}, {4, 10, 4, 3, {2, 2, 2, 2}}, {4, 10, 4, 4, {2, 2, 2, 2}}, {4, 10, 1, 5, {0, 0, 0, 0}}, {4, 10, 1, 5, {0, 0, 0, 0}}, {4, 10, 2, 5, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};
static acf_scenario_buffer_data gScenarioBufferData41_APU_FAST9_COLOR[] = {{4, 12, 7, 0, {0, 0, 0, 0}}, {4, 12, 4, 0, {1, 1, 2, 2}}, {4, 12, 4, 1, {3, 3, 4, 4}}, {4, 12, 4, 2, {1, 1, 2, 2}}, {4, 12, 4, 3, {2, 2, 2, 2}}, {4, 12, 4, 4, {2, 2, 2, 2}}, {4, 12, 1, 5, {0, 0, 0, 0}}, {4, 12, 1, 5, {0, 0, 0, 0}}, {4, 12, 2, 5, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}, {1, 1, 1, 0, {0, 0, 0, 0}}};

static acf_scenario_kernel_data gScenarioKernelData0_APU_FAST9_COLOR[] = {{0, 0}, {1, 0}, {4, 0}, {5, 0}, {7, 0}, {9, 0}, {9, 0}, {9, 0}};
static acf_scenario_kernel_data gScenarioKernelData1_APU_FAST9_COLOR[] = {{0, 0}, {1, 0}, {4, 0}, {5, 0}, {7, 0}, {9, 0}, {9, 0}, {9, 0}};
static acf_scenario_kernel_data gScenarioKernelData2_APU_FAST9_COLOR[] = {{0, 0}, {1, 0}, {4, 0}, {5, 0}, {7, 0}, {9, 0}, {9, 0}, {9, 0}};
static acf_scenario_kernel_data gScenarioKernelData3_APU_FAST9_COLOR[] = {{0, 0}, {1, 0}, {4, 0}, {5, 0}, {7, 0}, {9, 0}, {9, 0}, {9, 0}};
static acf_scenario_kernel_data gScenarioKernelData4_APU_FAST9_COLOR[] = {{0, 0}, {1, 0}, {4, 0}, {5, 0}, {7, 0}, {9, 0}, {9, 0}, {9, 0}};
static acf_scenario_kernel_data gScenarioKernelData5_APU_FAST9_COLOR[] = {{0, 0}, {1, 0}, {4, 0}, {5, 0}, {7, 0}, {9, 0}, {9, 0}, {9, 0}};
static acf_scenario_kernel_data gScenarioKernelData6_APU_FAST9_COLOR[] = {{0, 0}, {1, 0}, {4, 0}, {5, 0}, {7, 0}, {9, 0}, {9, 0}, {9, 0}};
static acf_scenario_kernel_data gScenarioKernelData7_APU_FAST9_COLOR[] = {{0, 0}, {1, 0}, {4, 0}, {5, 0}, {7, 0}, {9, 0}, {9, 0}, {9, 0}};
static acf_scenario_kernel_data gScenarioKernelData8_APU_FAST9_COLOR[] = {{0, 0}, {1, 0}, {4, 0}, {5, 0}, {7, 0}, {9, 0}, {9, 0}, {9, 0}};
static acf_scenario_kernel_data gScenarioKernelData9_APU_FAST9_COLOR[] = {{0, 0}, {1, 0}, {3, 0}, {4, 0}, {5, 0}, {6, 0}, {6, 0}, {6, 0}};
static acf_scenario_kernel_data gScenarioKernelData10_APU_FAST9_COLOR[] = {{0, 0}, {1, 0}, {3, 0}, {4, 0}, {5, 0}, {6, 0}, {6, 0}, {6, 0}};
static acf_scenario_kernel_data gScenarioKernelData11_APU_FAST9_COLOR[] = {{0, 0}, {1, 0}, {3, 0}, {4, 0}, {5, 0}, {6, 0}, {6, 0}, {6, 0}};
static acf_scenario_kernel_data gScenarioKernelData12_APU_FAST9_COLOR[] = {{0, 0}, {1, 0}, {3, 0}, {4, 0}, {5, 0}, {6, 0}, {6, 0}, {6, 0}};
static acf_scenario_kernel_data gScenarioKernelData13_APU_FAST9_COLOR[] = {{0, 0}, {1, 0}, {3, 0}, {4, 0}, {5, 0}, {6, 0}, {6, 0}, {6, 0}};
static acf_scenario_kernel_data gScenarioKernelData14_APU_FAST9_COLOR[] = {{0, 0}, {1, 0}, {3, 0}, {4, 0}, {5, 0}, {6, 0}, {6, 0}, {6, 0}};
static acf_scenario_kernel_data gScenarioKernelData15_APU_FAST9_COLOR[] = {{0, 0}, {1, 0}, {3, 0}, {4, 0}, {5, 0}, {6, 0}, {6, 0}, {6, 0}};
static acf_scenario_kernel_data gScenarioKernelData16_APU_FAST9_COLOR[] = {{0, 0}, {1, 0}, {3, 0}, {4, 0}, {5, 0}, {6, 0}, {6, 0}, {6, 0}};
static acf_scenario_kernel_data gScenarioKernelData17_APU_FAST9_COLOR[] = {{0, 0}, {1, 0}, {2, 0}, {3, 0}, {4, 0}, {5, 0}, {5, 0}, {5, 0}};
static acf_scenario_kernel_data gScenarioKernelData18_APU_FAST9_COLOR[] = {{0, 0}, {1, 0}, {2, 0}, {3, 0}, {4, 0}, {5, 0}, {5, 0}, {5, 0}};
static acf_scenario_kernel_data gScenarioKernelData19_APU_FAST9_COLOR[] = {{0, 0}, {1, 0}, {2, 0}, {3, 0}, {4, 0}, {5, 0}, {5, 0}, {5, 0}};
static acf_scenario_kernel_data gScenarioKernelData20_APU_FAST9_COLOR[] = {{0, 0}, {1, 0}, {2, 0}, {3, 0}, {4, 0}, {5, 0}, {5, 0}, {5, 0}};
static acf_scenario_kernel_data gScenarioKernelData21_APU_FAST9_COLOR[] = {{0, 0}, {1, 0}, {2, 0}, {3, 0}, {4, 0}, {5, 0}, {5, 0}, {5, 0}};
static acf_scenario_kernel_data gScenarioKernelData22_APU_FAST9_COLOR[] = {{0, 0}, {1, 0}, {2, 0}, {3, 0}, {4, 0}, {5, 0}, {5, 0}, {5, 0}};
static acf_scenario_kernel_data gScenarioKernelData23_APU_FAST9_COLOR[] = {{0, 0}, {1, 0}, {2, 0}, {3, 0}, {4, 0}, {5, 0}, {5, 0}, {5, 0}};
static acf_scenario_kernel_data gScenarioKernelData24_APU_FAST9_COLOR[] = {{0, 0}, {1, 0}, {2, 0}, {3, 0}, {4, 0}, {5, 0}, {5, 0}, {5, 0}};
static acf_scenario_kernel_data gScenarioKernelData25_APU_FAST9_COLOR[] = {{0, 0}, {1, 0}, {2, 0}, {3, 0}, {4, 0}, {5, 0}, {5, 0}, {5, 0}};
static acf_scenario_kernel_data gScenarioKernelData26_APU_FAST9_COLOR[] = {{0, 0}, {1, 0}, {2, 0}, {3, 0}, {4, 0}, {5, 0}, {5, 0}, {5, 0}};
static acf_scenario_kernel_data gScenarioKernelData27_APU_FAST9_COLOR[] = {{0, 0}, {1, 0}, {2, 0}, {3, 0}, {4, 0}, {5, 0}, {5, 0}, {5, 0}};
static acf_scenario_kernel_data gScenarioKernelData28_APU_FAST9_COLOR[] = {{0, 0}, {1, 0}, {2, 0}, {3, 0}, {4, 0}, {5, 0}, {5, 0}, {5, 0}};
static acf_scenario_kernel_data gScenarioKernelData29_APU_FAST9_COLOR[] = {{0, 0}, {1, 0}, {2, 0}, {3, 0}, {4, 0}, {5, 0}, {5, 0}, {5, 0}};
static acf_scenario_kernel_data gScenarioKernelData30_APU_FAST9_COLOR[] = {{0, 0}, {1, 0}, {2, 0}, {3, 0}, {4, 0}, {5, 0}, {5, 0}, {5, 0}};
static acf_scenario_kernel_data gScenarioKernelData31_APU_FAST9_COLOR[] = {{0, 0}, {1, 0}, {2, 0}, {3, 0}, {4, 0}, {5, 0}, {5, 0}, {5, 0}};
static acf_scenario_kernel_data gScenarioKernelData32_APU_FAST9_COLOR[] = {{0, 0}, {1, 0}, {2, 0}, {3, 0}, {4, 0}, {5, 0}, {5, 0}, {5, 0}};
static acf_scenario_kernel_data gScenarioKernelData33_APU_FAST9_COLOR[] = {{0, 0}, {1, 0}, {2, 0}, {3, 0}, {4, 0}, {5, 0}, {5, 0}, {5, 0}};
static acf_scenario_kernel_data gScenarioKernelData34_APU_FAST9_COLOR[] = {{0, 0}, {1, 0}, {2, 0}, {3, 0}, {4, 0}, {5, 0}, {5, 0}, {5, 0}};
static acf_scenario_kernel_data gScenarioKernelData35_APU_FAST9_COLOR[] = {{0, 0}, {1, 0}, {2, 0}, {3, 0}, {4, 0}, {5, 0}, {5, 0}, {5, 0}};
static acf_scenario_kernel_data gScenarioKernelData36_APU_FAST9_COLOR[] = {{0, 0}, {1, 0}, {2, 0}, {3, 0}, {4, 0}, {5, 0}, {5, 0}, {5, 0}};
static acf_scenario_kernel_data gScenarioKernelData37_APU_FAST9_COLOR[] = {{0, 0}, {1, 0}, {2, 0}, {3, 0}, {4, 0}, {5, 0}, {5, 0}, {5, 0}};
static acf_scenario_kernel_data gScenarioKernelData38_APU_FAST9_COLOR[] = {{0, 0}, {1, 0}, {2, 0}, {3, 0}, {4, 0}, {5, 0}, {5, 0}, {5, 0}};
static acf_scenario_kernel_data gScenarioKernelData39_APU_FAST9_COLOR[] = {{0, 0}, {1, 0}, {2, 0}, {3, 0}, {4, 0}, {5, 0}, {5, 0}, {5, 0}};
static acf_scenario_kernel_data gScenarioKernelData40_APU_FAST9_COLOR[] = {{0, 0}, {1, 0}, {2, 0}, {3, 0}, {4, 0}, {5, 0}, {5, 0}, {5, 0}};
static acf_scenario_kernel_data gScenarioKernelData41_APU_FAST9_COLOR[] = {{0, 0}, {1, 0}, {2, 0}, {3, 0}, {4, 0}, {5, 0}, {5, 0}, {5, 0}};

static acf_scenario gScenarioArray_APU_FAST9_COLOR[] = {
{4, 1, 628, 32, 9, gScenarioBufferData0_APU_FAST9_COLOR, 176, gScenarioKernelData0_APU_FAST9_COLOR, 32},
{6, 1, 820, 32, 9, gScenarioBufferData1_APU_FAST9_COLOR, 176, gScenarioKernelData1_APU_FAST9_COLOR, 32},
{8, 1, 992, 32, 9, gScenarioBufferData2_APU_FAST9_COLOR, 176, gScenarioKernelData2_APU_FAST9_COLOR, 32},
{10, 1, 1184, 32, 9, gScenarioBufferData3_APU_FAST9_COLOR, 176, gScenarioKernelData3_APU_FAST9_COLOR, 32},
{12, 1, 1356, 32, 9, gScenarioBufferData4_APU_FAST9_COLOR, 176, gScenarioKernelData4_APU_FAST9_COLOR, 32},
{16, 1, 1720, 32, 9, gScenarioBufferData5_APU_FAST9_COLOR, 176, gScenarioKernelData5_APU_FAST9_COLOR, 32},
{20, 1, 2084, 32, 9, gScenarioBufferData6_APU_FAST9_COLOR, 176, gScenarioKernelData6_APU_FAST9_COLOR, 32},
{24, 1, 2448, 32, 9, gScenarioBufferData7_APU_FAST9_COLOR, 176, gScenarioKernelData7_APU_FAST9_COLOR, 32},
{32, 1, 3176, 32, 9, gScenarioBufferData8_APU_FAST9_COLOR, 176, gScenarioKernelData8_APU_FAST9_COLOR, 32},
{4, 2, 848, 32, 6, gScenarioBufferData9_APU_FAST9_COLOR, 176, gScenarioKernelData9_APU_FAST9_COLOR, 32},
{6, 2, 1104, 32, 6, gScenarioBufferData10_APU_FAST9_COLOR, 176, gScenarioKernelData10_APU_FAST9_COLOR, 32},
{8, 2, 1360, 32, 6, gScenarioBufferData11_APU_FAST9_COLOR, 176, gScenarioKernelData11_APU_FAST9_COLOR, 32},
{10, 2, 1616, 32, 6, gScenarioBufferData12_APU_FAST9_COLOR, 176, gScenarioKernelData12_APU_FAST9_COLOR, 32},
{12, 2, 1872, 32, 6, gScenarioBufferData13_APU_FAST9_COLOR, 176, gScenarioKernelData13_APU_FAST9_COLOR, 32},
{16, 2, 2384, 32, 6, gScenarioBufferData14_APU_FAST9_COLOR, 176, gScenarioKernelData14_APU_FAST9_COLOR, 32},
{20, 2, 2896, 32, 6, gScenarioBufferData15_APU_FAST9_COLOR, 176, gScenarioKernelData15_APU_FAST9_COLOR, 32},
{24, 2, 3408, 32, 6, gScenarioBufferData16_APU_FAST9_COLOR, 176, gScenarioKernelData16_APU_FAST9_COLOR, 32},
{4, 3, 1044, 32, 5, gScenarioBufferData17_APU_FAST9_COLOR, 176, gScenarioKernelData17_APU_FAST9_COLOR, 32},
{6, 3, 1380, 32, 5, gScenarioBufferData18_APU_FAST9_COLOR, 176, gScenarioKernelData18_APU_FAST9_COLOR, 32},
{8, 3, 1704, 32, 5, gScenarioBufferData19_APU_FAST9_COLOR, 176, gScenarioKernelData19_APU_FAST9_COLOR, 32},
{10, 3, 2040, 32, 5, gScenarioBufferData20_APU_FAST9_COLOR, 176, gScenarioKernelData20_APU_FAST9_COLOR, 32},
{12, 3, 2364, 32, 5, gScenarioBufferData21_APU_FAST9_COLOR, 176, gScenarioKernelData21_APU_FAST9_COLOR, 32},
{16, 3, 3024, 32, 5, gScenarioBufferData22_APU_FAST9_COLOR, 176, gScenarioKernelData22_APU_FAST9_COLOR, 32},
{20, 3, 3684, 32, 5, gScenarioBufferData23_APU_FAST9_COLOR, 176, gScenarioKernelData23_APU_FAST9_COLOR, 32},
{4, 4, 1336, 32, 5, gScenarioBufferData24_APU_FAST9_COLOR, 176, gScenarioKernelData24_APU_FAST9_COLOR, 32},
{6, 4, 1764, 32, 5, gScenarioBufferData25_APU_FAST9_COLOR, 176, gScenarioKernelData25_APU_FAST9_COLOR, 32},
{8, 4, 2192, 32, 5, gScenarioBufferData26_APU_FAST9_COLOR, 176, gScenarioKernelData26_APU_FAST9_COLOR, 32},
{10, 4, 2620, 32, 5, gScenarioBufferData27_APU_FAST9_COLOR, 176, gScenarioKernelData27_APU_FAST9_COLOR, 32},
{12, 4, 3048, 32, 5, gScenarioBufferData28_APU_FAST9_COLOR, 176, gScenarioKernelData28_APU_FAST9_COLOR, 32},
{4, 5, 1628, 32, 5, gScenarioBufferData29_APU_FAST9_COLOR, 176, gScenarioKernelData29_APU_FAST9_COLOR, 32},
{6, 5, 2160, 32, 5, gScenarioBufferData30_APU_FAST9_COLOR, 176, gScenarioKernelData30_APU_FAST9_COLOR, 32},
{8, 5, 2680, 32, 5, gScenarioBufferData31_APU_FAST9_COLOR, 176, gScenarioKernelData31_APU_FAST9_COLOR, 32},
{10, 5, 3212, 32, 5, gScenarioBufferData32_APU_FAST9_COLOR, 176, gScenarioKernelData32_APU_FAST9_COLOR, 32},
{12, 5, 3732, 32, 5, gScenarioBufferData33_APU_FAST9_COLOR, 176, gScenarioKernelData33_APU_FAST9_COLOR, 32},
{4, 6, 1920, 32, 5, gScenarioBufferData34_APU_FAST9_COLOR, 176, gScenarioKernelData34_APU_FAST9_COLOR, 32},
{6, 6, 2544, 32, 5, gScenarioBufferData35_APU_FAST9_COLOR, 176, gScenarioKernelData35_APU_FAST9_COLOR, 32},
{8, 6, 3168, 32, 5, gScenarioBufferData36_APU_FAST9_COLOR, 176, gScenarioKernelData36_APU_FAST9_COLOR, 32},
{10, 6, 3792, 32, 5, gScenarioBufferData37_APU_FAST9_COLOR, 176, gScenarioKernelData37_APU_FAST9_COLOR, 32},
{4, 8, 2504, 32, 5, gScenarioBufferData38_APU_FAST9_COLOR, 176, gScenarioKernelData38_APU_FAST9_COLOR, 32},
{6, 8, 3324, 32, 5, gScenarioBufferData39_APU_FAST9_COLOR, 176, gScenarioKernelData39_APU_FAST9_COLOR, 32},
{4, 10, 3088, 32, 5, gScenarioBufferData40_APU_FAST9_COLOR, 176, gScenarioKernelData40_APU_FAST9_COLOR, 32},
{4, 12, 3672, 32, 5, gScenarioBufferData41_APU_FAST9_COLOR, 176, gScenarioKernelData41_APU_FAST9_COLOR, 32}
};

static acf_scenario_list gScenarioList_APU_FAST9_COLOR = {
42, //number of scenarios
gScenarioArray_APU_FAST9_COLOR};
//**************************************************************

class APU_FAST9_COLOR : public ACF_Process_APU
{

public:
   APU_FAST9_COLOR(int32_t apex_id = 0) : ACF_Process_APU(apex_id)
   {} 

   int32_t Initialize()
   {
      int32_t lRetVal = 0;

      if (!Initialized()) //initialization steps that only need to occur once
      {
         SetProcessIdentifier("APU_FAST9_COLOR");
         SetApuLoadInfo(APU_FAST9_COLOR_LOAD_SEGMENTS,
                        APU_FAST9_COLOR_LOAD_PMEM, APU_FAST9_COLOR_LOAD_PMEM_SIZE,
                        APU_FAST9_COLOR_LOAD_DMEM, APU_FAST9_COLOR_LOAD_DMEM_SIZE,
                        0, 0); //assuming _LOAD_CMEM does not exist

         lRetVal = NonParametricInitialization();
         if (lRetVal != 0) return lRetVal;

         FlagSpatialDep();

         //***NOTE: the order in which the following ports are added is meaningful; do not change!

         AddPort("INPUT", icp::DATATYPE_08U, 3, 1, 4, 1, 0, 0, 0, 0, 0, 0, 0, 9, 9, 9, 9, 0);
         AddPort("THRESHOLD", icp::DATATYPE_08U, 1, 1, 1, 1, 0, 1, 1, 0, 1, 0, 9, 0, 0, 0, 0, 0);
         AddPort("MARK_COLOR_CHANNEL", icp::DATATYPE_08U, 1, 1, 1, 1, 0, 1, 1, 0, 1, 0, 10, 0, 0, 0, 0, 0);
         AddPort("OUTPUT", icp::DATATYPE_08U, 3, 1, 4, 1, 1, 0, 0, 0, 0, 0, 8, 0, 0, 0, 0, 0);

         CfgScenarios(&gScenarioList_APU_FAST9_COLOR);

         FlagAsInitialized();
      }

      lRetVal = SelectApuConfiguration(mApuCfg, mApexId); //by default mApuCfg = ACF_APU_CFG__APEX0_DEFAULT and mApexId = 0

      return lRetVal;
   }
};

#endif //_ACF_PROCESS_APU_APU_FAST9_COLOR
