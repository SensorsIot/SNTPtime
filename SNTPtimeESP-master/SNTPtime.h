/*
SNTP library for ESP8266


This routine gets the unixtime from a NTP server and adjusts it to the time zone and the
Middle European summer time if requested
   
Copyright (c) 2016 Andreas Spiess

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

V1.0 2016-8-3

*/

#ifndef NTPtime_h
#define NTPtime_h

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WiFiUDP.h>
#include <time.h>


#define DEBUG_ON



struct strDateTime
{
  byte hour;
  byte minute;
  byte second;
  int year;
  byte month;
  byte day;
  byte dayofWeek;
  boolean valid;
};

class SNTPtime {
  public:
    SNTPtime(char *NTPServer);
    strDateTime getTime(double _timeZone, boolean _DayLightSaving);
    void printDateTime(strDateTime _dateTime);
    boolean setSNTPtime();

  private:
    strDateTime ConvertUnixTimestamp( unsigned long _tempTimeStamp);
    boolean summerTime(unsigned long _timeStamp );
    boolean daylightSavingTime(unsigned long _timeStamp);
    unsigned long adjustTimeZone(unsigned long _timeStamp, double _timeZone, byte _DayLightSavingSaving);
};
#endif
