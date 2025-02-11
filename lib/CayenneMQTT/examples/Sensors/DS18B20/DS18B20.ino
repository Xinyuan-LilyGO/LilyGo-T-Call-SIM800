/*
Cayenne DS18B20 Example

This sketch shows how to send DS18B20 Sensor data to the Cayenne Dashboard.

The CayenneMQTT Library is required to run this sketch. If you have not already done so you can install it from the Arduino IDE Library Manager.

Steps:
1. Install the OneWire library (http://www.pjrc.com/teensy/td_libs_OneWire.html) from the Arduino Library Manager.
2. Install the DallasTemperature library (http://milesburton.com/Main_Page?title=Dallas_Temperature_Control_Library#Code.2FLibrary) from the Arduino Library Manager.
3. Attach a DS18B20 to a digital pin on your Arduino.
   Schematic:
   [Ground] -- [DS18B20] -- [4.7k resistor] -- [5V]
                   |______________|
                   |
              Digital Pin
4. Set the SENSOR_PIN value below to the pin number you used when connecting the sensor.
5. Set the VIRTUAL_CHANNEL value below to a free virtual channel (or the virtual channel of a DS18B20 Sensor widget you have added) in the Dashboard.
6. Set the Cayenne authentication info to match the authentication info from the Dashboard.
7. Compile and upload this sketch.
8. Once the Arduino connects to the Dashboard it should automatically create a temporary display widget (or update the DS18B20 Sensor widget you have added) with data.
   To make a temporary widget permanent click the plus sign on the widget.
*/

#define CAYENNE_PRINT Serial  // Comment this out to disable prints and save space
#include <OneWire.h>
#include <DallasTemperature.h>
#include <CayenneMQTTEthernet.h>

// Cayenne authentication info. This should be obtained from the Cayenne Dashboard.
char username[] = "MQTT_USERNAME";
char password[] = "MQTT_PASSWORD";
char clientID[] = "CLIENT_ID";

#define SENSOR_PIN 2 // Do not use digital pins 0 or 1 since those conflict with the use of Serial.
#define VIRTUAL_CHANNEL 1

OneWire oneWire(SENSOR_PIN);
DallasTemperature sensors(&oneWire);

void setup()
{
	Serial.begin(9600);
	Cayenne.begin(username, password, clientID);
	sensors.begin();
}

void loop()
{
	Cayenne.loop();
}

// This function is called at intervals to send sensor data to Cayenne.
CAYENNE_OUT(VIRTUAL_CHANNEL)
{
	// Send the command to get temperatures.
	sensors.requestTemperatures();
	// This command writes the temperature in Celsius to the Virtual Channel.
	Cayenne.celsiusWrite(VIRTUAL_CHANNEL, sensors.getTempCByIndex(0));
	// To send the temperature in Fahrenheit use the corresponding code below.
	//Cayenne.fahrenheitWrite(VIRTUAL_CHANNEL, sensors.getTempFByIndex(0));
}
