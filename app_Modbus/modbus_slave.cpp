#include "modbus_slave.h"
#include "mbport.h"
#include <stdio.h>
 
/* ----------------------- Static variables ---------------------------------*/
USHORT   usRegInputStart = REG_INPUT_START;
USHORT   usRegInputBuf[REG_INPUT_NREGS];
 
/* ----------------------- Start implementation -----------------------------*/
eMBErrorCode eMBRegInputCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNRegs )
{
    eMBErrorCode    eStatus = MB_ENOERR;
    int             iRegIndex;
 
    --usAddress;    // real register index
    printf("eMBRegInputCB: 0x%04x %d\n", usAddress, usNRegs);

    if( ( usAddress >= REG_INPUT_START )
        && ( usAddress + usNRegs <= REG_INPUT_START + REG_INPUT_NREGS ) )
    {
        iRegIndex = ( int )( usAddress - usRegInputStart );
        while( usNRegs > 0 )
        {
            *pucRegBuffer++ = (unsigned char)(usRegInputBuf[iRegIndex] >> 8);
            *pucRegBuffer++ = (unsigned char)(usRegInputBuf[iRegIndex] & 0xFF);
            iRegIndex++;
            usNRegs--;
        }
    }
    else
    {
        eStatus = MB_ENOREG;
    }
 
    return eStatus;
}
 
eMBErrorCode eMBRegHoldingCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNRegs, eMBRegisterMode eMode )
{
    eMBErrorCode    eStatus = MB_ENOERR;
    int             iRegIndex;
 
    --usAddress;    // real register index
    printf("eMBRegHoldingCB: 0x%04x %d\n", usAddress, usNRegs);

    if (eMode == MB_REG_READ)
    {
        if( ( usAddress >= REG_INPUT_START )
            && ( usAddress + usNRegs <= REG_INPUT_START + REG_INPUT_NREGS ) )
        {
            iRegIndex = ( int )( usAddress - usRegInputStart );
            while( usNRegs > 0 )
            {
                printf("  read: 0x%04x\n", usRegInputBuf[iRegIndex]);

                *pucRegBuffer++ = (unsigned char)( usRegInputBuf[iRegIndex] >> 8);
                *pucRegBuffer++ = (unsigned char)( usRegInputBuf[iRegIndex] & 0xFF);
                iRegIndex++;
                usNRegs--;
            }
        } else {
            printf("  read address out of range\n");
        }
    }
 
    if (eMode == MB_REG_WRITE)
    {
        if( ( usAddress >= REG_INPUT_START )
            && ( usAddress + usNRegs <= REG_INPUT_START + REG_INPUT_NREGS ) )
        {
            iRegIndex = ( int )( usAddress - usRegInputStart );
            while( usNRegs > 0 )
            {
                uint16_t tmp =  ((unsigned int) *pucRegBuffer << 8) | ((unsigned int) *(pucRegBuffer+1));

                printf("  write: 0x%04x\n", tmp);

                usRegInputBuf[iRegIndex] = tmp;
                pucRegBuffer+=2;
                iRegIndex++;
                usNRegs--;
            }
        } else {
            printf("  write address out of range\n");
        }
    }
 
    return eStatus;
}
 
 
eMBErrorCode eMBRegCoilsCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNCoils,
               eMBRegisterMode eMode )
{
    --usAddress;    // real register index
    printf("eMBRegCoilsCB: 0x%04x %d\n", usAddress, usNCoils);

    return MB_ENOREG;
}
 
eMBErrorCode eMBRegDiscreteCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNDiscrete )
{
    --usAddress;    // real register index
    printf("eMBRegDiscreteCB: 0x%04x %d\n", usAddress, usNDiscrete);

    return MB_ENOREG;
}
 