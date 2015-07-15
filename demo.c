/*
 * FreeModbus Libary: BARE Demo Application
 * Copyright (C) 2006 Christian Walter <wolti@sil.at>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 * File: $Id: demo.c,v 1.1 2006/08/22 21:35:13 wolti Exp $
 */

/* ----------------------- Modbus includes ----------------------------------*/
#include "mb.h"
#include "mbport.h"

/* ----------------------- Defines ------------------------------------------*/
#define REG_INPUT_START 1000
#define REG_INPUT_NREGS 4
#define REG_HOLDING_START 2000
#define REG_HOLDING_NREGS 130

/* ----------------------- Static variables ---------------------------------*/
static USHORT   usRegInputStart = REG_INPUT_START;
static USHORT   usRegInputBuf[REG_INPUT_NREGS];
static unsigned short usRegHoldingStart = REG_HOLDING_START;
static unsigned short usRegHoldingBuf[REG_HOLDING_NREGS];

static void Init_CPU(void);

/* ----------------------- Start implementation -----------------------------*/
int main( void )
{
    eMBErrorCode    eStatus;

    Init_CPU();
  
    eStatus = eMBInit( MB_RTU, 0x0A, 0, 38400, MB_PAR_NONE );

    /* Enable the Modbus Protocol Stack. */
    eStatus = eMBEnable(  );

    //enableInterrupts();
  
    for( ;; )
    {
        ( void )eMBPoll(  );

        /* Here we simply count the number of poll cycles. */
        usRegInputBuf[0]++;
    }    
}

eMBErrorCode
eMBRegInputCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNRegs )
{
    eMBErrorCode    eStatus = MB_ENOERR;
    int             iRegIndex;

    if( ( usAddress >= REG_INPUT_START ) // 1001 >= 1000
        && ( usAddress + usNRegs <= REG_INPUT_START + REG_INPUT_NREGS ) ) // (1001 + 5) <= (1000 + 4)
    {
        iRegIndex = ( int )( usAddress - usRegInputStart );
        while( usNRegs > 0 )
        {
            *pucRegBuffer++ =
                ( unsigned char )( usRegInputBuf[iRegIndex] >> 8 );
            *pucRegBuffer++ =
                ( unsigned char )( usRegInputBuf[iRegIndex] & 0xFF );
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

eMBErrorCode
eMBRegHoldingCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNRegs,
                 eMBRegisterMode eMode )
{
  eMBErrorCode eStatus = MB_ENOERR; 
  int iRegIndex; 
  //u16 * PRT = (u16 *) pucRegBuffer; 
  if ((usAddress >= REG_HOLDING_START) && (usAddress + usNRegs <= REG_HOLDING_START + REG_HOLDING_NREGS)) 
  { 
    iRegIndex = (int) (usAddress - usRegHoldingStart); 
    switch (eMode) 
    { 
      case MB_REG_READ: 
        while (usNRegs > 0) 
        { 
          //// *PRT++ = __REV16 (usRegHoldingBuf [iRegIndex++]); // data transfer sequence REV16.W 

          *pucRegBuffer ++= (unsigned char) (usRegHoldingBuf[iRegIndex] >> 8); 
          *pucRegBuffer ++= (unsigned char) (usRegHoldingBuf[iRegIndex] & 0xFF); 
          iRegIndex++; 
          usNRegs--; 
        } 
      break; 

      case MB_REG_WRITE: 
        while (usNRegs> 0) 
        { 
          //// UsRegHoldingBuf[iRegIndex++] = __REV16(*PRT++); // data transfer sequence REV16.W 
          usRegHoldingBuf[iRegIndex] =  *pucRegBuffer++ << 8; 
          usRegHoldingBuf[iRegIndex] |= *pucRegBuffer++; 
          iRegIndex++; 
          usNRegs--; 
        } 
      break; 
    } 
  } 
  else 
  { 
    eStatus = MB_ENOREG; 
  } 
  return eStatus; 
}


eMBErrorCode
eMBRegCoilsCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNCoils,
               eMBRegisterMode eMode )
{
  (void) pucRegBuffer; 
  (void) usAddress; 
  (void) usNCoils; 
  (void) eMode; 
  return MB_ENOREG;
}

eMBErrorCode
eMBRegDiscreteCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNDiscrete )
{
  (void)pucRegBuffer;
  (void)usAddress;
  (void)usNDiscrete;

  return MB_ENOREG;
}

/**
  * @brief Инициализация микроконтроллера
  * @par
  * Параметры отсутствуют
  * @retval
  * Возвращаемое значение отсутствует
*/
static void Init_CPU(void)
{
#if 0
  // Настроить тактовую частоту микроконтроллера
  CLK_DeInit();
  
  // Внутренний источник, 16 МГц  
  CLK_HSICmd(ENABLE);
  
  // предделитель может варьироваться от 1 до 8.
  
  CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1);
#else
	/*
  u8 cnt = 0;
  // Настроить тактовую частоту микроконтроллера
  CLK_DeInit();
  
  // Внешний источник, 11,0592 МГц  
  CLK_SYSCLKConfig(CLK_PRESCALER_CPUDIV1);
  CLK_ClockSwitchConfig(CLK_SWITCHMODE_AUTO, CLK_SOURCE_HSE, DISABLE, CLK_CURRENTCLOCKSTATE_DISABLE);  

  // Ждать стабилизации частоты
  while (CLK_GetSYSCLKSource() != CLK_SOURCE_HSE)
  {
    if ( --cnt == 0 )
      return;
  }
  
  // Конфигурация предделителя CAN
  CLK_CANConfig( CLK_CANDIVIDER_1 );
	*/
#endif
}
