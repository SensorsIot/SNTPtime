# SNTPtime
Library for ESP8266 to get time from NTP servers using SNTP protocol

This library is based on the native SNTP implementation of espressif. It has two main functions:

setSNTPtime: This function reads the actual time from a NTP server and sets the internal ESP RTC to UTC
getTime: This function reads the ESP internal clock, adjust it to the time zone and eventually 
to summer time and fills the structure dateTime. This function does not need network connection, but is reset during deep sleep.
Pay attention: the internal RTC is not very precise and has to be synchrinized from time to time with a NTP server.
