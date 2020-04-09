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

#include <Arduino.h>

#ifdef TTGO_TBEAM
#define PROGMEM
#define memcpy_P memcpy

// nicked from TTNMapper...
// -----------------------------------------------------------------------------
// General
// -----------------------------------------------------------------------------

#define I2C_SDA            21
#define I2C_SCL            22
#define LED_PIN            25
#define RELAIS_PIN         14    // confirmed pin 14 works with board

// the pins used for SDS011 serial communication
#define SDS011_RXPIN 39  // Connect to SDS011 TX
#define SDS011_TXPIN 36  // Connect to SDS011 RX


// -----------------------------------------------------------------------------
// OLED
// -----------------------------------------------------------------------------

#define SSD1306_ADDRESS 0x3C
#define BMP280_ADDRESS 0x76

// -----------------------------------------------------------------------------
// LoRa SPI
// -----------------------------------------------------------------------------

#define SCK_GPIO        5
#define MISO_GPIO       19
#define MOSI_GPIO       27
#define NSS_GPIO        18
#define RESET_GPIO      23
#define DIO0_GPIO       26
#define DIO1_GPIO       33
#define DIO2_GPIO       32

// -----------------------------------------------------------------------------
// AXP192 (Rev1-specific options)
// -----------------------------------------------------------------------------

#define LORA_POWER_CTRL_CH    2
#define PMU_IRQ               35

#endif

#ifdef SAULMOD
// the pins used for SDS011 serial communication
#define SDS011_RXPIN D2  // Connect to SDS011 TX
#define SDS011_TXPIN D4  // Connect to SDS011 RX
// the pin that the DHT22 sensor is attached to
#define DHT_SENSOR_PIN D3
#endif

// comment out to remove debug output to the USB Serial port
#define DEBUG_SERIAL

#ifdef DEBUG_SERIAL
#define SETUP_DEBUG_SERIAL() Serial.begin(115200); Serial.print("\n\n")
#define DEBUG_MSG(...) Serial.printf( __VA_ARGS__ )
#else
#define SETUP_DEBUG_SERIAL()
#define DEBUG_MSG(...)
#endif



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