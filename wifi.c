
#include <stdint.h>
/*#include <stdio.h>*/
#include <string.h>
#include <stdbool.h>

#include "driverlib/uart.h"
#include "utils/uartstdio.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "inc/hw_memmap.h"

#include "wifi.h"
#include "networkCredentials.h"

// A regular command returns OK when good, ERROR when failed
bool processRegularCommand(char* atCommand, char* waitForString, unsigned retryLimit, char* response, unsigned maxLen)
{
	const unsigned rxBufLen = 200;
    char rxBuf[200] = {0};
    int counter = 0;
    int i = 0;
    int index = 0;
    unsigned retryCounter = 0;

    // Take control of the stdio UART
    // The wifi chip is connected to UART1
    UARTStdioConfig(1, 115200, 16000000);
    // Can we even talk to the device?
    while(1<2)
    {
    	// First time, and every time counter is reset, we try again
    	if( counter == 0)
    	{
    		// Send command
    		if( !(atCommand[0] == '\0') )
    		{
		    	UARTprintf("%s\r\n", atCommand);
		    }

		    // If they didn't set a limit, we try forever
		   	if( retryLimit && (retryCounter >= retryLimit) )
		    {
		    	UARTStdioConfig(0, 115200, 16000000);
		    	return false;
		    }
			
			// Clean out the buffer
            memset(rxBuf, 0x00, rxBufLen);

			index = 0;  //resending the command, so reset the index
		    retryCounter++;

		    // THis is for debug
		    counter++;
    	}

        // Check if data exists on uart1 (non usb uart)
        if( UARTCharsAvail(UART1_BASE) )
        {
            rxBuf[index] = (char)UARTCharGetNonBlocking(UART1_BASE);
            index++;
	        
	        // Go until we get a carriage return
			//while( rxBuf[i-1] != '\n' );
            /*
	        UARTStdioConfig(0, 115200, 16000000);
            
            
            for(i=0; i<=strlen(rxBuf); i++)
            {
            	UARTprintf("rxBuf[%d]: %c = %d\n", i, rxBuf[i], rxBuf[i]);
            }
            
            
            
            //UARTprintf("wifi chip gave us: %s\n", rxBuf);

            UARTStdioConfig(1, 115200, 16000000);

            */
			// At this point we have some chunk of characters
	        //char expectedResponse[20] = {0};
	        
	        //Find the substring in the string
	        /*response = strstr(rxBuf, waitForString);

	        //sprintf(expectedResponse, "%s\r\n", waitForString);
	        strcat(expectedResponse, waitForString);
	        strcat(expectedResponse, "\r\n");*/

            if( strstr(rxBuf, waitForString) )
            {
            	UARTStdioConfig(0, 115200, 16000000);
                
                // Copy data back to user if they passed in a valid buffer and length
                if( (response!=0) && (maxLen != 0) )
                {
                    memcpy(response, rxBuf, maxLen);
                }
                return true;
            }
            else if( strstr(rxBuf, "ERROR") )
            {
            	UARTStdioConfig(0, 115200, 16000000);
            	return false;
            }
            else if( strstr(rxBuf, "no change") )
            {
            	UARTStdioConfig(0, 115200, 16000000);
            	return true;
            }
            else if( strstr(rxBuf, "\r\n") )
            {
            	// So the data we received so far was useless. 
            	// Go back to zero
            	memset(rxBuf, 0x00, rxBufLen);
            	index = 0;
            }
			
			/*
            UARTStdioConfig(0, 115200, 16000000);
            
            for(i=0;i<strlen(response);i++)
            {
            	UARTprintf("response[%d]: %c = %d\n", i, response[i], response[i]);
            }
            
            
            //UARTprintf("wifi chip gave us: %s\n", rxBuf);

            UARTStdioConfig(1, 115200, 16000000);
            */
        }
        /*
        else
        {
        	//SysCtlDelay(SysCtlClockGet() / 3);
        	counter++;

        	// Reset counter
        	if(counter>15)
        	{
        		//counter = 0;
        	}
        }
        counter++;
        */
    }
}

