/*
This example sketch shows how a value can be sent from the Arduino to the Cayenne Dashboard at specified intervals.

The CayenneMQTT Library is required to run this sketch. If you have not already done so you can install it from the Arduino IDE Library Manager.

Steps:
1. Set the Cayenne authentication info to match the authentication info from the Dashboard.
2. Compile and upload the sketch.
3. A temporary widget will be automatically generated in the Cayenne Dashboard. To make the widget permanent click the plus sign on the widget.

NOTE:
 For this example you'll need SimpleTimer library:
   https://github.com/jfturcot/SimpleTimer
 Visit this page for more information:
   http://playground.arduino.cc/Code/SimpleTimer
*/

#define CAYENNE_PRINT Serial     // Comment this out to disable prints and save space
#include <CayenneMQTTEthernet.h> // Change this to use a different communication device. See Communications examples.
#include <SimpleTimer.h>

// Cayenne authentication info. This should be obtained from the Cayenne Dashboard.
char username[] = "MQTT_USERNAME";
char password[] = "MQTT_PASSWORD";
char clientID[] = "CLIENT_ID";

SimpleTimer timer;

// This function sends the device's uptime to Cayenne on Virtual Channel 5.
void sendUptime()
{
	// Send values using the virtualWrite function. Cayenne currently accepts int and float values.
	// Please don't send more that 10 values per second.
	Cayenne.virtualWrite(5, millis() / 1000);
}

void setup()
{
	Serial.begin(9600); // See the connection status in Serial Monitor
	Cayenne.begin(username, password, clientID);

	// Set up a function to be called every 5 seconds
	timer.setInterval(5000L, sendUptime);
}

void loop()
{
	Cayenne.loop(); // Runs main loop
	timer.run(); // Initiates SimpleTimer
}

