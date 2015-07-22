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
bool interruptHappened = false;
// Treat the beginning of execution like we got new data
bool gotNewData = true;
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
const int g_rxMaxBufLen = 256+20;
int g_rxBufLen = 0;
char g_rxBuf[256+20] = {0};

// g_currentDisplayString holds the string to display on the matrix
int g_currentDisplayStrLen = 0;
char g_currentDisplayString[256+20] = {0};



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


void uartCallback()
{
    uint32_t ui32Interrupts;

    UARTprintf("INside uart callback!\n");

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
    IntDisable(INT_UART1_TM4C123); 

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
        interruptHappened = true;
        while( !done )
        {
            if( UARTCharsAvail(UART1_BASE) )
            {
                tempBuf[i] = (char)UARTCharGetNonBlocking(UART1_BASE);
                i++;
            }

            // Commands should always end with a carriage return and newline
            if( ((i>=2)||(i>=g_rxMaxBufLen)) && (tempBuf[i-2] == '\r') && (tempBuf[i-1] == '\n') )
            {
                done = true;
                // We need 20 extra bytes for leading spaces
                g_rxBufLen = i+20;

                // Ensure we got appropriate response
                if( !strstr(tempBuf, "OK") )
                {   
                    // Clear the global buffer
                    memset(g_rxBuf, 0x00, g_rxBufLen);
                    // Put in the leading spaces
                    memset(g_rxBuf, ' ', 20);
                    // Copy the temp buffer into it offset 20 spaces
                    strcpy(&g_rxBuf[20], tempBuf);

                    // Null terminate this guy
                    g_rxBuf[(i-2) + 20] = '\0'; 
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
    IntEnable(INT_UART1_TM4C123);
}



int main(void)
{

    // Delay for a bit
    //SysCtlDelay(80000000);


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


    //
    // We are finished.  Hang around doing nothing.
    //
    unsigned counter =0;
    unsigned i = 0;
    bool done = false;
    char temp = 0;
    const unsigned timeoutLimit = 0x0000FFFF;
    memset(g_rxBuf, 0x00, g_rxMaxBufLen);
    static const short charWidth = 6;
    unsigned numFrameUpdates = 0;
    unsigned updatesPerScroll = 4;
    // keep track of the overall columnOffset
    unsigned columnOffset = 0;

    //unsigned numCharsInBuf = 117 + 20;

    // frameBuf holds the row information for each column
    RowNum frameBuf[120] = {0};

    // Screen can handle 20 characters at a time
    //const char* rxString = "                    ABCDEFGHIJKLMNOPQRSTUVWXYZ abcdefghijklmnopqrstuvwxyz 0123456789 ~!@#$\%^&*()_+`\\|?/.>,<";
    //const char* rxString = "                    07/18/15. Weather: Sunny, ^96  67\% Chance of thunderstorms in the afternoon. Now Playing: How soon is now, The Smiths";
    //const char* rxString = "                    @essentialBenyc: I am getting pretty tired of looking at labels. Can't someone just tell me what is good for me?! #fuckTheGrocery @giantGroceryStore" ;
    
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



    // aaaaaaaand enable the interrupts
    UARTIntDisable(UART1_BASE, 0xFFFFFFFF);
    UARTIntEnable(UART1_BASE, UART_INT_RX | UART_INT_RT);
    
    // 22 is the interrupt line for Uart1
    IntEnable(22);

    SysCtlDelay(500000);
    UARTprintf("Now beginning main loop...\n");
    // BAND-AID! Need to figure out why this is getting triggered.
    // I probably need to flush the UART before enabling interrupts
    interruptHappened = false;

    while(1<2)
    {
        numFrameUpdates = 0;
        // THis is what we do for the majority of the time
        while(numFrameUpdates < updatesPerScroll)
        { 
            doFrame(frameBuf);
            ++numFrameUpdates;
            //delay here to have time to stay off
            //SysCtlDelay( 200 );
            //turnAllRowsOff();
            //disableAllDmux();
            //SysCtlDelay(80);
        }

        if(interruptHappened)
        {
            interruptHappened = false;

            UARTprintf("interrupt flag set. Populating buffer\n");
            // Set active length to new length of buffer
            g_currentDisplayStrLen = g_rxBufLen;

            UARTprintf("g_currentDisplayStrLen = %d\n", g_currentDisplayStrLen);
            // Clear out existing display string
            memset(g_currentDisplayString, 0x00, g_currentDisplayStrLen);
            // Copy in the new display data
            memcpy(g_currentDisplayString, g_rxBuf, g_currentDisplayStrLen);

            //UARTprintf("g_currentDisplayString: %s\n", g_currentDisplayString);

            /*
            // Build our buffer
            memset(frameBuf, 0x0000, 120);
            for(int i = 0; i < 120; ++i)
            {
                // Index characters with ascii characters -32 (cool right!?)
                frameBuf[120-1-i] = characters[ g_rxBuf[i/charWidth]-32][i-(charWidth*(i/charWidth))];
                //frameBuf[120-1-i] = characters[currentChar-32][column-(charWidth*(column/charWidth))];
            }
            */

            // Reset our column offset, we are restarting the scroll with the new data
            columnOffset = 0;
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
                    currentChar = g_currentDisplayString[(column)/charWidth];
                }
                // Fill from the end of the buffer first. So: frameBuf[119], frameBuf[118]... etc.
                frameBuf[120-1-i] = characters[currentChar-32][column-(charWidth*(column/charWidth))];
            }
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
        //SysCtlDelay(40);
    }
}
