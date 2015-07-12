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
#include "driverlib/interrupt.h"
#include "utils/uartstdio.h"

#include "wifi.h"
#include "marquee.h"
#include "characters.h"

// Global interruptFlag used to know when an interrupt occurred
bool interruptHappened = false;
//*****************************************************************************
//
//! \addtogroup example_list
//! <h1>Hello World (hello)</h1>
//!
//! A very simple ``hello world'' example.  It simply displays ``Hello World!''
//! on the UART and is a starting point for more complicated applications.
//!
//! UART0, connected to the Virtual Serial Port and running at
//! 115,200, 8-N-1, is used to display messages from this application.
//
//*****************************************************************************

//*****************************************************************************
//
// The error routine that is called if the driver library encounters an error.
//
//*****************************************************************************
#ifdef DEBUG
void
__error__(char *pcFilename, uint32_t ui32Line)
{
}
#endif


const int g_rxBufLen = 140;
char g_rxBuf[140] = {0};



//*****************************************************************************
//
// Configure the UART and its pins.  This must be called before UARTprintf().
//
//*****************************************************************************
void
ConfigureUART(void)
{
    //
    // Enable the GPIO Peripheral used by the UART.
    //
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);

    //
    // Enable UART0
    //
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_UART1);

    //
    // Configure GPIO Pins for UART mode.
    //
    ROM_GPIOPinConfigure(GPIO_PA0_U0RX);
    ROM_GPIOPinConfigure(GPIO_PA1_U0TX);
    ROM_GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);

    ROM_GPIOPinConfigure(GPIO_PB0_U1RX);
    ROM_GPIOPinConfigure(GPIO_PB1_U1TX);
    ROM_GPIOPinTypeUART(GPIO_PORTB_BASE, GPIO_PIN_0 | GPIO_PIN_1);

    //
    // Use the internal 16MHz oscillator as the UART clock source.
    //
    UARTClockSourceSet(UART0_BASE, UART_CLOCK_PIOSC);
    UARTClockSourceSet(UART1_BASE, UART_CLOCK_PIOSC);

    //
    // Initialize the UART for console I/O.
    //
    UARTStdioConfig(0, 115200, 16000000);
}




void delay_s(int seconds)
{
    
    
    double secondsPerDelay = ( (1/SysCtlClockGet())*3 );
    double totalDelayCalls = seconds/secondsPerDelay;
    UARTprintf("value passed to SysCtlDelay(): %f", totalDelayCalls );
    UARTprintf("sizeof(double): %d", sizeof(double));

    SysCtlDelay( (int)totalDelayCalls );
}


