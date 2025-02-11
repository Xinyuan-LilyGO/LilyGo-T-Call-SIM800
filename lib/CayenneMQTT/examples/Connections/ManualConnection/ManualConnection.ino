/*
This example shows how to connect to Cayenne using a manually specified Ethernet connection and send/receive sample data.

The CayenneMQTT Library is required to run this sketch. If you have not already done so you can install it from the Arduino IDE Library Manager.

Steps:
1. Set the Cayenne authentication info to match the authentication info from the Dashboard.
2. Compile and upload the sketch.
3. A temporary widget will be automatically generated in the Cayenne Dashboard. To make the widget permanent click the plus sign on the widget.
*/

//#define CAYENNE_DEBUG       // Uncomment to show debug messages
#define CAYENNE_PRINT Serial  // Comment this out to disable prints and save space
#include <CayenneMQTTEthernet.h>
//#include <CayenneMQTTEthernetW5500.h> // Uncomment this and comment out CayenneMQTTEthernet.h to use an Ethernet 2 shield or other Ethernet W5500 shield.
                                        // You will need the Ethernet2 library installed. See the ArduinoEthernetW5500 example sketch for more info.
//#include <CayenneMQTTEthernetW5200.h> // Uncomment this and comment out CayenneMQTTEthernet.h to use an Ethernet W5200 shield.
                                        // You will need the EthernetW5200 library installed. See the ArduinoEthernetW5200 example sketch for more info.

// Cayenne authentication info. This should be obtained from the Cayenne Dashboard.
char username[] = "MQTT_USERNAME";
char password[] = "MQTT_PASSWORD";
char clientID[] = "CLIENT_ID";

// Mac address should be different for each device in your LAN
byte arduino_mac[] = { 0xDE, 0xED, 0xBA, 0xFE, 0xFE, 0xED };
IPAddress arduino_ip(10, 0, 0, 20);
IPAddress dns_ip(8, 8, 8, 8);
IPAddress gateway_ip(10, 0, 0, 1);
IPAddress subnet_mask(255, 255, 255, 0);

void setup() {
	Serial.begin(9600);
	Cayenne.begin(username, password, clientID, arduino_ip, dns_ip, gateway_ip, subnet_mask, arduino_mac);
}

void loop() {
	Cayenne.loop();
}

// Default function for sending sensor data at intervals to Cayenne.
// You can also use functions for specific channels, e.g CAYENNE_OUT(1) for sending channel 1 data.
CAYENNE_OUT_DEFAULT()
{
	// Write data to Cayenne here. This example just sends the current uptime in milliseconds on virtual channel 0.
	Cayenne.virtualWrite(0, millis());
	// Some examples of other functions you can use to send data.
	//Cayenne.celsiusWrite(1, 22.0);
	//Cayenne.luxWrite(2, 700);
	//Cayenne.virtualWrite(3, 50, TYPE_PROXIMITY, UNIT_CENTIMETER);
}

// Default function for processing actuator commands from the Cayenne Dashboard.
// You can also use functions for specific channels, e.g CAYENNE_IN(1) for channel 1 commands.
CAYENNE_IN_DEFAULT()
{
	CAYENNE_LOG("Channel %u, value %s", request.channel, getValue.asString());
	//Process message here. If there is an error set an error message using getValue.setError(), e.g getValue.setError("Error message");
}