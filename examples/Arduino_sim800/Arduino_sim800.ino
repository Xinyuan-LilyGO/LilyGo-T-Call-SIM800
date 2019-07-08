#include <Arduino.h>
#include <Wire.h>
#include <sim800.h>
#include <WiFi.h>
#include <gprs.h>

#define T_CALL

#if defined(T_CALL)
#define UART_TX                     27
#define UART_RX                     26
#define SIMCARD_RST                 5
#define SIMCARD_PWKEY               4
#define SIM800_POWER_ON             23
#else
#define UART_TX                     33
#define UART_RX                     34
#define SIMCARD_RST                 14
#define SIMCARD_PWKEY               15
#define SIM800_POWER_ON             4
#endif

#define UART_BANUD_RATE             9600

#define I2C_SDA                     21
#define I2C_SCL                     22

#define IP5306_ADDR                 0X75
#define IP5306_REG_SYS_CTL0         0x00

HardwareSerial hwSerial(1);
GPRS gprs(hwSerial, SIMCARD_PWKEY, SIMCARD_RST, SIM800_POWER_ON);

bool setPowerBoostKeepOn(int en)
{
    Wire.beginTransmission(IP5306_ADDR);
    Wire.write(IP5306_REG_SYS_CTL0);
    if (en)
        Wire.write(0x37); // Set bit1: 1 enable 0 disable boost keep on
    else
        Wire.write(0x35); // 0x37 is default reg value
    return Wire.endTransmission() == 0;
}

void wifi_test()
{
    WiFi.mode(WIFI_STA);
    WiFi.disconnect();
    int n = WiFi.scanNetworks();
    if (n == 0) {
        Serial.println("no networks found");
        return;
    }
    Serial.println("Found " + String(n) + " networks");
}

void setup()
{
    Serial.begin(115200);

#if defined(T_CALL)
    Wire.begin(I2C_SDA, I2C_SCL);
    bool   isOk = setPowerBoostKeepOn(1);
    String info = "IP5306 KeepOn " + String((isOk ? "PASS" : "FAIL"));
    Serial.println(info);
#endif

    hwSerial.begin(UART_BANUD_RATE, SERIAL_8N1, UART_RX, UART_TX);
    Serial.println("Setup Complete!");

    wifi_test();

    if (gprs.preInit()) {
        Serial.println("SIM800 Begin PASS");
    } else {
        Serial.println("SIM800 Begin FAIL");
    }

    Serial.println("Test motor ...");
    int i = 3;
    while (i--) {
        hwSerial.print("AT+SPWM=0,1000,1\r\n");
        delay(2000);
        hwSerial.print("AT+SPWM=0,0,0\r\n");
    }

    delay(200);

    if (0 != gprs.init()) {
        Serial.println("Not checked out to SIM card...");
        while (1);
    }

    // Switch audio channels
    hwSerial.print("AT+CHFA=1\r\n");
    delay(2);
    hwSerial.print("AT+CRSL=100\r\n");
    delay(2);
    hwSerial.print("AT+CLVL=100\r\n");
    delay(2);
    hwSerial.print("AT+CLIP=1\r\n");
    delay(2);
    Serial.println("Init success");
}

bool isCallIn = false;
void loop()
{
    if (hwSerial.available()) {
        const char *s = hwSerial.readStringUntil('\n').c_str();
        if (strstr(s, "OK" ) != NULL) {
            Serial.println("SIM OK");
        } else if (strstr(s, "+CPIN: NOT READY") != NULL) {
            Serial.println("SIM +CPIN: NOT READY");
        } else if (strstr(s, "+CPIN: READY") != NULL) {
            Serial.println("SIM +CPIN: READY");
        } else if (strstr(s, "+CLIP:") != NULL) {
            Serial.printf("SIM %s\n", s);
        } else if (strstr(s, "RING") != NULL) {
            delay(200);
            hwSerial.write("ATA\r\n");
            Serial.println("SIM RING");
        } else if (strstr(s, "Call Ready") != NULL) {
            Serial.println("SIM Call Ready");
        } else if (strstr(s, "SMS Ready") != NULL) {
            Serial.println("SIM SMS Ready");
        } else if (strstr(s, "NO CARRIER") != NULL) {
            Serial.println("SIM NO CARRIER");
        } else if (strstr(s, "NO DIALTONE") != NULL) {
            Serial.println("SIM NO DIALTONE");
        } else if (strstr(s, "BUSY") != NULL) {
            Serial.println("SIM BUSY");
        } else {
            Serial.print(s);
        }
        Serial.println("==========");
    }

    if (Serial.available()) {
        String r = Serial.readString();
        hwSerial.write(r.c_str());
    }
}


