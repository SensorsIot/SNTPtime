/*

   SNTPtime for ESP8266
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

   V1.0 2016-6-28
 */
#include <SNTPtime.h>


SNTPtime NTPch("ch.pool.ntp.org");

/*
   The structure contains following fields:
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
 */
strDateTime dateTime;

void setup() {
        Serial.begin(115200);
        Serial.println();
        Serial.println("Booted");
        Serial.println("Connecting to Wi-Fi");

        WiFi.mode(WIFI_STA);
        WiFi.begin (ssid, password);
        while (WiFi.status() != WL_CONNECTED) {
                Serial.print(".");
                delay(500);
        }
        Serial.println();
        Serial.println("WiFi connected");

        while (!NTPch.setSNTPtime()) Serial.print("."); // set internal clock
        Serial.println();
        Serial.println("Time set");
}

void loop() {

        // first parameter: Time zone; second parameter: 1 for European summer time; 2 for US daylight saving time (not implemented yet)
        dateTime = NTPch.getTime(1.0, 1); // get time from internal clock
        NTPch.printDateTime(dateTime);

        byte actualHour = dateTime.hour;
        byte actualMinute = dateTime.minute;
        byte actualsecond = dateTime.second;
        int actualyear = dateTime.year;
        byte actualMonth = dateTime.month;
        byte actualday = dateTime.day;
        byte actualdayofWeek = dateTime.dayofWeek;
        delay(1000);
}