void uartCallback()
{
    interruptHappened = true;
    uint32_t ui32Interrupts;

    //UARTprintf("INside uart callback!\n");

    //unsigned interruptNum = _UARTIntNumberGet(ui32Base);


    //
    // Get and clear the current interrupt source(s)
    //
    ui32Interrupts = UARTIntStatus(UART1_BASE, true);
    UARTIntClear(UART1_BASE, ui32Interrupts);

    //
    // Are we being interrupted because the TX FIFO has space available?
    if(ui32Interrupts & UART_INT_TX)
    {
        //
        // Move as many bytes as we can into the transmit FIFO.
        //
        //UARTPrimeTransmit(UART1_BASE);

        //
        // If the output buffer is empty, turn off the transmit interrupt.
        //
        //if(TX_BUFFER_EMPTY)
        //{
        //    UARTIntDisable(UART1_BASE, UART_INT_TX);
        //}
    }

    // Disable interrupt line, so we don't get called while we are in here
    IntDisable(22); 

    //
    // Are we being interrupted due to a received character?
    //
    int i = 0;
    bool enableLeds = false;
    bool done = false;
    unsigned counter = 0;
    unsigned timeoutLimit = 0x00FFFFFF;
    char tempBuf[140] = {0};
    if(ui32Interrupts & (UART_INT_RX | UART_INT_RT))
    {
        while( !done )
        {
            if( UARTCharsAvail(UART1_BASE) )
            {
                tempBuf[i] = (char)UARTCharGetNonBlocking(UART1_BASE);
                i++;
            }

            // Commands should always end with a carriage return and newline
            if( (i>=2) && (tempBuf[i-2] == '\r') && (tempBuf[i-1] == '\n') && !UARTCharsAvail(UART1_BASE) )
            {
                done = true;
                if( !strstr(tempBuf, "OK") )
                {   
                    // Clear the global buffer
                    memset(g_rxBuf, 0x00, g_rxBufLen);
                    // Copy the temp buffer into it
                    strcpy(g_rxBuf, tempBuf);
                }
            }
            /*
            if( counter > timeoutLimit )
            {
                done = true;  // oooh, timeout fail
            }

            counter++;
            */
            
            //UARTprintf("received: %c\n", (char)UARTCharGetNonBlocking(UART1_BASE));
        }
        
        /*
        UARTprintf("received: %s\n", g_rxBuf);
        for(int x = 0; x<strlen(g_rxBuf); x++)
        {
            UARTprintf("buf[%d]: %c=%d\n", x, g_rxBuf[x], g_rxBuf[x]);
        }
        */
        
    }
    // Turn interrupt back on
    IntEnable(22);
}



int main(void)
{
    //volatile uint32_t ui32Loop;

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

    configureMarqueeLedPins();
    //
    // Initialize the UART.
    //
    ConfigureUART();

    //
    // Hello!
    //
    UARTprintf("Ok, Start:\n");
    UARTprintf("ClockSpeed: %d\n", SysCtlClockGet());

    UARTStdioConfig(0, 115200, 16000000);


    if( !configureWifiChip() )
    {
        UARTprintf("No point going on! Wifi didn't come up correctly\n"); 
    }

    // register a guy to get called when the uart has data:
    UARTIntRegister(UART1_BASE, &uartCallback);

    // aaaaaaaand enable the interrupts
    UARTIntDisable(UART1_BASE, 0xFFFFFFFF);
    UARTIntEnable(UART1_BASE, UART_INT_RX | UART_INT_RT);
    
    // 22 is the interrupt line for Uart1
    IntEnable(22);

    //
    // We are finished.  Hang around doing nothing.
    //
    unsigned counter =0;
    unsigned i = 0;
    bool done = false;
    char temp = 0;
    const unsigned timeoutLimit = 0x0000FFFF;
    memset(g_rxBuf, 0x00, g_rxBufLen);
    static const short columnsPerChar = 6;

    // frameBuf holds the row information for each column
    RowNum frameBuf[120] = {0};

    // Screen can handle 20 characters at a time
    const char* frameString = "Ben Clouser is cool!";

    UARTprintf("sizeof(RowNum) == %d\n", sizeof(RowNum));
    UARTprintf("sizeof(short) == %d\n", sizeof(short));

    while(1<2)
    {
        if(interruptHappened)
        {
            interruptHappened = false;

            if( strstr(g_rxBuf, "turn lights on") )
            {
                turnAllRowsOn();
            }
            else
            {
                turnAllRowsOff();
            }


            // Build our buffer
            memset(frameBuf, 0x0000, 120);
            for(int i = 0; i < 120; ++i)
            {
                // Index characters with ascii characters -32 (cool right!?)
                frameBuf[120-1-i] = characters[ frameString[i/6]-32][i-(6*(i/6))];
            }

            UARTprintf("H as int %d - 32 = %d\n", (unsigned)'H', 'H'-32 );
        }



        putFrame(frameBuf);

        /*
        // THis is what we do for the majority of the time
        while(numFrameUpdates<maxFramUpdates)
        {
            putFrame(frameBuf);
            ++numFrameUpdates;
        }

        //shift frame buffer by one column
        */

    }
}
