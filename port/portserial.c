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

#include "port.h"

/* ----------------------- Modbus includes ----------------------------------*/
#include "mb.h"
#include "mbport.h"

/* ----------------------- static functions ---------------------------------*/


/* ----------------------- Start implementation -----------------------------*/
void
vMBPortSerialEnable( BOOL xRxEnable, BOOL xTxEnable )
{ 
	/*
  if (TRUE == xRxEnable) 
  { 
    UART1_ITConfig (UART1_IT_RXNE, ENABLE); 
  } 
  else 
  { 
    UART1_ITConfig (UART1_IT_RXNE, DISABLE); 
  } 

  if (TRUE == xTxEnable) 
  { 
    UART1_ITConfig (UART1_IT_TXE, ENABLE); 
  } 
  else 
  { 
    UART1_ITConfig (UART1_IT_TXE, DISABLE); 
  } 
	*/
}

BOOL
xMBPortSerialInit( UCHAR ucPORT, ULONG ulBaudRate, UCHAR ucDataBits, eMBParity eParity )
{
	/*
  UART1_WordLength_TypeDef databit; 
  UART1_Parity_TypeDef parity; 
  UART1_DeInit (); 
  ucPORT = ucPORT; // подавить предупреждение
  if (8 == ucDataBits) 
  { 
    databit = UART1_WORDLENGTH_8D; 
  } 
  else 
  { 
    databit = UART1_WORDLENGTH_9D; 
  } 
  if (MB_PAR_NONE == eParity) 
  { 
    parity = UART1_PARITY_NO; 
  } 
  else if (MB_PAR_ODD == eParity) 
  { 
    parity = UART1_PARITY_ODD; 
  } 
  else 
  { 
    parity = UART1_PARITY_EVEN; 
  } 
  UART1_Init ((uint32_t) ulBaudRate, databit, UART1_STOPBITS_1, parity, 
  UART1_SYNCMODE_CLOCK_DISABLE, UART1_MODE_TXRX_ENABLE); 
  UART1_ITConfig (UART1_IT_TXE, ENABLE);
	*/
  return TRUE;
}

BOOL
xMBPortSerialPutByte( CHAR ucByte )
{
	/*
  UART1_SendData8(ucByte); 
	*/
  return TRUE;
}

BOOL
xMBPortSerialGetByte( CHAR * pucByte )
{
	/*
  *pucByte = UART1_ReceiveData8(); 
	*/
  return TRUE;
}

/* Create an interrupt handler for the transmit buffer empty interrupt
 * (or an equivalent) for your target processor. This function should then
 * call pxMBFrameCBTransmitterEmpty( ) which tells the protocol stack that
 * a new character can be sent. The protocol stack will then call 
 * xMBPortSerialPutByte( ) to send the character.
 */
void UART1_TX_IRQHandler(void)
{
    pxMBFrameCBTransmitterEmpty(  );
}

/* Create an interrupt handler for the receive interrupt for your target
 * processor. This function should then call pxMBFrameCBByteReceived( ). The
 * protocol stack will then call xMBPortSerialGetByte( ) to retrieve the
 * character.
 */
void UART1_RX_IRQHandler(void)
{
    pxMBFrameCBByteReceived(  ); // xMBRTUReceiveFSM
}

void vMBPortClose( void )
{
}

void enableInterrupts( void )
{
}

void disableInterrupts( void )
{
}
