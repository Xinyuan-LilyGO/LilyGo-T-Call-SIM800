// Your GPRS credentials (leave empty, if missing)
const char apn[]      = ""; // Your APN
const char gprsUser[] = ""; // User
const char gprsPass[] = ""; // Password
const char simPIN[]   = ""; // SIM card PIN code, if any

// TTGO T-Call pin definitions
#define MODEM_RST            5
#define MODEM_PWKEY          4
#define MODEM_POWER_ON       23
#define MODEM_TX             27
#define MODEM_RX             26
#define MODEM_DTR            32
#define MODEM_RI             33


#define I2C_SDA              21
#define I2C_SCL              22
#define LED_GPIO             13

// Set serial for debug console (to the Serial Monitor, default speed 115200)
#define SerialMon Serial
// Set serial for AT commands (to the module)
#define SerialAT  Serial1

// Configure TinyGSM library
#define TINY_GSM_MODEM_SIM800      // Modem is SIM800
#define TINY_GSM_RX_BUFFER   1024  // Set RX buffer to 1Kb

// Define the serial console for debug prints, if needed
//#define TINY_GSM_DEBUG SerialMon
//#define DUMP_AT_COMMANDS

// #define TEST_RING_RI_PIN            //Note will cancel the phone call test


#include <Wire.h>
#include <TinyGsmClient.h>      //https://github.com/vshymanskyy/TinyGSM
#include <axp20x.h>             //https://github.com/lewisxhe/AXP202X_Library

#ifdef DUMP_AT_COMMANDS
#include <StreamDebugger.h>
StreamDebugger debugger(SerialAT, SerialMon);
TinyGsm modem(debugger);
#else
TinyGsm modem(SerialAT);
#endif

#define uS_TO_S_FACTOR 1000000ULL  /* Conversion factor for micro seconds to seconds */
#define TIME_TO_SLEEP  30        /* Time ESP32 will go to sleep (in seconds) */


// Server details
const char server[] = "vsh.pp.ua";
const char resource[] = "/TinyGSM/logo.txt";

AXP20X_Class axp;
TinyGsmClient client(modem);
const int  port = 80;


void setup()
{
    // Set console baud rate
    SerialMon.begin(115200);
    delay(10);

    // Keep power when running from battery
    Wire.begin(I2C_SDA, I2C_SCL);

    int ret = axp.begin(Wire, AXP192_SLAVE_ADDRESS);

    if (ret == AXP_FAIL) {
        Serial.println("AXP Power begin failed");
        while (1);
    }

    //! Turn off unused power
    axp.setPowerOutPut(AXP192_DCDC1, AXP202_OFF);
    axp.setPowerOutPut(AXP192_LDO2, AXP202_OFF);
    axp.setPowerOutPut(AXP192_LDO3, AXP202_OFF);
    axp.setPowerOutPut(AXP192_DCDC2, AXP202_OFF);
    axp.setPowerOutPut(AXP192_EXTEN, AXP202_OFF);

    //! Do not turn off DC3, it is powered by esp32
    // axp.setPowerOutPut(AXP192_DCDC3, AXP202_ON);

    //Set the charging indicator to turn off
    // axp.setChgLEDMode(AXP20X_LED_OFF);

    // Set the charging indicator to flash once per second
    axp.setChgLEDMode(AXP20X_LED_BLINK_1HZ);


    //! Use axp192 adc get voltage info
    axp.adc1Enable(AXP202_VBUS_VOL_ADC1 | AXP202_VBUS_CUR_ADC1 | AXP202_BATT_CUR_ADC1 | AXP202_BATT_VOL_ADC1, true);

    float vbus_v = axp.getVbusVoltage();
    float vbus_c = axp.getVbusCurrent();
    float batt_v = axp.getBattVoltage();
    // axp.getBattPercentage();   // axp192 is not support percentage
    Serial.printf("VBUS:%.2f mV %.2f mA ,BATTERY: %.2f\n", vbus_v, vbus_c, batt_v);


    // Set-up modem reset, enable, power pins
    pinMode(MODEM_PWKEY, OUTPUT);
    pinMode(MODEM_RST, OUTPUT);
    pinMode(MODEM_POWER_ON, OUTPUT);
    pinMode(LED_GPIO, OUTPUT);

    digitalWrite(LED_GPIO, LOW);
    digitalWrite(MODEM_PWKEY, LOW);
    digitalWrite(MODEM_RST, HIGH);
    digitalWrite(MODEM_POWER_ON, HIGH);

    // Set GSM module baud rate and UART pins
    SerialAT.begin(115200, SERIAL_8N1, MODEM_RX, MODEM_TX);
    delay(3000);

    // Restart takes quite some time
    // To skip it, call init() instead of restart()
    SerialMon.println("Initializing modem...");
    modem.restart();
    // Or, use modem.init() if you don't need the complete restart

    String modemInfo = modem.getModemInfo();
    SerialMon.print("Modem: ");
    SerialMon.println(modemInfo);

    // Unlock your SIM card with a PIN if needed
    if (strlen(simPIN) && modem.getSimStatus() != 3 ) {
        modem.simUnlock(simPIN);
    }
}

