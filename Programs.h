#ifndef PROGRAMS_H
#define PROGRAMS_H

#include <stdint.h>

const int PIN_B0 = 18;
const int PIN_B1 = 19;
const int PIN_B2 = 20;
const int PIN_B3 = 21;

const char _programs[][1024] = {

#include "programs/Program1.h"
,
#include "programs/Program2.h"
,
#include "programs/Program3.h"
,
#include "programs/Program4.h"
,
#include "programs/Program5.h"
,
#include "programs/Program6.h"
,
#include "programs/Program7.h"
,
#include "programs/Program8.h"
,
#include "programs/Program9.h"
,
#include "programs/Program10.h"
,
#include "programs/Program11.h"
,
#include "programs/Program12.h"
,
#include "programs/Program13.h"
,
#include "programs/Program14.h"
,
#include "programs/Program15.h"
,
#include "programs/Program16.h"

};

void InitProgramNumberPins();

uint8_t GetProgramIndexFromPins();

void RunProgramDataFromIndex(uint8_t progNum);

#endif // PROGRAMS_H
