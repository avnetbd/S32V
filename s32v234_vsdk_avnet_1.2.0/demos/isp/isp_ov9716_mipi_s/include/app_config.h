#ifndef __APP_CONFIG__
#define __APP_CONFIG__

#include "global_defines.h"
#include "context.h"

#define CCM_SETS 10
//***************************************************************************
// global variables
//***************************************************************************
extern float_t gKbCcmMatrix[CCM_SETS][9];
extern float_t gRgb2YuvMatrix[9];
extern float_t gCcmCscMatrix[9];
extern uint8_t  gHDRShift[4];

/************************************************************************/
/** Parses infile and setsup default parameter values.
************************************************************************/
#ifndef __STANDALONE__
int32_t IniParse(AppContext_t &arContext);
#endif

#endif