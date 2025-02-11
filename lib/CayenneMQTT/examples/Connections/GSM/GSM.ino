/*
This example shows how to connect to Cayenne using a GSM device and send/receive sample data.

The CayenneMQTT Library is required to run this sketch. If you have not already done so you can install it from the Arduino IDE Library Manager.

Steps:
1. Install the TinyGSM library (https://github.com/vshymanskyy/TinyGSM) from the Arduino Library Manager.
2. Set the Cayenne authentication info to match the authentication info from the Dashboard.
3. Uncomment the correct GSM modem type and set the GSM connection info, if needed.
4. Compile and upload the sketch.
5. A temporary widget will be automatically generated in the Cayenne Dashboard. To make the widget permanent click the plus sign on the widget.
*/

//#define CAYENNE_DEBUG       // Uncomment to show debug messages
#define CAYENNE_PRINT Serial  // Comment this out to disable prints and save space

// Uncomment your modem type:
#define TINY_GSM_MODEM_SIM800
// #define TINY_GSM_MODEM_SIM808
// #define TINY_GSM_MODEM_SIM900
// #define TINY_GSM_MODEM_UBLOX
// #define TINY_GSM_MODEM_BG96
// #define TINY_GSM_MODEM_A6
// #define TINY_GSM_MODEM_A7
// #define TINY_GSM_MODEM_M590
// #define TINY_GSM_MODEM_ESP8266
// #define TINY_GSM_MODEM_XBEE

#include <CayenneMQTTGSM.h>

// This sketch uses a software serial connection.
#include <SoftwareSerial.h>
SoftwareSerial gsmSerial(2, 3); // RX, TX
// If you are using a device that supports a hardware serial (Mega, Leonardo, etc.) and prefer to use
// that you can comment out the above lines and uncomment the one below.
//#define gsmSerial Serial1

// GSM connection info.
char apn[] = ""; // Access point name. Leave empty if it is not needed.
char gprsLogin[] = ""; // GPRS username. Leave empty if it is not needed.
char gprsPassword[] = ""; // GPRS password. Leave empty if it is not needed.
char pin[] = ""; // SIM pin number. Leave empty if it is not needed.

// Cayenne authentication info. This should be obtained from the Cayenne Dashboard.
char username[] = "MQTT_USERNAME";
char password[] = "MQTT_PASSWORD";
char clientID[] = "CLIENT_ID";

void setup() {
	Serial.begin(9600);
	// Auto-detect the GSM serial baud rate. You can manually set it instead if you want to save a bit of space.
	TinyGsmAutoBaud(gsmSerial);
	Cayenne.begin(username, password, clientID, gsmSerial, apn, gprsLogin, gprsPassword, pin);
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
