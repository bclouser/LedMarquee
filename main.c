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
#include <stdio.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_ints.h"
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
volatile bool interruptHappened = false;
volatile bool g_rxSuccess = false;

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

// g_rxBuf holds the raw data read from the UART
#define RX_MAX_BUF_LEN 512
int g_rxBufLen = 0;
char g_rxBuf[RX_MAX_BUF_LEN] = {0};

// g_currentDisplayString holds the string to display on the matrix
int g_currentDisplayStrLen = 0;
char g_currentDisplayString[RX_MAX_BUF_LEN+20] = {0};



//*****************************************************************************
//
// Configure the UART and its pins.  This must be called before UARTprintf().
//
//*****************************************************************************
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

bool getUartDataBlocking()
{
    int i = 0;
    unsigned counter = 0;
    unsigned timeoutLimit = 0xFFFF;
    bool timeout = false;

    // clear out the receive buffer
    memset(g_rxBuf, 0x00, RX_MAX_BUF_LEN);
    while( 1<2 )
    {
        while(UARTCharsAvail(UART1_BASE))
        {
            if(i >= RX_MAX_BUF_LEN)
            {
                return false;
            }
            g_rxBuf[i] = (char)UARTCharGetNonBlocking(UART1_BASE);
            ++i;
            g_rxBufLen = i;
        }

        if( g_rxBufLen >= RX_MAX_BUF_LEN )
        {
            return false;
        }
        else if( counter > timeoutLimit )
        {
            return true;
        }

        counter++;
        //UARTprintf("received: %c\n", (char)UARTCharGetNonBlocking(UART1_BASE));
    }
    return false;
}
void uartCallback()
{
    uint32_t ui32Interrupts;

    //unsigned interruptNum = _UARTIntNumberGet(ui32Base);

    //
    // Get and clear the current interrupt source(s)
    //
    ui32Interrupts = UARTIntStatus(UART1_BASE, true);
    UARTIntClear(UART1_BASE, ui32Interrupts);
    // Disable interrupt line, so we don't get called while we are in here
    IntDisable(INT_UART1_TM4C123); 

    //
    // Are we being interrupted because the TX FIFO has space available?
    if(ui32Interrupts & UART_INT_TX)
    {

    }

    //
    // Are we being interrupted due to a received character?
    //
    else if(ui32Interrupts & (UART_INT_RX | UART_INT_RT))
    {
        interruptHappened = true;
        g_rxSuccess = getUartDataBlocking();
    }
    // Turn interrupt back on
    IntEnable(INT_UART1_TM4C123);
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


    if( !configureWifiChip() )
    {
        UARTprintf("No point going on! Wifi didn't come up correctly\n"); 
    }

    // register a guy to get called when the uart has data:
    UARTIntRegister(UART1_BASE, &uartCallback);


    //
    // We are finished.  Hang around doing nothing.
    //
    unsigned counter =0;
    unsigned i = 0;
    bool done = false;
    char temp = 0;
    const unsigned timeoutLimit = 0x0000FFFF;
    memset(g_rxBuf, 0x00, RX_MAX_BUF_LEN);
    static const short charWidth = 6;
    unsigned numFrameUpdates = 0;
    unsigned updatesPerScroll = 4;
    // keep track of the overall columnOffset
    unsigned columnOffset = 0;

    //unsigned numCharsInBuf = 117 + 20;

    // frameBuf holds the row information for each column
    RowNum frameBuf[120] = {0};
    
    // Initially we paint the screen with our ip address
    char ipAddr[26] = {0};
    if( getIpAddr(ipAddr) )
    {
        UARTprintf("IP Address: %s stringlen: %d\n", ipAddr, strlen(ipAddr));
        memcpy(&g_currentDisplayString[20], ipAddr, strlen(ipAddr));
        
    }
    else
    {
        UARTprintf("Problem getting an ip address\n");
        memcpy(&g_currentDisplayString[20], "Error getting IP Address", strlen(ipAddr));
    }
    memset(g_currentDisplayString, (unsigned)' ', 20);
    // white space +the len + single space for the null character
    g_currentDisplayStrLen = (20+strlen(ipAddr));
    g_currentDisplayString[g_currentDisplayStrLen] = '\0';
    
    UARTprintf("g_currentDisplayStrLen = %d\n", g_currentDisplayStrLen);
    UARTprintf("g_currentDisplayString: %s\n", g_currentDisplayString);
    UARTprintf("sizeof(RowNum) == %d\n", sizeof(RowNum));
    UARTprintf("sizeof(short) == %d\n", sizeof(short));
    UARTprintf("Character number(utf) == %d as hex: 0x%8X\n", '\u25B2', '\u25B2');


    UARTprintf("Now beginning main loop...\n");

    // among other things, this should flush the uART!
    UARTStdioConfig(1, 115200, 16000000);

    // Set it back though so we can print things...
    UARTStdioConfig(0, 115200, 16000000);


    //
    // Get and clear the current interrupt source(s)
    //
    int ui32Interrupts = UARTIntStatus(UART1_BASE, true);
    UARTIntClear(UART1_BASE, ui32Interrupts);

    // aaaaaaaand enable the interrupts
    UARTIntDisable(UART1_BASE, 0xFFFFFFFF);
    UARTIntEnable(UART1_BASE, UART_INT_RX | UART_INT_RT);
    
    // 22 is the interrupt line for Uart1
    IntEnable(INT_UART1_TM4C123);
    // BAND-AID! Need to figure out why this is getting triggered.
    // I probably need to flush the UART before enabling interrupts
    interruptHappened = false;

    while(1<2)
    {
        if(interruptHappened)
        { 
            // Clear and turn interrupts off
            int ui32Interrupts = UARTIntStatus(UART1_BASE, true);
            UARTIntClear(UART1_BASE, ui32Interrupts);
            IntDisable(INT_UART1_TM4C123);

            interruptHappened = false;

            //UARTprintf("interrupt flag set. Populating buffer\n");
            if(g_rxSuccess)
            {
                // verify data is legit
                int len = 0;
                //sscanf(g_rxBuf, "+IPD,0,%d:", &len);

                //UARTprintf("Length based on header = %d\n", len);

                // Get the index for the start of our string (starts following colon)
                char* begin = strchr(g_rxBuf, (int)':');

                if(begin != NULL)
                {
                    // string starts after the colon
                    begin += 1;


                    //if( len == (g_rxBufLen-(abs(g_rxBuf-begin))) )
                    //{
                        // Set active length to new length of buffer
                        g_currentDisplayStrLen = g_rxBufLen + 20 - abs(g_rxBuf-begin);

                        UARTprintf("g_currentDisplayStrLen = %d\n", g_currentDisplayStrLen);

                        // Chop off the "0,CLOSED" at the end (if it exists)
                        // 8 characters plus 2 for the /n, and /r
                        if( strstr(begin+(g_currentDisplayStrLen-20-10), "CLOSED") != NULL )
                        {
                            UARTprintf("Found CLOSED at end of string... shortening length\n");
                            g_currentDisplayStrLen -= 10;
                        }
                        else
                        {
                            UARTprintf("Did not find CLOSED at end of the string\n");
                            // reduce the len to account for the last two /n and
                            //g_currentDisplayStrLen -= 2;
                        }

                        UARTprintf("g_currentDisplayStrLen = %d\n", g_currentDisplayStrLen);

                        // Clear out existing display string
                        memset(g_currentDisplayString, (unsigned)' ', g_currentDisplayStrLen);
                        // Copy in the new display data
                        memcpy(&g_currentDisplayString[20], begin, g_currentDisplayStrLen - 20);

                        UARTprintf("g_currentDisplayString: %s\n", g_currentDisplayString);

                        // Reset our column offset, we are restarting the scroll with the new data
                        columnOffset = 0;
                    //}
                    //else
                    //{
                    //    UARTprintf("Lengths do not match!\n");
                    //}

                }
                else
                {
                    UARTprintf("begin was null\n");
                }
                
            }

            // Clear and turn interrupts back on
            ui32Interrupts = UARTIntStatus(UART1_BASE, true);
            UARTIntClear(UART1_BASE, ui32Interrupts);
            IntEnable(INT_UART1_TM4C123);
        }

        //shift frame buffer by one column
        for(int i = 0; i < 120; ++i)
        {
            unsigned column = i+columnOffset;
            
            // Check if we are passed the end of the buffer
            // Start putting in empty columns if we are
            if(column/charWidth > g_currentDisplayStrLen)
            {
                frameBuf[120-1-i]=0x0000;
            }
            
            else
            {
                char currentChar = ' ';
                if(column/charWidth < g_currentDisplayStrLen)
                {
                    // given our current column, and character width, we get the current character.
                    currentChar = g_currentDisplayString[column/charWidth];
                }
                // Fill from the end of the buffer first. So: frameBuf[119], frameBuf[118]... etc.
                frameBuf[120-1-i] = characters[currentChar-32][column-(charWidth*(column/charWidth))];
            }
        }


        // THis is what we do for the majority of the time
        for(numFrameUpdates = 0; numFrameUpdates<updatesPerScroll; ++numFrameUpdates)
        {
            doFrame(frameBuf);
        }

        // 6 columns per character in buffer
        // to completely scroll all data off the screen
        if( columnOffset >= (g_currentDisplayStrLen*6) )
        {
            columnOffset = 0;
        }
        else
        {
            ++columnOffset;
        }
    }
}
