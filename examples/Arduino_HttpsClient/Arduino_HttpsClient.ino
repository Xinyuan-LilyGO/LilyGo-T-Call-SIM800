// Please select the corresponding model
// Select your modem:
#define TINY_GSM_MODEM_SIM800

// #define SIM800L_IP5306_VERSION_20190610
// #define SIM800L_AXP192_VERSION_20200327
 #define SIM800C_AXP192_VERSION_20200609
// #define SIM800L_IP5306_VERSION_20200811



#include <Arduino.h>
#include "utilities.h"

// Set serial for debug console (to the Serial Monitor, default speed 115200)
#define SerialMon Serial
// Set serial for AT commands (to the module)
#define SerialAT  Serial1

// See all AT commands, if wanted
 #define DUMP_AT_COMMANDS

// Define the serial console for debug prints, if needed
#define TINY_GSM_DEBUG SerialMon
// #define LOGGING  // <- Logging is for the HTTP library

// Range to attempt to autobaud
// NOTE:  DO NOT AUTOBAUD in production code.  Once you've established
// communication, set a fixed baud rate using modem.setBaud(#).
#define GSM_AUTOBAUD_MIN 9600
#define GSM_AUTOBAUD_MAX 115200

// Add a reception delay, if needed.
// This may be needed for a fast processor at a slow baud rate.
// #define TINY_GSM_YIELD() { delay(2); }

// Define how you're planning to connect to the internet.
// This is only needed for this example, not in other code.
#define TINY_GSM_USE_GPRS true
#define TINY_GSM_USE_WIFI false

// set GSM PIN, if any
#define GSM_PIN ""

// flag to force SSL client authentication, if needed
// #define TINY_GSM_SSL_CLIENT_AUTHENTICATION

// Your GPRS credentials, if any
const char apn[]      = "YourAPN";
const char gprsUser[] = "";
const char gprsPass[] = "";

// Your WiFi connection credentials, if applicable
const char wifiSSID[] = "YourSSID";
const char wifiPass[] = "YourWiFiPass";



#include <TinyGsmClient.h>
#include <ArduinoHttpClient.h>

// Just in case someone defined the wrong thing..
#if TINY_GSM_USE_GPRS && not defined TINY_GSM_MODEM_HAS_GPRS
#undef TINY_GSM_USE_GPRS
#undef TINY_GSM_USE_WIFI
#define TINY_GSM_USE_GPRS false
#define TINY_GSM_USE_WIFI true
#endif
#if TINY_GSM_USE_WIFI && not defined TINY_GSM_MODEM_HAS_WIFI
#undef TINY_GSM_USE_GPRS
#undef TINY_GSM_USE_WIFI
#define TINY_GSM_USE_GPRS true
#define TINY_GSM_USE_WIFI false
#endif

#ifdef DUMP_AT_COMMANDS
#include <StreamDebugger.h>
StreamDebugger debugger(SerialAT, SerialMon);
TinyGsm        modem(debugger);
#else
TinyGsm        modem(SerialAT);
#endif


void setupModem()
{
#ifdef MODEM_RST
    // Keep reset high
    pinMode(MODEM_RST, OUTPUT);
    digitalWrite(MODEM_RST, HIGH);
#endif

    pinMode(MODEM_PWRKEY, OUTPUT);
    pinMode(MODEM_POWER_ON, OUTPUT);

    // Turn on the Modem power first
    digitalWrite(MODEM_POWER_ON, HIGH);

    // Pull down PWRKEY for more than 1 second according to manual requirements
    digitalWrite(MODEM_PWRKEY, HIGH);
    delay(100);
    digitalWrite(MODEM_PWRKEY, LOW);
    delay(1000);
    digitalWrite(MODEM_PWRKEY, HIGH);

    // Initialize the indicator as an output
    pinMode(LED_GPIO, OUTPUT);
    digitalWrite(LED_GPIO, LED_OFF);
}
bool Bearing_set();
bool Https_get();
bool Close_serve();

void setup() {
  // Set console baud rate
  SerialMon.begin(115200);
  delay(10);

  // !!!!!!!!!!!
  // Set your reset, enable, power pins here
  // !!!!!!!!!!!

  SerialMon.println("Wait...");

  // Set GSM module baud rate
    // Set GSM module baud rate and UART pins
    SerialAT.begin(115200, SERIAL_8N1, MODEM_RX, MODEM_TX);

  setupModem();
  delay(6000);

  // Restart takes quite some time
  // To skip it, call init() instead of restart()
  SerialMon.println("Initializing modem...");
  modem.restart();
  // modem.init();

  String modemInfo = modem.getModemInfo();
  SerialMon.print("Modem Info: ");
  SerialMon.println(modemInfo);

#if TINY_GSM_USE_GPRS
  // Unlock your SIM card with a PIN if needed
  if (GSM_PIN && modem.getSimStatus() != 3) { modem.simUnlock(GSM_PIN); }
#endif
}

