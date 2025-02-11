/*
  Custom request header example for the ArduinoHttpClient
  library. This example sends a GET and a POST request with a custom header every 5 seconds.

  based on SimpleGet example by Tom Igoe
  header modifications by Todd Treece
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
  Serial.println("making GET request");
  client.beginRequest();
  client.get("/");
  client.sendHeader("X-CUSTOM-HEADER", "custom_value");
  client.endRequest();

  // read the status code and body of the response
  int statusCode = client.responseStatusCode();
  String response = client.responseBody();

  Serial.print("GET Status code: ");
  Serial.println(statusCode);
  Serial.print("GET Response: ");
  Serial.println(response);

  Serial.println("Wait five seconds");
  delay(5000);

  Serial.println("making POST request");
  String postData = "name=Alice&age=12";
  client.beginRequest();
  client.post("/");
  client.sendHeader(HTTP_HEADER_CONTENT_TYPE, "application/x-www-form-urlencoded");
  client.sendHeader(HTTP_HEADER_CONTENT_LENGTH, postData.length());
  client.sendHeader("X-CUSTOM-HEADER", "custom_value");
  client.endRequest();
  client.write((const byte*)postData.c_str(), postData.length());
  // note: the above line can also be achieved with the simpler line below:
  //client.print(postData);

  // read the status code and body of the response
  statusCode = client.responseStatusCode();
  response = client.responseBody();

  Serial.print("POST Status code: ");
  Serial.println(statusCode);
  Serial.print("POST Response: ");
  Serial.println(response);

  Serial.println("Wait five seconds");
  delay(5000);
}
