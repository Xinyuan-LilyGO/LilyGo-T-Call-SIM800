// Please select the corresponding model

// #define SIM800L_IP5306_VERSION_20190610
// #define SIM800L_AXP192_VERSION_20200327
// #define SIM800C_AXP192_VERSION_20200609
#define SIM800L_IP5306_VERSION_20200811

#include "utilities.h"

#define TINY_GSM_MODEM_SIM800 // sim800 and sim7000 are very identical, no sim7000 reset tool yet but sim800 works

#include <TinyGsmClient.h>


// Set serial for AT commands (to the module)
#define SerialAT Serial1

// See all AT commands, if wanted
#define DUMP_AT_COMMANDS

#ifdef DUMP_AT_COMMANDS
#include <StreamDebugger.h>    // if enabled it requires the streamDebugger lib
StreamDebugger debugger(SerialAT, Serial);
TinyGsm modem(debugger);
#else
TinyGsm modem(SerialAT);
#endif

bool reply = false;

void setup()
{
    // Set console and modem baud rate
    Serial.begin(115200);

    delay(10);

    setupModem();

    // Set GSM module baud rate and UART pins
    SerialAT.begin(115200, SERIAL_8N1, MODEM_RX, MODEM_TX);

    int i = 10;
    Serial.println("\nTesting Modem Response...\n");
    Serial.println("****");
    while (i) {
        SerialAT.println("AT");
        delay(500);
        if (SerialAT.available()) {
            String r = SerialAT.readString();
            Serial.println(r);
            if ( r.indexOf("OK") >= 0 ) {
                reply = true;
                break;;
            }
        }
        delay(500);
        i--;
    }
    Serial.println("****\n");

    if (reply) {

        bool ret = modem.factoryDefault();

        Serial.println(F("\n***********************************************************"));
        Serial.print  (F(" Reset settings to Factory Default: "));
        Serial.println((ret) ? "OK" : "FAIL");
        Serial.println(F("***********************************************************"));
    } else {
        Serial.println(F("***********************************************************"));
        Serial.println(F(" Failed to connect to the modem! Check the baud and try again."));
        Serial.println(F("***********************************************************\n"));
    }
}

void loop()
{
    while (SerialAT.available()) {
        Serial.write(SerialAT.read());
    }
    while (Serial.available()) {
        SerialAT.write(Serial.read());
    }
}
