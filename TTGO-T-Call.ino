#include "WiFi.h"
#include <Wire.h>


#define SIM800_TX 26
#define SIM800_RX 27
#define SIM800_PWKEY 4
#define SIM800_RST 5
#define SIM800_BAUDRATE 9600
#define I2C_SDA 21
#define I2C_SCL 22
#define SIM800_POWER_ON 23


#define IP5306_ADDR 0X75
#define IP5306_REG_SYS_CTL0 0x00
#define TEST_DELAY 0

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

int waitForResp(const char *resp, unsigned int timeout)
{
    int len = strlen(resp);
    int sum = 0;
    unsigned long timerStart, timerEnd;
    timerStart = millis();

    while (1) {
        if (Serial1.available()) {
            char c = Serial1.read();
            sum = (c == resp[sum]) ? sum + 1 : 0;
            if (sum == len)break;
        }
        timerEnd = millis();
        if (timerEnd - timerStart > 1000 * timeout) {
            return -1;
        }
    }

    while (Serial1.available()) {
        Serial1.read();
    }

    return 0;
}

void sim800_test()
{
    Serial1.begin(115200, SERIAL_8N1, SIM800_TX, SIM800_RX);

    pinMode(SIM800_POWER_ON, OUTPUT);
    digitalWrite(SIM800_POWER_ON, HIGH);
    Serial.printf("SIM800 Power On Result: [PIN:%d] - [Level:%d]\n", SIM800_POWER_ON, digitalRead(SIM800_POWER_ON));

    delay(500);

    Serial.println("Open SIM800 ...");
    pinMode(SIM800_PWKEY, OUTPUT);
    digitalWrite(SIM800_PWKEY, LOW);
    delay(1000);
    digitalWrite(SIM800_PWKEY, HIGH);

    Serial.println("SIM800 Reset");
    pinMode(SIM800_RST, OUTPUT);
    digitalWrite(SIM800_RST, HIGH);
    delay(1);
    digitalWrite(SIM800_RST, LOW);
    delay(1);
    digitalWrite(SIM800_RST, HIGH);

    Serial.println("Send AT Command Test ...");
    while (1) {
        Serial1.print("AT\r\n");
        if (waitForResp("OK", 2) == 0) {
            Serial.println("SIM800 ACK OK");
            break;
        } else {
            Serial.println("SIM800 No ACK,check wire");
        }
    }
    Serial.println("SIM800 Test PASS!");
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
    delay(TEST_DELAY);
}

void setup()
{
    Serial.begin(115200);
    while (!Serial);
    
    pinMode(13, OUTPUT);
    digitalWrite(13, HIGH);

    Wire.begin(I2C_SDA, I2C_SCL);
    bool   isOk = setPowerBoostKeepOn(1);
    String info = "IP5306 KeepOn " + String((isOk ? "PASS" : "FAIL"));
    Serial.println(info);
    wifi_test();
    sim800_test();
    Serial.println("Self test Done");
}

void loop()
{
    delay(TEST_DELAY);
}