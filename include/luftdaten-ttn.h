/*
Copyright 2020 Mark Marsh

Permission is hereby granted, free of charge, to any person obtaining 
a copy of this software and associated documentation files (the "Software"), 
to deal in the Software without restriction, including without limitation 
the rights to use, copy, modify, merge, publish, distribute, sublicense, 
and/or sell copies of the Software, and to permit persons to whom the 
Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included 
in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, 
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF 
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. 
IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY 
CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, 
TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE 
SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

/*
Ideas / ToDo list

Discard queued SDS011 readings accumulated while waiting for attach
  - maybe do multiple reads until failure

Try multiple reads for the Humidity sensor (maybe I've got a bad one)

Try swapping pins around so DHT22 not on D4

*/

#pragma once

// We can operate in two modes, with a serial port for the SDS011 on D2. D3 or 
// with the i2c bus on D2, D3 
#define MODE_SERIAL 1
#define MODE_I2C 2

#define MODE MODE_SERIAL

// comment out to remove debug output to the USB Serial port
#define DEBUG_SERIAL

#ifdef DEBUG_SERIAL
#define SETUP_DEBUG_SERIAL() Serial.begin(115200); DEBUG_MSG("\n\n")
#define DEBUG_MSG(...) Serial.printf( __VA_ARGS__ )
#else
#define SETUP_DEBUG_SERIAL()
#define DEBUG_MSG(...)
#endif

// the pin that the DHT22 sensor is attached to
#define DHT_SENSOR_PIN D3

// the pins used for SDS011 serial communication
#define PARTICULATE_RX_PIN D2
#define PARTICULATE_TX_PIN D4

/*
  the data we're going to send to TTN
*/
struct reading
{
    float temperature;
    float humidity;
    float pm25;
    float pm10;
};

// how many minutes betweeen samples for the SDS011 sensor
const int SAMPLE_PERIOD = 3;

// how many seconds between polls of the SDS011 sensor
const int SENSOR_POLL_INTERVAL = 10;