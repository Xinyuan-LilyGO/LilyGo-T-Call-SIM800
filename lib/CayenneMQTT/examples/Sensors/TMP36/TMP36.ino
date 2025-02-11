/*
Cayenne TMP36 Example

This sketch shows how to send TMP36 Sensor data to the Cayenne Dashboard.

The CayenneMQTT Library is required to run this sketch. If you have not already done so you can install it from the Arduino IDE Library Manager.

Steps:
1. Attach a TMP36 to an analog pin on your Arduino. Make sure to use an analog pin, not a digital pin.
   Schematic:
   [Ground] -- [TMP36] -- [5V]
                  |
              Analog Pin
2. Set the VIRTUAL_CHANNEL value below to a free virtual channel (or the virtual channel of the TMP36 Sensor widget you have added) in the Dashboard.
3. Set the tmpPin variable to match the pin used to connect the TMP36.
4. Set the voltage variable to match the voltage used to connect the TMP36.
5. Set the Cayenne authentication info to match the authentication info from the Dashboard.
6. Compile and upload this sketch.
7. Once the Arduino connects to the Dashboard it should automatically create temporary display widgets (or update the TMP36 Sensor widgets you have added) with data.
   To make a temporary widget permanent click the plus sign on the widget.
*/

#define CAYENNE_PRINT Serial   // Comment this out to disable prints and save space
#include <CayenneTemperature.h>
#include <CayenneMQTTEthernet.h>   // Change this to use a different communication device. See Communications examples.

// Cayenne authentication info. This should be obtained from the Cayenne Dashboard.
char username[] = "MQTT_USERNAME";
char password[] = "MQTT_PASSWORD";
char clientID[] = "CLIENT_ID";

#define VIRTUAL_CHANNEL 1

// Analog pin the TMP36 is connected to.
const int tmpPin = 0;

// Voltage to the TMP36. For 3v3 Arduinos use 3.3.
const float voltage = 5.0; 

TMP36 tmpSensor(tmpPin, voltage);

void setup()
{
	Serial.begin(9600);
	Cayenne.begin(username, password, clientID);
}

void loop()
{
	Cayenne.loop();
}

// This function is called at intervals to send sensor data to Cayenne.
CAYENNE_OUT(VIRTUAL_CHANNEL)
{
	// This command writes the temperature in Celsius to the Virtual Channel.
	Cayenne.celsiusWrite(VIRTUAL_CHANNEL, tmpSensor.getCelsius());
	// To send the temperature in Fahrenheit or Kelvin use the corresponding code below.
	//Cayenne.fahrenheitWrite(VIRTUAL_CHANNEL, tmpSensor.getFahrenheit());
	//Cayenne.kelvinWrite(VIRTUAL_CHANNEL, tmpSensor.getKelvin());
}
