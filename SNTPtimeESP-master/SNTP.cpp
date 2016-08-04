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

#include <Arduino.h>
#include "SNTPtime.h"

#define LEAP_YEAR(Y) ( ((1970+Y)>0) && !((1970+Y)%4) && ( ((1970+Y)%100) || !((1970+Y)%400) ) )

const int NTP_PACKET_SIZE = 48;
byte _packetBuffer[ NTP_PACKET_SIZE];
static const uint8_t _monthDays[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

char *_NTPserver="";

// NTPserver is the name of the NTPserver

SNTPtime::SNTPtime(char *NTPserver) {
  _NTPserver = NTPserver;
}

// Converts a unix time stamp to a strDateTime structure
strDateTime SNTPtime::ConvertUnixTimestamp( unsigned long _tempTimeStamp) {
  strDateTime _tempDateTime;
  uint8_t _year, _month, _monthLength;
  uint32_t _time;
  unsigned long _days;

  _time = (uint32_t)_tempTimeStamp;
  _tempDateTime.second = _time % 60;
  _time /= 60; // now it is minutes
  _tempDateTime.minute = _time % 60;
  _time /= 60; // now it is hours
  _tempDateTime.hour = _time % 24;
  _time /= 24; // now it is _days
  _tempDateTime.dayofWeek = ((_time + 4) % 7) + 1;  // Sunday is day 1

  _year = 0;
  _days = 0;
  while ((unsigned)(_days += (LEAP_YEAR(_year) ? 366 : 365)) <= _time) {
    _year++;
  }
  _tempDateTime.year = _year; // year is offset from 1970

  _days -= LEAP_YEAR(_year) ? 366 : 365;
  _time  -= _days; // now it is days in this year, starting at 0

  _days = 0;
  _month = 0;
  _monthLength = 0;
  for (_month = 0; _month < 12; _month++) {
    if (_month == 1) { // february
      if (LEAP_YEAR(_year)) {
        _monthLength = 29;
      } else {
        _monthLength = 28;
      }
    } else {
      _monthLength = _monthDays[_month];
    }

    if (_time >= _monthLength) {
      _time -= _monthLength;
    } else {
      break;
    }
  }
  _tempDateTime.month = _month + 1;  // jan is month 1
  _tempDateTime.day = _time + 1;     // day of month
  _tempDateTime.year += 1970;

  return _tempDateTime;
}


//
// Summertime calculates the daylight saving time for middle Europe. Input: Unixtime in UTC
//
boolean SNTPtime::summerTime(unsigned long _timeStamp ) {

  strDateTime  _tempDateTime;
  _tempDateTime = ConvertUnixTimestamp(_timeStamp);
  // printTime("Innerhalb ", _tempDateTime);

  if (_tempDateTime.month < 3 || _tempDateTime.month > 10) return false; // keine Sommerzeit in Jan, Feb, Nov, Dez
  if (_tempDateTime.month > 3 && _tempDateTime.month < 10) return true; // Sommerzeit in Apr, Mai, Jun, Jul, Aug, Sep
  if (_tempDateTime.month == 3 && (_tempDateTime.hour + 24 * _tempDateTime.day) >= (3 +  24 * (31 - (5 * _tempDateTime.year / 4 + 4) % 7)) || _tempDateTime.month == 10 && (_tempDateTime.hour + 24 * _tempDateTime.day) < (3 +  24 * (31 - (5 * _tempDateTime.year / 4 + 1) % 7)))
    return true;
  else
    return false;
}

boolean SNTPtime::daylightSavingTime(unsigned long _timeStamp) {

  strDateTime  _tempDateTime;
  _tempDateTime = ConvertUnixTimestamp(_timeStamp);

// here the US code
    return false;
}


unsigned long SNTPtime::adjustTimeZone(unsigned long _timeStamp, double _timeZone, byte _DayLightSaving) {
  strDateTime _tempDateTime;
  _timeStamp += _timeZone *  3600; // adjust timezone
  if (_DayLightSaving ==1 && summerTime(_timeStamp)) _timeStamp += 3600; // European Summer time
  if (_DayLightSaving ==2 && daylightSavingTime(_timeStamp)) _timeStamp += 3600; // US daylight time
  return _timeStamp;
}




boolean SNTPtime::setSNTPtime() {

unsigned long entry=millis();
	do {
		configTime(0,0,_NTPserver);
  		delay(500);
	} while (millis()-entry<5000 && time(NULL)<946684800); // 1.1.2000
	if (time(NULL)>100) return true;
	else return false;
}

// time zone is the difference to UTC in hours
// if _isDayLightSaving is 0, time in timezone is returned
// if _isDayLightSaving is 1, time will be adjusted to European Summer time


strDateTime SNTPtime::getTime(double _timeZone, boolean _DayLightSaving)
{
	unsigned long  _unixTime = time(NULL);
	Serial.println(_unixTime);
	unsigned long  _currentTimeStamp = adjustTimeZone(_unixTime, _timeZone, _DayLightSaving);
	strDateTime _dateTime = ConvertUnixTimestamp(_currentTimeStamp);
	
	return _dateTime;
}

void SNTPtime::printDateTime(strDateTime _dateTime) {

  Serial.print(_dateTime.year);
  Serial.print( "-");
  Serial.print(_dateTime.month);
  Serial.print( "-");
  Serial.print(_dateTime.day);
  Serial.print( "-");
  Serial.print(_dateTime.dayofWeek);
  Serial.print( " ");

  Serial.print(_dateTime.hour);
  Serial.print( "H ");
  Serial.print(_dateTime.minute);
  Serial.print( "M ");
  Serial.print(_dateTime.second);
  Serial.print( "S ");
  Serial.println();
}