void loop() {
#if TINY_GSM_USE_WIFI
  // Wifi connection parameters must be set before waiting for the network
  SerialMon.print(F("Setting SSID/password..."));
  if (!modem.networkConnect(wifiSSID, wifiPass)) {
    SerialMon.println(" fail");
    delay(10000);
    return;
  }
  SerialMon.println(" success");
#endif

#if TINY_GSM_USE_GPRS && defined TINY_GSM_MODEM_XBEE
  // The XBee must run the gprsConnect function BEFORE waiting for network!
  modem.gprsConnect(apn, gprsUser, gprsPass);
#endif

  SerialMon.print("Waiting for network...");
  if (!modem.waitForNetwork()) {
    SerialMon.println(" fail");
    delay(10000);
    return;
  }
  SerialMon.println(" success");

  if (modem.isNetworkConnected()) { SerialMon.println("Network connected"); }

#if TINY_GSM_USE_GPRS
  // GPRS connection parameters are usually set after network registration
  SerialMon.print(F("Connecting to "));
  SerialMon.print(apn);
  if (!modem.gprsConnect(apn, gprsUser, gprsPass)) {
    SerialMon.println(" fail");
    delay(10000);
    return;
  }
  SerialMon.println(" success");

  if (modem.isGprsConnected()) { SerialMon.println("GPRS connected"); }
#endif

  SerialMon.print(F("Performing HTTPS GET request... "));

  if(Bearing_set()==false) SerialMon.println("Bearing set fall");

  if(Https_get()==false) {SerialMon.println("https get fall");}

  Close_serve();

#if TINY_GSM_USE_WIFI
  modem.networkDisconnect();
  SerialMon.println(F("WiFi disconnected"));
#endif
#if TINY_GSM_USE_GPRS
  modem.gprsDisconnect();
  SerialMon.println(F("GPRS disconnected"));
#endif

  // Do nothing forevermore
  while (true) { 
    delay(1000); 
    }
}

//Bearing set
bool Bearing_set(){

    modem.sendAT(GF("+HTTPTERM"));//Configuring Bearer Scenarios
    if(modem.waitResponse(10000L) != 1) {
       DBG(GF("+HTTPTERM"));
        //return false;
    }

    
    modem.sendAT(GF("+SAPBR=0,1"));//Configuring Bearer Scenarios
    if(modem.waitResponse(10000L) != 1) {
       DBG(GF("+SAPBR=0,1"));
        return false;
    }
   delay(1000);

    modem.sendAT(GF("+SAPBR=3,1,\"Contype\",\"GPRS\""));//Configuring Bearer Scenarios
    if(modem.waitResponse(10000L) != 1) {
       DBG(GF("+SAPBR=3,1,\"Contype\",\"GPRS\""));
        return false;
    }

    modem.sendAT(GF("+SAPBR=1,1"));//Activate a GPRS context
    if (modem.waitResponse(10000L) != 1) {
       DBG(GF("+SAPBR=1,1"));
        //return false;
    }

    modem.sendAT(GF("+SAPBR=2,1"));//Query the GPRS context
    if (modem.waitResponse(10000L) != 1) {
       DBG(GF("+SAPBR=2,1"));
        //return false;
    }
    delay(2000);
}

bool Https_get(){

    modem.sendAT(GF("+HTTPINIT"));// Initialize the HTTP service
    if (modem.waitResponse(10000L) != 1) {
      DBG(GF("+HTTPINIT"));
        return false;
    }
    modem.sendAT(GF("+HTTPPARA=\"CID\",1"));//Set HTTP session parameters
    if (modem.waitResponse(10000L) != 1) {
       DBG(GF("+HTTPPARA=\"CID\",1"));
        return false;
    }

    modem.sendAT(GF("+HTTPPARA=\"URL\",\"www.baidu.com\""));//Set HTTP session parameters
    if (modem.waitResponse(10000L) != 1) {
       DBG(GF("+HTTPPARA=\"URL\",\"www.baidu.com\""));
        return false;
    }

    modem.sendAT(GF("+HTTPPARA=\"REDIR\",1"));//Set HTTP session parameters
    if (modem.waitResponse(10000L) != 1) {
       DBG(GF("+HTTPPARA=\"REDIR\",1"));
        return false;
    }
    modem.sendAT(GF("+HTTPSSL=1"));//Enabling the HTTPS function
    if (modem.waitResponse(10000L) != 1) {
       DBG(GF("+HTTPSSL=1"));
        return false;
    }
    modem.sendAT(GF("+HTTPACTION=0"));//Get
    if (modem.waitResponse(60000L) != 1) {
       DBG(GF("+HTTPACTION=0"));
        return false;
    }
    delay(10000);

    modem.sendAT(GF("+HTTPREAD"));//Read data from the HTTP server
    if (modem.waitResponse(60000L) != 1) {
       DBG(GF("+HTTPREAD"));
        return false;
    }

}

bool Close_serve(){

    modem.sendAT(GF("+HTTPTERM"));//close https
    if (modem.waitResponse(10000L) != 1) {
       DBG(GF("+HTTPTERM"));
        return false;
    }

    modem.sendAT(GF("+SAPBR=0,1"));//close GPRS
    if (modem.waitResponse(10000L) != 1) {
       DBG(GF("+SAPBR=0,1"));
        return false;
    }
}
