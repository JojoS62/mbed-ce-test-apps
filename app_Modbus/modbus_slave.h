#pragma once

#include "mb.h"

/* ----------------------- Defines ------------------------------------------*/
#define REG_INPUT_START     99           //
#define REG_INPUT_NREGS     4               // number of regs
#define SLAVE_ID            1               // our slave ID  
#define DUMMY_PORT          0               // currently unused port in modbus/portserial.cpp

extern USHORT   usRegInputStart;
extern USHORT   usRegInputBuf[REG_INPUT_NREGS];
