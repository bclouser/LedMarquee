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


/*
#include "driverlib/uart.h"
#include "driverlib/interrupt.h"
#include "utils/uartstdio.h"

#include "wifi.h"
*/


//
// Configure GPIO Pins for UART mode.
//
void ConfigureUART(void)
{
    //
    // Enable the GPIO Peripheral used by the UART.
    //
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);

    //
    // Enable UART0
    //
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);

    //
    // Configure GPIO Pins for UART mode.
    //
    ROM_GPIOPinConfigure(GPIO_PA0_U0RX);
    ROM_GPIOPinConfigure(GPIO_PA1_U0TX);
    ROM_GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);

    //
    // Use the internal 16MHz oscillator as the UART clock source.
    //
    UARTClockSourceSet(UART0_BASE, UART_CLOCK_PIOSC);

    //
    // Initialize the UART for console I/O.
    //
    UARTStdioConfig(0, 115200, 16000000);
}

void configurePins()
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


void delay_s(int seconds)
{
    double secondsPerDelay = ( (1/SysCtlClockGet())*3 );
    double totalDelayCalls = seconds/secondsPerDelay;
    //UARTprintf("value passed to SysCtlDelay(): %f", totalDelayCalls );
    //UARTprintf("sizeof(double): %d", sizeof(double));

    SysCtlDelay( (int)totalDelayCalls );
}

typedef enum
{
    e_dmux1 = 0,
    e_dmux2 = 1,
    e_dmux3,
    e_dmux4,
    e_dmux5,
    e_dmux6,
    e_dmux7,
    e_dmux8
}DmuxType;

typedef enum {
    e_row1 = 1,
    e_row2 = 2,
    e_row3 = 3,
    e_row4 = 4,
    e_row5 = 5,
    e_row6 = 6,
    e_row7 = 7,
    e_row8 = 8,
    e_row9 = 9
}RowNum;

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

void setColumn(short columnNum)
{
    // It seems we need to turn off existing LEDS to start.
    setMuxAddr(0x00);
    setActiveDmux( columnNum/15 );
    setMuxAddr( (columnNum%15)+1 );
    //UARTprintf("\tsetMuxAddr( (%dmod15) + 1) = 0x%02X \n",columnNum, (columnNum%15) + 1); 
}

// Good to remember that these guys are active low
void setActiveRow(RowNum rowNum)
{
    // Start by setting(clearing, active low) all of them
    GPIO_PORTD_DATA_R |= 0x0F;
    GPIO_PORTE_DATA_R |= 0x3E;

    if(rowNum < 6) // PORTE
    {
        GPIO_PORTE_DATA_R &= ~(1<<rowNum); 
    }
    else  // PORTD
    {
        GPIO_PORTD_DATA_R &= ~( 1<<(rowNum-6) );
    }
}

void loopMux(unsigned dmuxIndex)
{
    volatile uint32_t i;
    for(i = 0; i < 15; ++i)
    {
        //UARTprintf("setColumn( (%d*15) + i) = %d \n",dmuxIndex, (dmuxIndex*15) + i);
        setColumn( (dmuxIndex*15) + i );
        SysCtlDelay(SysCtlClockGet()/190000);
    }
}

void loopColumns()
{
    unsigned ui32Loop = 0;
    unsigned dmuxIndex = 0;
    // columns
    GPIO_PORTC_DATA_R |= 0x10; // column1

    // Rows
    GPIO_PORTD_DATA_R &= ~0x0F;  // all rows on
    GPIO_PORTE_DATA_R &= ~0x3E;  // all rows on

    while(1<2)
    {
        //SysCtlDelay( SysCtlClockGet() );
        for(ui32Loop = 0; ui32Loop < 120; ++ui32Loop)
        {
            // Check if we reached a dmux boundary
            if((ui32Loop%15) == 0)
            {
                //UARTprintf("ui32Loop: %u... setActiveDmux(%u)\n",ui32Loop, dmuxIndex);
                setActiveDmux(dmuxIndex); 
                if(dmuxIndex < 8)
                {
                    ++dmuxIndex;
                }
                else
                {
                    dmuxIndex = 0;
                }
            }
            setMuxAddr(ui32Loop%15+1);
            // Delay for a bit.
            //
            SysCtlDelay( SysCtlClockGet()/500 );
        }
    }
}

void loopGrid()
{
    unsigned ui32Loop = 0;
    unsigned dmuxIndex = 0;
    // columns
    GPIO_PORTC_DATA_R |= 0x10; // column1

    // Rows
    GPIO_PORTD_DATA_R &= ~0x0F;  // all rows o
    GPIO_PORTE_DATA_R &= ~0x3E;  // all rows on

    while(1<2)
    {
        //SysCtlDelay( SysCtlClockGet() );
        for(ui32Loop = 0; ui32Loop < 120; ++ui32Loop)
        {
            //UARTprintf("ui32Loop: %u... setActiveDmux(%u), setMuxAddr(%u); \n",ui32Loop, dmuxIndex, ui32Loop%15+1);
            // Check if we reached a dmux boundary
            if((ui32Loop%15) == 0)
            {
                setActiveDmux(dmuxIndex); 
                if(dmuxIndex < 8)
                {
                    ++dmuxIndex;
                }
                else
                {
                    dmuxIndex = 0;
                }
            }
            
            setMuxAddr(ui32Loop%15+1);

            // Loop through each row
            for(unsigned i = 1; i<10; i++)
            {
                setActiveRow(i);
                // Delay for a bit.
                //
                SysCtlDelay( SysCtlClockGet()/5000 );
            }

            
        }
    }
}



int main(void)
{
    //
    // Enable lazy stacking for interrupt handlers.  This allows floating-point
    // instructions to be used within interrupt handlers, but at the expense of
    // extra stack usage.
    //
    ROM_FPULazyStackingEnable();

    //
    // Set the clocking to run directly from the crystal.
    //

    //ROM_SysCtlClockSet(SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ |
    //                   SYSCTL_OSC_MAIN);

    // Set the clock for 80 mhz... currently only getting 66.666666mhz
    SysCtlClockSet(SYSCTL_SYSDIV_2_5| SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | 
                                        SYSCTL_XTAL_16MHZ);  //80MHz

    configurePins();

    //
    // Initialize the UART.
    //
    ConfigureUART();

    //
    // Hello!
    //
    UARTprintf("Ok, Start:\n");
    UARTprintf("ClockSpeed: %d\n", SysCtlClockGet());

    // delay before the real stuff begins
    SysCtlDelay(SysCtlClockGet()/10000000);

    //loopColumns();
    loopGrid();

}
