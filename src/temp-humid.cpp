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

#include <DHT.h>
#include "luftdaten-ttn.h"

DHT dht(DHT_SENSOR_PIN, DHT22);

void temp_humid_setup() {
  dht.begin();
}

boolean temp_humid_read(reading &rdg) {
  rdg.humidity = dht.readHumidity();
  rdg.temperature = dht.readTemperature();

  if (isnan(rdg.humidity) || isnan(rdg.temperature)) {
    DEBUG_MSG("Failed to read from DHT sensor :-( - temp = %f, humid = %f\n", rdg.temperature, rdg.humidity);
    return false;
  }
  //DEBUG_MSG("Temperature = %f, Humidity = %f\n", rdg.temperature, rdg.humidity);
  return true;
}
