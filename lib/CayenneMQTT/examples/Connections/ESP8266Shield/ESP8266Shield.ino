/*
This sketch connects to the Cayenne server using an ESP8266 WiFi module as a shield connected via a hardware serial to an Arduino.

The CayenneMQTT Library is required to run this sketch. If you have not already done so you can install it from the Arduino IDE Library Manager.

Steps:
1. Install the ESP8266SerialLibrary.zip library via the Arduino IDE (Sketch->Include Library->Add .ZIP Library) from the Cayenne extras/libraries
   folder (e.g. My Documents\Arduino\libraries\CayenneMQTT\extras\libraries) to compile this example.
2. Connect the ESP8266 as a shield to your Arduino. This example uses the Serial1 hardware serial pins available on the Mega. You can also try 
   using a software serial, though it may be less stable.
3. Set the Cayenne authentication info to match the authentication info from the Dashboard.
4. Set the network name and password.
5. Compile and upload the sketch.
6. A temporary widget will be automatically generated in the Cayenne Dashboard. To make the widget permanent click the plus sign on the widget.

NOTE: This code requires ESP8266 firmware version 1.0.0 (AT v0.22) or later.
*/

//#define CAYENNE_DEBUG       // Uncomment to show debug messages
#define CAYENNE_PRINT Serial  // Comment this out to disable prints and save space
#include <CayenneMQTTESP8266Shield.h>

// WiFi network info.
char ssid[] = "ssid";
char wifiPassword[] = "wifiPassword";

// Cayenne authentication info. This should be obtained from the Cayenne Dashboard.
char username[] = "MQTT_USERNAME";
char password[] = "MQTT_PASSWORD";
char clientID[] = "CLIENT_ID";

// Set ESP8266 Serial object. In this example we use the Serial1 hardware serial which is available on boards like the Arduino Mega.
#define EspSerial Serial1

ESP8266 wifi(&EspSerial);

void setup()
{
	Serial.begin(9600);
	delay(10);

	// Set ESP8266 baud rate
	EspSerial.begin(115200);
	delay(10);

	Cayenne.begin(username, password, clientID, wifi, ssid, wifiPassword);
}

void loop()
{
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