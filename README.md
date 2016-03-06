# LedMarquee
LED Matrix using Tiva Launchpad EK-TM4C123GXL and esp8266

Building
--------

create the file "networkCredentials.h" in the root directory and make it look like this:
'''c
#ifndef _NETWORK_CREDENTIALS_
#define _NETWORK_CREDENTIALS_


#define WIFI_SSID "someNetworkName"
#define WIFI_PSWD "SomethingSecret"

#endif
'''


