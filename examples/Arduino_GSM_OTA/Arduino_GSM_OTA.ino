/*
 * Copyright Volodymyr Shymanskyy 2018
 */

// Your GPRS credentials (leave empty, if missing)
const char apn[]      = ""; // Your APN
const char gprsUser[] = ""; // User
const char gprsPass[] = ""; // Password
const char simPIN[]   = ""; // SIM card PIN code, if any

// URL to download the firmware from
String overTheAirURL = "http://vsh.pp.ua/ota/ttgo-t-call-B.bin";

// TTGO T-Call pin definitions
#define MODEM_RST            5
#define MODEM_PWKEY          4
#define MODEM_POWER_ON       23
#define MODEM_TX             27
#define MODEM_RX             26
#define I2C_SDA              21
#define I2C_SCL              22

// Set serial for debug console (to the Serial Monitor, default speed 115200)
#define SerialMon Serial
// Set serial for AT commands (to the module)
#define SerialAT  Serial1

// Configure TinyGSM library
#define TINY_GSM_MODEM_SIM800      // Modem is SIM800
#define TINY_GSM_RX_BUFFER   1024  // Set RX buffer to 1Kb

// Define the serial console for debug prints, if needed
//#define TINY_GSM_DEBUG SerialMon
//#define DUMP_AT_COMMANDS

#include <Wire.h>
#include <TinyGsmClient.h>
#include <Update.h>
#include "utilities.h"

#ifdef DUMP_AT_COMMANDS
  #include <StreamDebugger.h>
  StreamDebugger debugger(SerialAT, SerialMon);
  TinyGsm modem(debugger);
#else
  TinyGsm modem(SerialAT);
#endif

#define DEBUG_PRINT(...) { SerialMon.print(millis()); SerialMon.print(" - "); SerialMon.println(__VA_ARGS__); }
#define DEBUG_FATAL(...) { SerialMon.print(millis()); SerialMon.print(" - FATAL: "); SerialMon.println(__VA_ARGS__); delay(10000); ESP.restart(); }

void startOtaUpdate(const String& ota_url)
{
  String protocol, host, url;
  int port;

  if (!parseURL(ota_url, protocol, host, port, url)) {
    DEBUG_FATAL(F("Cannot parse URL"));
  }

  DEBUG_PRINT(String("Connecting to ") + host + ":" + port);

  Client* client = NULL;
  if (protocol == "http") {
    client = new TinyGsmClient(modem);
    if (!client->connect(host.c_str(), port)) {
      DEBUG_FATAL(F("Client not connected"));
    }
  } else if (protocol == "https") {
    client = new TinyGsmClientSecure(modem);
    if (!client->connect(host.c_str(), port)) {
      DEBUG_FATAL(F("Client not connected"));
    }
  } else {
    DEBUG_FATAL(String("Unsupported protocol: ") + protocol);
  }
  
  DEBUG_PRINT(String("Requesting ") + url);

  client->print(String("GET ") + url + " HTTP/1.0\r\n"
               + "Host: " + host + "\r\n"
               + "Connection: keep-alive\r\n"
               + "\r\n");

  long timeout = millis();
  while (client->connected() && !client->available()) {
    if (millis() - timeout > 10000L) {
      DEBUG_FATAL("Response timeout");
    }
  }

  // Collect headers
  String md5;
  int contentLength = 0;

  while (client->available()) {
    String line = client->readStringUntil('\n');
    line.trim();
    //SerialMon.println(line);    // Uncomment this to show response headers
    line.toLowerCase();
    if (line.startsWith("content-length:")) {
      contentLength = line.substring(line.lastIndexOf(':') + 1).toInt();
    } else if (line.startsWith("x-md5:")) {
      md5 = line.substring(line.lastIndexOf(':') + 1);
    } else if (line.length() == 0) {
      break;
    }
  }

  if (contentLength <= 0) {
    DEBUG_FATAL("Content-Length not defined");
  }

  bool canBegin = Update.begin(contentLength);
  if (!canBegin) {
    Update.printError(SerialMon);
    DEBUG_FATAL("OTA begin failed");
  }

  if (md5.length()) {
    DEBUG_PRINT(String("Expected MD5: ") + md5);
    if(!Update.setMD5(md5.c_str())) {
      DEBUG_FATAL("Cannot set MD5");
    }
  }

  DEBUG_PRINT("Flashing...");

  // The next loop does approx. the same thing as Update.writeStream(http) or Update.write(http)

  int written = 0;
  int progress = 0;
  uint8_t buff[256];
  while (client->connected() && written < contentLength) {
    timeout = millis();
    while (client->connected() && !client->available()) {
      delay(1);
      if (millis() - timeout > 10000L) {
        DEBUG_FATAL("Timeout");
      }
    }

    int len = client->read(buff, sizeof(buff));
    if (len <= 0) continue;

    Update.write(buff, len);
    written += len;

    int newProgress = (written*100)/contentLength;
    if (newProgress - progress >= 5 || newProgress == 100) {
      progress = newProgress;
      SerialMon.print(String("\r ") + progress + "%");
    }
  }
  SerialMon.println();

  if (written != contentLength) {
    Update.printError(SerialMon);
    DEBUG_FATAL(String("Write failed. Written ") + written + " / " + contentLength + " bytes");
  }

  if (!Update.end()) {
    Update.printError(SerialMon);
    DEBUG_FATAL(F("Update not ended"));
  }

  if (!Update.isFinished()) {
    DEBUG_FATAL(F("Update not finished"));
  }

  DEBUG_PRINT("=== Update successfully completed. Rebooting.");
  ESP.restart();
}

