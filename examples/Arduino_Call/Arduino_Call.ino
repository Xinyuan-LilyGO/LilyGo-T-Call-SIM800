// Please select the corresponding model

// #define SIM800L_IP5306_VERSION_20190610
// #define SIM800L_AXP192_VERSION_20200327
// #define SIM800C_AXP192_VERSION_20200609
#define SIM800L_IP5306_VERSION_20200811

// Define the serial console for debug prints, if needed
#define DUMP_AT_COMMANDS
#define TINY_GSM_DEBUG          SerialMon

#include "utilities.h"

// Set serial for debug console (to the Serial Monitor, default speed 115200)
#define SerialMon Serial
// Set serial for AT commands (to the module)
#define SerialAT  Serial1

// Configure TinyGSM library
#define TINY_GSM_MODEM_SIM800          // Modem is SIM800
#define TINY_GSM_RX_BUFFER      1024   // Set RX buffer to 1Kb

#include <TinyGsmClient.h>

#ifdef DUMP_AT_COMMANDS
#include <StreamDebugger.h>
StreamDebugger debugger(SerialAT, SerialMon);
TinyGsm modem(debugger);
#else
TinyGsm modem(SerialAT);
#endif

// Set phone numbers, if you want to test SMS and Calls
#define SMS_TARGET  "+380xxxxxxxxx"
#define CALL_TARGET "+380xxxxxxxxx"


void setup()
{
    // Set console baud rate
    SerialMon.begin(115200);

    delay(10);

    // Start power management
    if (setupPMU() == false) {
        Serial.println("Setting power error");
    }

    // Some start operations
    setupModem();

    // Set GSM module baud rate and UART pins
    SerialAT.begin(115200, SERIAL_8N1, MODEM_RX, MODEM_TX);

    delay(6000);
}

void loop()
{
    // Restart takes quite some time
    // To skip it, call init() instead of restart()
    SerialMon.println("Initializing modem...");
    modem.restart();

    // Swap the audio channels
    SerialAT.print("AT+CHFA=1\r\n");
    delay(2);

    //Set ringer sound level
    SerialAT.print("AT+CRSL=100\r\n");
    delay(2);

    //Set loud speaker volume level
    SerialAT.print("AT+CLVL=100\r\n");
    delay(2);

    // Calling line identification presentation
    SerialAT.print("AT+CLIP=1\r\n");
    delay(10000);

    Serial.print("Calling:");
    Serial.println(CALL_TARGET);

    bool res = modem.callNumber(CALL_TARGET);

    Serial.print("Call:");
    Serial.println(res ? "OK" : "fail");

    // Hang up after 20 seconds
    delay(20000);

    res = modem.callHangup();
    DBG("Hang up:", res ? "OK" : "fail");

    // Do nothing forevermore
    while (true) {
        modem.maintain();
    }
}
