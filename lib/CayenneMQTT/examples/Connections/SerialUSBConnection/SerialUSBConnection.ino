/*
This example shows how to connect to Cayenne using a Serial USB connection and send/receive sample data.

The CayenneMQTT Library is required to run this sketch. If you have not already done so you can install it from the Arduino IDE Library Manager.

This requires the use of the Serial USB connection so you cannot use the Serial device for
printing messages. If you need to print you can use SoftwareSerial and connect another device
to read messages via the SoftwareSerial pins.

In order for this to work you must run the connection script on the machine the Arduino is connected to.
The scripts are located under the extras\scripts folder in the main library folder. This redirects the traffic
from the Arduino to the Cayenne server.

Steps:
1. Set the Cayenne authentication info to match the authentication info from the Dashboard.
2. Compile and upload this sketch.
3. Launch the connection script as described below for Windows or Linux/OSX.

Windows:
	1. Open the Windows command line (cmd.exe)
	2. Navigate to the scripts folder by typing "cd [path]", e.g.  "cd C:\Users\[YourUserName]\Documents\Arduino\libraries\CayenneMQTT\extras\scripts"
	3. Run the script by typing "cayenne-ser.bat -c COM4" (where COM4 is the Arduino serial port) and hitting Enter

Linux and OSX:
    ./cayenne-ser.sh (may need to run with sudo)
    
You can specify port, baud rate, and server endpoint like this:
    ./cayenne-ser.sh -c <serial port> -b <baud rate> -s <server address> -p <server port>

    For instance :
      ./cayenne-ser.sh -c /dev/ttyACM0 -b 9600 -s mqtt.mydevices.com -p 1883

    Run cayenne-ser.sh -h for more information

    Be sure to select the right serial port (there may be multiple).

ATTENTION!
	Do not use Serial to display any output in this sketch. It will interfere with the Serial
	USB connection. When uploading sketches the Arduino IDE may complain with "programmer is
	not responding" or "Access is denied." You will need to terminate the connection script
	before uploading new sketches since it blocks access to the Serial port. Also make sure 
	the Serial Monitor is disabled in the IDE since that can prevent the Arduino from 
	connecting to the Windows/Linux/OSX machine. If you use Visual Micro for Visual Studio make
	sure Automatic Debugging is disabled. Otherwise the Serial Monitor can interfere with the
	Serial connection.
*/

#include <CayenneMQTTSerial.h>

// Cayenne authentication info. This should be obtained from the Cayenne Dashboard.
char username[] = "MQTT_USERNAME";
char password[] = "MQTT_PASSWORD";
char clientID[] = "CLIENT_ID";

void setup()
{
	//Baud rate can be specified by calling Cayenne.begin(username, password, clientID, 9600);
	Cayenne.begin(username, password, clientID);
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
	//Process message here. If there is an error set an error message using getValue.setError(), e.g getValue.setError("Error message");
}