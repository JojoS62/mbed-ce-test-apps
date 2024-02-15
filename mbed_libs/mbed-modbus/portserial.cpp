/*
 * FreeModbus Libary: BARE Port
 * Copyright (C) 2006 Christian Walter <wolti@sil.at>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 * File: $Id: portserial.c,v 1.1 2006/08/22 21:35:13 wolti Exp $
 */

/* ----------------------- System includes ----------------------------------*/
#include "mbed.h"                   // Cam

/* ----------------------- Platform includes --------------------------------*/
#include "port.h"

/* ----------------------- Modbus includes ----------------------------------*/
#include "mb.h"
#include "mbport.h"


/* ----------------------- static functions ---------------------------------*/
static void prvvUARTTxReadyISR( void );
static void prvvUARTRxISR( void );

/* ----------------------- System Variables ---------------------------------*/
// UnbufferedSerial pc(USART3_TX, USART3_RX);    // Cam - mbed USB serial port
UnbufferedSerial pc(PD_5, PD_6);
DigitalOut	rs485TxEn(PD_7, 0);
DigitalOut  testPin(PA_6);
DigitalOut  testPin2(PA_4);

/* ----------------------- Start implementation -----------------------------*/
void
vMBPortSerialEnable( BOOL xRxEnable, BOOL xTxEnable )
{
    /* If xRXEnable enable serial receive interrupts. If xTxENable enable
     * transmitter empty interrupts.
     */
    // pc.enable_input( xRxEnable );
    // pc.enable_output( xTxEnable );

    // pc.format(8, SerialBase::Parity::Even);  // ugly Hack to fix enable resetting the parity setting

    if (xRxEnable) {
        wait_us(100);
        rs485TxEn = 0;
        pc.attach( &prvvUARTRxISR, SerialBase::RxIrq ); 
    } else {
        pc.attach( nullptr, SerialBase::RxIrq ); 
    }

    if (xTxEnable) {
        rs485TxEn = 1;
        pc.attach( &prvvUARTTxReadyISR, SerialBase::TxIrq );
    } else {
        pc.attach( nullptr, SerialBase::TxIrq );
    }


    // rs485TxEn = xTxEnable ? 1 : 0;
}

BOOL
xMBPortSerialInit( UCHAR ucPORT, ULONG ulBaudRate, UCHAR ucDataBits, eMBParity eParity )
{
    // pc.enable_output( false );
    // pc.enable_input( false );

    pc.set_blocking( false );
    pc.baud( ulBaudRate );

    // convert MBParity to Mbed Parity
    SerialBase::Parity p = SerialBase::Parity::None;
    if (eParity == eMBParity::MB_PAR_EVEN) {
        p = SerialBase::Parity::Even;
        printf("parity: EVEN\n");
    }  else if (eParity == eMBParity::MB_PAR_ODD) {
        p = SerialBase::Parity::Odd;
        printf("parity: ODD\n");
    } else if (eParity == eMBParity::MB_PAR_NONE) {
        printf("parity: NONE\n");
    } else  {
        printf("parity: UNKNOWN\n");
        return FALSE;
    }
    
    pc.format(ucDataBits, p);

    // pc.attach( &prvvUARTTxReadyISR, SerialBase::TxIrq );
    // pc.attach( &prvvUARTRxISR, SerialBase::RxIrq ); 
    
    return TRUE;
}

BOOL
xMBPortSerialPutByte( CHAR ucByte )
{
    /* Put a byte in the UARTs transmit buffer. This function is called
     * by the protocol stack if pxMBFrameCBTransmitterEmpty( ) has been
     * called. */
    pc.write( &ucByte, 1 );
    return TRUE;
}

BOOL
xMBPortSerialGetByte( CHAR * pucByte )
{
    /* Return the byte in the UARTs receive buffer. This function is called
     * by the protocol stack after pxMBFrameCBByteReceived( ) has been called.
     */
    pc.read( pucByte, 1 );
    return TRUE;
}

/* Create an interrupt handler for the transmit buffer empty interrupt
 * (or an equivalent) for your target processor. This function should then
 * call pxMBFrameCBTransmitterEmpty( ) which tells the protocol stack that
 * a new character can be sent. The protocol stack will then call
 * xMBPortSerialPutByte( ) to send the character.
 */
static void prvvUARTTxReadyISR( void )
{
    testPin = 1;
    if( pc.writable() )
        pxMBFrameCBTransmitterEmpty(  );
    testPin = 0;
}

/* Create an interrupt handler for the receive interrupt for your target
 * processor. This function should then call pxMBFrameCBByteReceived( ). The
 * protocol stack will then call xMBPortSerialGetByte( ) to retrieve the
 * character.
 */
static void prvvUARTRxISR( void )
{
    testPin2 = 1;
    if( pc.readable() ) {
        pxMBFrameCBByteReceived(  );
    }
    testPin2 = 0;
}


