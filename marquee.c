//*****************************************************************************
//
// hello.c - Simple hello world example.
//
// Copyright (c) 2012-2014 Texas Instruments Incorporated.  All rights reserved.
// Software License Agreement
// 
// Texas Instruments (TI) is supplying this software for use solely and
// exclusively on TI's microcontroller products. The software is owned by
// TI and/or its suppliers, and is protected under applicable copyright
// laws. You may not combine this software with "viral" open-source
// software in order to form a larger program.
// 
// THIS SOFTWARE IS PROVIDED "AS IS" AND WITH ALL FAULTS.
// NO WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT
// NOT LIMITED TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. TI SHALL NOT, UNDER ANY
// CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL, OR CONSEQUENTIAL
// DAMAGES, FOR ANY REASON WHATSOEVER.
// 
// This is part of revision 2.1.0.12573 of the EK-TM4C123GXL Firmware Package.
//
//*****************************************************************************

#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/debug.h"
#include "driverlib/fpu.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/rom.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"
#include "inc/tm4c123gh6pm.h"
#include "utils/uartstdio.h"
#include "marquee.h"

void configureMarqueeLedPins()
{
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);

    GPIO_PORTF_DIR_R = 0xFF; // output
    GPIO_PORTF_DEN_R = 0xFF; // data enable?

    GPIO_PORTD_DIR_R = 0xFF; // output
    GPIO_PORTD_DEN_R = 0xFF; // data enable?

    GPIO_PORTE_DIR_R = 0xFF; // output
    GPIO_PORTE_DEN_R = 0xFF; // data enable?

    GPIO_PORTC_DIR_R = 0xFF; // output
    GPIO_PORTC_DEN_R = 0xFF; // data enable?

    // The wifi uart is on port B... Tread lightly
    //GPIO_PORTB_DIR_R = 0xFF; // output
    //GPIO_PORTB_DEN_R = 0xFF; // data enable?

    GPIO_PORTA_DIR_R |= 0xFC; // output
    GPIO_PORTA_DEN_R |= 0xFC; // data enable?

    //ROM_GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_2);
}

void setMuxAddr(unsigned addr)
{
    // read
    unsigned temp = GPIO_PORTC_DATA_R;

    // modify
    temp &= ~0xF0; // clear bits we care about
    // shift addr up by four bits b/c portc
    temp |= ((addr & 0x0F) << 4);

    // Write
    GPIO_PORTC_DATA_R = temp;
}

void disableAllDmux()
{
    // THe dmux chip is active low
    // ensure that PORT A select pins are unselected
    GPIO_PORTA_DATA_R |= 0xFC;
    // ensure that PORT F select pins are unselected
    GPIO_PORTF_DATA_R |= 0x06;
}

void setActiveDmux(DmuxType dmux)
{
    unsigned temp = 0;
    // select bits 0 and 1 are physical port F bits 1 and 2
    if(dmux < 2)
    {
        //GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_5, GPIO_PIN_5);

        // THe dmux chip is active low
        // ensure that PORT A select pins are unselected
        GPIO_PORTA_DATA_R |= 0xFC;
        // read
        temp = GPIO_PORTF_DATA_R;

        // modify. (dmux +1) b/c sel0 starts on F1 not F0
        temp &= ~0x06; // clear the bits we will modify
        temp |= ( ~(1<<(dmux+1)) ) & 0x06;

        // write 
        //UARTprintf("Writing 0x%02X to PORTF\n", temp);
        GPIO_PORTF_DATA_R = temp; 
    }
    else // select bits 2-7 are physical port A bits 2-7
    {
        // ensure that PORT F select pins are unselected
        GPIO_PORTF_DATA_R |= 0x06;
        // read
        temp = GPIO_PORTA_DATA_R;

        // modify
        temp &= ~0xFC; // clear the bits we will modify
        temp |= (~(1<<dmux)) & 0xFC;

        // write

        //UARTprintf("Writing 0x%02X to PORTA\n", temp);
        GPIO_PORTA_DATA_R = temp;
    }
}

