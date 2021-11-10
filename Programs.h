#ifndef PROGRAMS_H
#define PROGRAMS_H

#include <stdint.h>

const int PIN_B0 = 18;
const int PIN_B1 = 19;
const int PIN_B2 = 20;
const int PIN_B3 = 21;

const char _programs[][1024] = {

#include "Program1.h"
,
#include "Program2.h"
,
#include "Program3.h"
,
#include "Program4.h"
,
#include "Program5.h"
,
#include "Program6.h"
,
#include "Program7.h"
,
#include "Program8.h"
,
#include "Program9.h"
,
#include "Program10.h"
,
#include "Program11.h"
,
#include "Program12.h"
,
#include "Program13.h"
,
#include "Program14.h"
,
#include "Program15.h"
,
#include "Program16.h"

};

void InitProgramNumberPins();

uint8_t GetProgramIndexFromPins();

void RunProgramDataFromIndex(uint8_t progNum);

#endif // PROGRAMS_H
