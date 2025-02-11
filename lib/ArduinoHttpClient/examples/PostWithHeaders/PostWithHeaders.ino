/*
  POST with headers client for ArduinoHttpClient library
  Connects to server once every five seconds, sends a POST request
  with custom headers and a request body

  created 14 Feb 2016
  by Tom Igoe
  modified 18 Mar 2017
  by Sandeep Mistry
  modified 22 Jan 2019
  by Tom Igoe
  
  this example is in the public domain
 */
#include <ArduinoHttpClient.h>
#include <WiFi101.h>

#include "arduino_secrets.h"

///////please enter your sensitive data in the Secret tab/arduino_secrets.h
/////// WiFi Settings ///////
char ssid[] = SECRET_SSID;
char pass[] = SECRET_PASS;


char serverAddress[] = "192.168.0.3";  // server address
int port = 8080;

WiFiClient wifi;
HttpClient client = HttpClient(wifi, serverAddress, port);
int status = WL_IDLE_STATUS;

void setup() {
  Serial.begin(9600);
  while ( status != WL_CONNECTED) {
    Serial.print("Attempting to connect to Network named: ");
    Serial.println(ssid);                   // print the network name (SSID);

    // Connect to WPA/WPA2 network:
    status = WiFi.begin(ssid, pass);
  }

  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);
}

void loop() {
  Serial.println("making POST request");
  String postData = "name=Alice&age=12";

  client.beginRequest();
  client.post("/");
  client.sendHeader("Content-Type", "application/x-www-form-urlencoded");
  client.sendHeader("Content-Length", postData.length());
  client.sendHeader("X-Custom-Header", "custom-header-value");
  client.beginBody();
  client.print(postData);
  client.endRequest();

  // read the status code and body of the response
  int statusCode = client.responseStatusCode();
  String response = client.responseBody();

  Serial.print("Status code: ");
  Serial.println(statusCode);
  Serial.print("Response: ");
  Serial.println(response);

  Serial.println("Wait five seconds");
  delay(5000);
}