void getValue(char* atCommand, char* response, unsigned maxLen){
    // Take control of the stdio UART
    // The wifi chip is connected to UART1
    UARTStdioConfig(1, 115200, 16000000);
    char rxBuf[200] = {0};
    unsigned index = 0;

    // Send command
    if( !(atCommand[0] == '\0') )
    {
        UARTprintf("%s\r\n", atCommand);
    }

    // read rxBuf

    // Check if data exists on uart1 (non usb uart)
    do
    {
        if( UARTCharsAvail(UART1_BASE) )
        {
            rxBuf[index] = (char)UARTCharGetNonBlocking(UART1_BASE);
            index++;
        }
    }while(index < maxLen);

    memcpy(response, rxBuf, maxLen);
    UARTStdioConfig(0, 115200, 16000000);
    return;



    // parse
}


bool configureWifiChip()
{
	const unsigned rxBufLen = 50;
    char rxBuf[50] = {0};
    bool done = false;
    int counter = 0;
    int i = 0;

    // TODO, Handle if the chip is in another baud rate
    if( !processRegularCommand("AT", "OK", 0, 0, 0) )
    {
    	return false;
    }

    UARTprintf("Yay! wifi chip is responding to AT commands!\n");

    //Get wifi chip into a known state by resetting the chip
    if( !processRegularCommand("AT+RST", "OK", 0, 0, 0) )
    {
    	return false;
    }

    if( !processRegularCommand("", "ready", 0, 0, 0) )
    {
    	UARTprintf("Failed to reset wifi chip \n");
    	return false;
    }

    UARTprintf("Got past the reset!\n");

    if( !processRegularCommand("AT+CWMODE=3", "OK", 0, 0, 0) )
    {
    	UARTprintf("Failed to set CWMODE=3.\n");
    	return false;
    }

    UARTprintf("Got past mode setting!\n");

    // THe command looks like this "AT+CWJAP=myHomeNetworkName,catsAreCool"
    if( !processRegularCommand(WIFI_ID_AND_PASS_CMD, "OK", 0, 0, 0) )
    {
    	UARTprintf("Failed to connect to network\n");
    	return false;
    }

    UARTprintf("Got past wifi network setting\n");

    char response[200] = {0};
    unsigned maxLen = 64;
    if( !processRegularCommand("AT+CIFSR", "192.168.1.", 0, response, maxLen) )
    {
    	UARTprintf("Failed to get an ip address\n");
    	return false;
    }

    UARTprintf("Got past getting an ip address: %s\n", response);

    unsigned numChars = 130;
    memset(response, 0x00, maxLen);
    getValue("AT+CIFSR", response, numChars);

    for(int i = 0; i<numChars; i++)
    {
        UARTprintf("response[%d] = %c (0x%02X)\n", i, (unsigned)response[i] );
    }


    if( !processRegularCommand("AT+CIPMUX=1", "OK", 0, 0, 0) )
    {
    	UARTprintf("Failed to set CIPMUX=1.\n");
    	return false;
    }

    UARTprintf("Got past CIPMUX\n");

    if( !processRegularCommand("AT+CIPSERVER=1,1336", "OK", 0, 0, 0) )
    {
    	UARTprintf("Failed to activate server on port 1336.\n");
    	return false;
    }


	SysCtlDelay(SysCtlClockGet() / 50 / 3);
	UARTprintf("Wifi module configured correctly\n");
	return true;
}

/*
// Regular commands require no response. Simple set and forget
bool processRegularCommand( ATCommandEnum atCommand )
{
	char rxBuf[32] = {0};
	UARTprintf( "%s\r", atCommandStr[atCommand] );

	UARTgets(rxBuf, bufLen); //Blocks until we get something
	if( strcmp(rxBuf, "ok") )
	{
		return true;
	}

	return false;
}

*/