void loop()
{
    bool res;

    SerialMon.print("Waiting for network...");
    if (!modem.waitForNetwork(240000L)) {
        SerialMon.println(" fail");
        delay(10000);
        return;
    }
    SerialMon.println(" OK");

    // When the network connection is successful, turn on the indicator
    digitalWrite(LED_GPIO, HIGH);

    if (modem.isNetworkConnected()) {
        SerialMon.println("Network connected");
    }

    SerialMon.print(F("Connecting to APN: "));
    SerialMon.print(apn);
    if (!modem.gprsConnect(apn, gprsUser, gprsPass)) {
        SerialMon.println(" fail");
        delay(10000);
        return;
    }
    SerialMon.println(" OK");

    SerialMon.print("Connecting to ");
    SerialMon.print(server);
    if (!client.connect(server, port)) {
        SerialMon.println(" fail");
        delay(10000);
        return;
    }
    SerialMon.println(" OK");

    // Make a HTTP GET request:
    SerialMon.println("Performing HTTP GET request...");
    client.print(String("GET ") + resource + " HTTP/1.1\r\n");
    client.print(String("Host: ") + server + "\r\n");
    client.print("Connection: close\r\n\r\n");
    client.println();

    unsigned long timeout = millis();
    while (client.connected() && millis() - timeout < 10000L) {
        // Print available data
        while (client.available()) {
            char c = client.read();
            SerialMon.print(c);
            timeout = millis();
        }
    }
    SerialMon.println();

    client.stop();
    SerialMon.println(F("Server disconnected"));

    modem.gprsDisconnect();
    SerialMon.println(F("GPRS disconnected"));

    modem.sleepEnable();

    delay(100);

    // test modem response , res == 0 , modem is sleep
    res = modem.testAT();
    Serial.print("SIM800 Test AT result -> ");
    Serial.println(res);

    delay(1000);

    Serial.println("Use DTR Pin Wakeup");
    pinMode(MODEM_DTR, OUTPUT);
    //Set DTR Pin low , wakeup modem .
    digitalWrite(MODEM_DTR, LOW);


    // test modem response , res == 1 , modem is wakeup
    res = modem.testAT();
    Serial.print("SIM800 Test AT result -> ");
    Serial.println(res);


#ifdef TEST_RING_RI_PIN
    // Swap the audio channels
    SerialAT.print("AT+CHFA=1\r\n");
    delay(2);

    //Set ringer sound level
    SerialAT.print("AT+CRSL=100\r\n");
    delay(2);

    //Set loud speaker volume level
    SerialAT.print("AT+CLVL=100\r\n");
    delay(2);

    // Calling line identification presentation
    SerialAT.print("AT+CLIP=1\r\n");
    delay(2);

    //Set RI Pin input
    pinMode(MODEM_RI, INPUT);

    Serial.println("Wait for call in");
    //When is no calling ,RI pin is high level
    while (digitalRead(MODEM_RI)) {
        Serial.print('.');
        delay(500);
    }
    Serial.println("call in ");

    //Wait 10 seconds for the bell to ring
    delay(10000);

    //Accept call
    SerialAT.println("ATA");

    //Let the call go on for 10 seconds and then shut down
    delay(10000);
#endif

    SerialMon.println(F("Poweroff"));
    modem.poweroff();

    esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);
    // esp_sleep_enable_ext1_wakeup(GPIO_SEL_33, ESP_EXT1_WAKEUP_ANY_HIGH);

    esp_deep_sleep_start();

}
