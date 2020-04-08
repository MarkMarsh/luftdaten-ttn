/*******************************************************************************
 * Derived from LMIC-Arduino APB sample file - copyright message below
 * 
 * Modified to poll the sensors frequently but only send a packet
 * if data is available.
 *  
 *  -------------------------------------------------------------------
 * Copyright (c) 2015 Thomas Telkamp and Matthijs Kooijman
 *
 * Permission is hereby granted, free of charge, to anyone
 * obtaining a copy of this document and accompanying files,
 * to do whatever they want with them without any restriction,
 * including, but not limited to, copying, modification and redistribution.
 * NO WARRANTY OF ANY KIND IS PROVIDED.
 *
 * This example sends a valid LoRaWAN packet with payload "Hello,
 * world!", using frequency and encryption settings matching those of
 * the The Things Network.
 *
 * This uses OTAA (Over-the-air activation), where where a DevEUI and
 * application key is configured, which are used in an over-the-air
 * activation procedure where a DevAddr and session keys are
 * assigned/generated for use with all further communication.
 *
 * Note: LoRaWAN per sub-band duty-cycle limitation is enforced (1% in
 * g1, 0.1% in g2), but not the TTN fair usage policy (which is probably
 * violated by this sketch when left running for longer)!

 * To use this sketch, first register your application and device with
 * the things network, to set or generate an AppEUI, DevEUI and AppKey.
 * Multiple devices can use the same AppEUI, but each device has its own
 * DevEUI and AppKey.
 *
 * Do not forget to define the radio type correctly in config.h.
 *
 *******************************************************************************/

#include <lmic.h>
#include <hal/hal.h>
#include <SPI.h>

#include "luftdaten-ttn.h"
#include "particulates.h"
#include "temp-humid.h"

// see the file ttn-keys.h.tpl in the repo
#include "ttn-keys.h"

// check the library has been set to the EU frequency
#if CFG_eu868 != 1
#pragma error "You must edit project_config/lmic_project_config.h in the library (yes really) to set the frequency"
#endif

// provide callbacks for the OTAA join to fetch the keys
void os_getArtEui (u1_t* buf) { memcpy_P(buf, APPEUI, 8); }
void os_getDevEui (u1_t* buf) { memcpy_P(buf, DEVEUI, 8); }
void os_getDevKey (u1_t* buf) {  memcpy_P(buf, APPKEY, 16); }

static osjob_t sendjob;

// Pin mapping
const lmic_pinmap lmic_pins = {
    .nss = 15,
    .rxtx = LMIC_UNUSED_PIN,
    .rst = LMIC_UNUSED_PIN,
    .dio = {5, 16, LMIC_UNUSED_PIN},
};

static uint8_t dataTX[8];
static int16_t * dataTX16 = (int16_t *) dataTX;

// Q&D encoder - i needs to be word aligned
void encodeFloat(float f, int i, int multiplier) { dataTX16[i/2] = ((int16_t) (f * multiplier)); }

const int POLL_INTERVAL = 1;

void lmicCallback(osjob_t* j)  {
    DEBUG_MSG("Starting lmicCallback()\n");

    // Don't poll the sensors if there's a current TX/RX job running
    if (LMIC.opmode & OP_TXRXPEND) {
        DEBUG_MSG("OP_TXRXPEND, not sending\n");
    }
    else  {
        // check to see if there's data to read and send
        // - this is effectively loop()
        reading rdg;
        if(particulates_read(rdg) && temp_humid_read(rdg))  {
            // encode data
            encodeFloat(rdg.temperature, 0, 100);
            encodeFloat(rdg.humidity, 2, 100);
            encodeFloat(rdg.pm25, 4, 10);
            encodeFloat(rdg.pm10, 6, 10);
            // Prepare upstream data transmission at the next possible time.
            LMIC_setTxData2(1, dataTX, sizeof(dataTX), 0);
            DEBUG_MSG("Packet queued\n");
        }
    }
    // always initiate another callback
    os_setTimedCallback(&sendjob, os_getTime()+sec2osticks(POLL_INTERVAL), lmicCallback);
}

/* TTN decoder javascript is:
function Decoder(bytes, port) {
  // Decode an uplink message from a buffer
  // (array) of bytes to an object of fields.
  var decoded = {};

  if (port === 1) {
    decoded.temperature = (bytes[0] + (bytes[1] * 256)) / 100.0;
    decoded.humidity = (bytes[2] + (bytes[3] * 256)) / 100.0;
    decoded.pm25 = (bytes[4] + (bytes[5] * 256)) / 10.0;
    decoded.pm10 = (bytes[6] + (bytes[7] * 256)) / 10.0;
  }
  return decoded;
}
*/

void onEvent (ev_t ev) {
    DEBUG_MSG("%d: ", os_getTime());
    switch(ev) {
        case EV_SCAN_TIMEOUT:
            DEBUG_MSG("EV_SCAN_TIMEOUT\n");
            break;
        case EV_BEACON_FOUND:
            DEBUG_MSG("EV_BEACON_FOUND\n");
            break;
        case EV_BEACON_MISSED:
            DEBUG_MSG("EV_BEACON_MISSED\n");
            break;
        case EV_BEACON_TRACKED:
            DEBUG_MSG("EV_BEACON_TRACKED\n");
            break;
        case EV_JOINING:
            DEBUG_MSG("EV_JOINING\n");
            break;
        case EV_JOINED:
            DEBUG_MSG("EV_JOINED\n");
            // Disable link check validation (automatically enabled
            // during join, but not supported by TTN at this time).
            LMIC_setLinkCheckMode(0);
            break;
        case EV_RFU1:
            DEBUG_MSG("EV_RFU1\n");
            break;
        case EV_JOIN_FAILED:
            DEBUG_MSG("EV_JOIN_FAILED\n");
            break;
        case EV_REJOIN_FAILED:
            DEBUG_MSG("EV_REJOIN_FAILED\n");
            break;
            break;
        case EV_TXCOMPLETE:
            DEBUG_MSG("EV_TXCOMPLETE (includes waiting for RX windows)\n");
            if (LMIC.txrxFlags & TXRX_ACK)
              DEBUG_MSG("Received ack\n");
            if (LMIC.dataLen) {
              DEBUG_MSG("Received %d bytes of payload : ", LMIC.dataLen);
            }
            break;
        case EV_LOST_TSYNC:
            DEBUG_MSG("EV_LOST_TSYNC\n");
            break;
        case EV_RESET:
            DEBUG_MSG("EV_RESET\n");
            break;
        case EV_RXCOMPLETE:
            // data received in ping slot
            DEBUG_MSG("EV_RXCOMPLETE\n");
            break;
        case EV_LINK_DEAD:
            DEBUG_MSG("EV_LINK_DEAD\n");
            break;
        case EV_LINK_ALIVE:
            DEBUG_MSG("EV_LINK_ALIVE\n");
            break;
         default:
            DEBUG_MSG("Unknown event\n");
            break;
    }
}

void setup() {
    Serial.begin(115200);
    DEBUG_MSG("Starting\n");

    // setup the sensors
    particulates_setup();
    temp_humid_setup();

    #ifdef VCC_ENABLE
    // For Pinoccio Scout boards
    pinMode(VCC_ENABLE, OUTPUT); 
    digitalWrite(VCC_ENABLE, HIGH);
    delay(1000);
    #endif

    // LMIC init
    os_init();

    // Reset the MAC state. Session and pending data transfers will be discarded.
    LMIC_reset();

    // Start job (sending automatically starts OTAA too)
    lmicCallback(&sendjob);
}

void loop() {
    os_runloop_once();
}
