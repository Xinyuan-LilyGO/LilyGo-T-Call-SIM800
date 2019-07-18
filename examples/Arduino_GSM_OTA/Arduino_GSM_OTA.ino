/*
 * Copyright Volodymyr Shymanskyy 2018
 */

// Your GPRS credentials (leave empty, if missing)
const char apn[]      = "www.ab.kyivstar.net"; // Your APN
const char gprsUser[] = "igprs"; // User
const char gprsPass[] = "internet"; // Password
const char simPIN[]   = ""; // SIM card PIN code, if any

String overTheAirURL = "https://vsh.pp.ua/ota/ttgo-t-call-1.bin";

#define TINY_GSM_MODEM_SIM800
#define TINY_GSM_RX_BUFFER   1024  // Set RX buffer to 1Kb
//#define DUMP_AT_COMMANDS

#include <TinyGsmClient.h>
#include <Update.h>
#include "utilities.h"

#define SerialMon Serial
#define SerialAT  Serial1

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
  SerialMon.begin(115200);
  delay(10);

  //SerialMon.setDebugOutput(true);
  printDeviceInfo();

  // Set-up modem reset, enable, power pins
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(23,OUTPUT);
  digitalWrite(4, LOW);
  digitalWrite(5, HIGH);
  digitalWrite(23, HIGH);

  // Set GSM module baud rate and UART pins
  SerialAT.begin(115200, SERIAL_8N1, 26, 27); // RX, TX
  delay(3000);

  // Restart takes quite some time
  // To skip it, call init() instead of restart()
  DEBUG_PRINT(F("Initializing modem..."));
  modem.restart();

  String modemInfo = modem.getModemInfo();
  DEBUG_PRINT(String("Modem: ") + modemInfo);

  // Unlock your SIM card with a PIN if needed
  if (strlen(simPIN) && modem.getSimStatus() != 3 ) {
    modem.simUnlock(simPIN);
  }

  DEBUG_PRINT(F("Waiting for network..."));
  if (!modem.waitForNetwork()) {
    DEBUG_FATAL(F("Network failed to connect"));
  }

  DEBUG_PRINT(F("Connecting to GPRS"));
  if (!modem.gprsConnect(apn, gprsUser, gprsPass)) {
    DEBUG_FATAL(F("APN failed to connect"));
  }

  startOtaUpdate(overTheAirURL);
}

void loop() {
  // put your main code here, to run repeatedly:

}
