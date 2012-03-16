#ifndef __CREDENTIALS_H__
#define __CREDENTIALS_H__

// supply your own Pachube feed ID
#define PACHUBEFEED "50477"
//#define PACHUBEFEED "46342"
// this API key will only work from my IP address - you need to supply your own
#define APIKEY "YOUR API KEY HERE"
#define TIMETOUPDATE 15000  // frequency of update - every 15 seconds

// Wifi parameters
char passphrase[] = "your passphrase here";
char ssid[] = "your ssid";
boolean mode = WPA_MODE; //or WEP_MODE

#endif
