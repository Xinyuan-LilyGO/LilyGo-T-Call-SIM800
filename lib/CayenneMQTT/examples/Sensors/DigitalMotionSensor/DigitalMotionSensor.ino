/*
Cayenne Digital Motion Sensor Example

This sketch shows how to send Digital Motion Sensor data to the Cayenne Dashboard.

The CayenneMQTT Library is required to run this sketch. If you have not already done so you can install it from the Arduino IDE Library Manager.

Steps:
1. Attach a Digital Motion Sensor to a digital pin on your Arduino.
   Schematic:
   [Ground] -- [Digital Motion Sensor] -- [5V]
                         |
                     Digital Pin
2. Set the SENSOR_PIN value below to the pin number you used when connecting the sensor.
3. Set the VIRTUAL_CHANNEL value below to a free virtual channel (or the virtual channel of a Digital Motion Sensor widget you have added) in the Dashboard.
4. Set the Cayenne authentication info to match the authentication info from the Dashboard.
5. Compile and upload this sketch.
6. Once the Arduino connects to the Dashboard it should automatically create a temporary display widget (or update the Digital Motion Sensor widget you have added) with data.
   To make a temporary widget permanent click the plus sign on the widget.
*/

#define CAYENNE_PRINT Serial  // Comment this out to disable prints and save space
#include <CayenneMQTTEthernet.h>

// Cayenne authentication info. This should be obtained from the Cayenne Dashboard.
char username[] = "MQTT_USERNAME";
char password[] = "MQTT_PASSWORD";
char clientID[] = "CLIENT_ID";

#define SENSOR_PIN 4 // Do not use digital pins 0 or 1 since those conflict with the use of Serial.
#define VIRTUAL_CHANNEL 1

void setup()
{
	pinMode(SENSOR_PIN, INPUT);
	Serial.begin(9600);
	Cayenne.begin(username, password, clientID);
}

void loop()
{
	Cayenne.loop();
	checkSensor();
}

int previousState = -1;
int currentState = -1;
unsigned long previousMillis = 0;

void checkSensor()
{
	unsigned long currentMillis = millis();
	// Check sensor data every 250 milliseconds
	if (currentMillis - previousMillis >= 250) {
		// Check the sensor state and send data when it changes.
		currentState = digitalRead(SENSOR_PIN);
		if (currentState != previousState) {
			Cayenne.virtualWrite(VIRTUAL_CHANNEL, currentState, "digital_sensor", "d");
			previousState = currentState;
		}
		previousMillis = currentMillis;
	}
}