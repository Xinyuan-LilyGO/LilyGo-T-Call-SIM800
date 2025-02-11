// (c) Copyright 2010-2012 MCQN Ltd.
// Released under Apache License, version 2.0
//
// Simple example to show how to use the HttpClient library
// Gets the web page given at http://<kHostname><kPath> and
// outputs the content to the serial port

#include <SPI.h>
#include <WiFi101.h>
#include <ArduinoHttpClient.h>

// This example downloads the URL "http://arduino.cc/"

#include "arduino_secrets.h"

///////please enter your sensitive data in the Secret tab/arduino_secrets.h
/////// WiFi Settings ///////
char ssid[] = SECRET_SSID;
char pass[] = SECRET_PASS;



// Name of the server we want to connect to
const char kHostname[] = "arduino.cc";
// Path to download (this is the bit after the hostname in the URL
// that you want to download
const char kPath[] = "/";

// Number of milliseconds to wait without receiving any data before we give up
const int kNetworkTimeout = 30*1000;
// Number of milliseconds to wait if no data is available before trying again
const int kNetworkDelay = 1000;

WiFiClient c;
HttpClient http(c, kHostname);

void setup()
{
  //Initialize serial and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  // attempt to connect to WiFi network:
  Serial.print("Attempting to connect to WPA SSID: ");
  Serial.println(ssid);
  while (WiFi.begin(ssid, pass) != WL_CONNECTED) {
    // unsuccessful, retry in 4 seconds
    Serial.print("failed ... ");
    delay(4000);
    Serial.print("retrying ... ");
  }

  Serial.println("connected");
}

void loop()
{
  int err =0;
  
  err = http.get(kPath);
  if (err == 0)
  {
    Serial.println("startedRequest ok");

    err = http.responseStatusCode();
    if (err >= 0)
    {
      Serial.print("Got status code: ");
      Serial.println(err);

      // Usually you'd check that the response code is 200 or a
      // similar "success" code (200-299) before carrying on,
      // but we'll print out whatever response we get

      // If you are interesting in the response headers, you
      // can read them here:
      //while(http.headerAvailable())
      //{
      //  String headerName = http.readHeaderName();
      //  String headerValue = http.readHeaderValue();
      //}

      int bodyLen = http.contentLength();
      Serial.print("Content length is: ");
      Serial.println(bodyLen);
      Serial.println();
      Serial.println("Body returned follows:");
    
      // Now we've got to the body, so we can print it out
      unsigned long timeoutStart = millis();
      char c;
      // Whilst we haven't timed out & haven't reached the end of the body
      while ( (http.connected() || http.available()) &&
             (!http.endOfBodyReached()) &&
             ((millis() - timeoutStart) < kNetworkTimeout) )
      {
          if (http.available())
          {
              c = http.read();
              // Print out this character
              Serial.print(c);
             
              // We read something, reset the timeout counter
              timeoutStart = millis();
          }
          else
          {
              // We haven't got any data, so let's pause to allow some to
              // arrive
              delay(kNetworkDelay);
          }
      }
    }
    else
    {    
      Serial.print("Getting response failed: ");
      Serial.println(err);
    }
  }
  else
  {
    Serial.print("Connect failed: ");
    Serial.println(err);
  }
  http.stop();

  // And just stop, now that we've tried a download
  while(1);
}
