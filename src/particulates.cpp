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

#include <SdsDustSensor.h>
#include "luftdaten-ttn.h"
#include "particulates.h"

#ifdef SIMULATE_PARTICULATES
  void particulates_setup() {
    DEBUG_MSG("Simulating particulate readings\n");
  }
  
  boolean particulates_read(reading &rdg) {
      static int ctr = 0;
      if(ctr++ % 60 == 0)  {  	// return valid data once every 60 polls
        rdg.pm25 = 2.5;
        rdg.pm10 = 10.0;    
        return true;
      }
    return false;
  }
#else

  SdsDustSensor *sds = NULL;
  
  void particulates_setup() {
    #if defined(TTGO_TBEAM)
      Serial2.begin(9600, SERIAL_8N1, SDS011_RXPIN, SDS011_TXPIN);
      sds = new SdsDustSensor(Serial2);
    #endif
    #if defined(SAULMOD)
    sds = new SdsDustSensor(SDS011_RXPIN, SDS011_TXPIN);
    sds->begin();
    #endif

    DEBUG_MSG(sds->queryFirmwareVersion().toString().c_str()); DEBUG_MSG("\n"); // prints firmware version
    DEBUG_MSG(sds->setActiveReportingMode().toString().c_str()); DEBUG_MSG("\n"); // ensures sensor is in 'active' reporting mode
    DEBUG_MSG(sds->setCustomWorkingPeriod(SAMPLE_PERIOD).toString().c_str()); DEBUG_MSG("\n"); // sensor sends data every 3 minutes
  }

  boolean particulates_read(reading &rdg) {
    PmResult pm = sds->readPm();
    if (pm.isOk()) {
      DEBUG_MSG("%lu : PM2.5 = %f, PM10 = %f                                              \n", millis() / 1000, pm.pm25, pm.pm10);
      rdg.pm25 = pm.pm25;
      rdg.pm10 = pm.pm10;    
      return true;
    }
    // we expect reads to fail as the polling frequency is higher than the sample frequency
    // DEBUG_MSG("%lu : Could not read values from sensor, reason: %s\r", millis() / 1000, pm.statusToString().c_str());
    return false;
  }
#endif