void setActiveColumn(short columnNum)
{
    // It seems we need to turn off existing LEDS to start.
    setMuxAddr(0x00);
    setActiveDmux( columnNum/15 );
    setMuxAddr( (columnNum%15)+1 );
    //UARTprintf("\tsetMuxAddr( (%dmod15) + 1) = 0x%02X \n",columnNum, (columnNum%15) + 1); 
}

// Good to remember that these guys are active low
void setActiveRows(RowNum rows)
{
    // bits: XXXX_XXX1_1111_1111

    // Start by setting(clearing, active low) all of them
    GPIO_PORTD_DATA_R |= 0x0F;
    GPIO_PORTE_DATA_R |= 0x3E;

    // The rows number passed in is two bytes, representational of rows 1-9 as bits 0-8
    // bits 1-5 on portE represent rows 1-5
    GPIO_PORTE_DATA_R &= ~( (rows << 1) & 0x003E );

    // bits 0-3 on portD represent rows 6-9
    GPIO_PORTD_DATA_R &= ~( (rows>>5) & 0x00F );
}

void loopMux(unsigned dmuxIndex)
{
    volatile uint32_t i;
    for(i = 0; i < 15; ++i)
    {
        //UARTprintf("setActiveColumn( (%d*15) + i) = %d \n",dmuxIndex, (dmuxIndex*15) + i);
        setActiveColumn( (dmuxIndex*15) + i );
        SysCtlDelay(SysCtlClockGet()/190000);
    }
}

void turnAllRowsOn()
{
    // Rows
    GPIO_PORTD_DATA_R &= ~0x0F;  // all rows on
    GPIO_PORTE_DATA_R &= ~0x3E;  // all rows on
}

void turnAllRowsOff()
{
    // Rows
    GPIO_PORTD_DATA_R |= 0x0F;  // all rows off
    GPIO_PORTE_DATA_R |= 0x3E;  // all rows off
}

void loopColumns()
{
    static unsigned ui32Loop = 0;

    //SysCtlDelay( SysCtlClockGet() );
    for(ui32Loop = 0; ui32Loop < 120; ++ui32Loop) {
        setActiveColumn(ui32Loop);
        SysCtlDelay( 100 );
    }
}

// Stole this from stack overflow.
int numberOfSetBits(int i)
{
     i = i - ((i >> 1) & 0x55555555);
     i = (i & 0x33333333) + ((i >> 2) & 0x33333333);
     return (((i + (i >> 4)) & 0x0F0F0F0F) * 0x01010101) >> 24;
}

// buffer better be 120 Rows long!!!
void doFrame(RowNum* buf)
{
    static unsigned ui32Loop = 0;

    for(int columnIndex = 0;  columnIndex< 120; ++columnIndex) {  

        setActiveColumn(columnIndex);
        SysCtlDelay(30);  //columns need some ticks to become active
        setActiveRows( buf[columnIndex] );
        
        // Brightness issues. Sigh. Tuning the delays for a uniform brightness
        if( numberOfSetBits( buf[columnIndex] ) < 5)
        {
           SysCtlDelay( 5 );
        }
        else{
           SysCtlDelay( 15 );
        }

        turnAllRowsOff();
        SysCtlDelay(30);
    }
    turnAllRowsOff();
}

void loopGrid()
{
    static unsigned ui32Loop = 0;

    while(1<2)
    {
        //SysCtlDelay( SysCtlClockGet() );
        for(ui32Loop = 0; ui32Loop < 120; ++ui32Loop)
        {
            //UARTprintf("ui32Loop: %u... setActiveDmux(%u), setMuxAddr(%u); \n",ui32Loop, dmuxIndex, ui32Loop%15+1);
            setActiveColumn(ui32Loop);

            // Loop through each row
            for(unsigned i = 0; i<9; i++)
            {
                setActiveRows(1<<i);
                // Delay for a bit.
                SysCtlDelay( 100 );
            }  
        }
    }
}