void setup() {
  // Set console baud rate
  SerialMon.begin(115200);
  delay(10);

  //SerialMon.setDebugOutput(true);
  printDeviceInfo();

  // Keep power when running from battery
  Wire.begin(I2C_SDA, I2C_SCL);
  bool   isOk = setPowerBoostKeepOn(1);
  SerialMon.println(String("IP5306 KeepOn ") + (isOk ? "OK" : "FAIL"));

  SerialMon.println("  Firmware A is running");
  SerialMon.println("--------------------------");
  DEBUG_PRINT(F("Starting OTA update in 10 seconds..."));
  delay(10000);

  // Set-up modem reset, enable, power pins
  pinMode(MODEM_PWKEY, OUTPUT);
  pinMode(MODEM_RST, OUTPUT);
  pinMode(MODEM_POWER_ON, OUTPUT);

  digitalWrite(MODEM_PWKEY, LOW);
  digitalWrite(MODEM_RST, HIGH);
  digitalWrite(MODEM_POWER_ON, HIGH);

  // Set GSM module baud rate and UART pins
  SerialAT.begin(115200, SERIAL_8N1, MODEM_RX, MODEM_TX);
  delay(3000);

  // Restart takes quite some time
  // To skip it, call init() instead of restart()
  DEBUG_PRINT(F("Initializing modem..."));
  modem.restart();
  // Or, use modem.init() if you don't need the complete restart

  String modemInfo = modem.getModemInfo();
  DEBUG_PRINT(String("Modem: ") + modemInfo);

  // Unlock your SIM card with a PIN if needed
  if (strlen(simPIN) && modem.getSimStatus() != 3 ) {
    modem.simUnlock(simPIN);
  }

  DEBUG_PRINT(F("Waiting for network..."));
  if (!modem.waitForNetwork(240000L)) {
    DEBUG_FATAL(F("Network failed to connect"));
  }

  DEBUG_PRINT(F("Connecting to GPRS"));
  if (!modem.gprsConnect(apn, gprsUser, gprsPass)) {
    DEBUG_FATAL(F("APN failed to connect"));
  }

  startOtaUpdate(overTheAirURL);
}

void loop() {
  // We do nothing here
  delay(1000);
}
