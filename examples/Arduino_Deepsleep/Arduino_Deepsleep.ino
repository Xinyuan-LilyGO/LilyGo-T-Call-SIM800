#include <Arduino.h>
#include <Wire.h>
#include <sim800.h>
#include <gprs.h>

#define UART_TX                     27
#define UART_RX                     26
#define SIMCARD_RST                 5
#define SIMCARD_PWKEY               4
#define SIM800_POWER_ON             23

#define UART_BANUD_RATE             9600

#define I2C_SDA                     21
#define I2C_SCL                     22

#define IP5306_ADDR                 0X75
#define IP5306_REG_SYS_CTL0         0x00

HardwareSerial hwSerial(1);
SIM800 gprs(hwSerial, SIMCARD_PWKEY, SIMCARD_RST, SIM800_POWER_ON);

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


void setup()
{
    Serial.begin(115200);

    Wire.begin(I2C_SDA, I2C_SCL);
    bool   isOk = setPowerBoostKeepOn(1);
    String info = "IP5306 KeepOn " + String((isOk ? "PASS" : "FAIL"));
    Serial.println(info);


    hwSerial.begin(UART_BANUD_RATE, SERIAL_8N1, UART_RX, UART_TX);

    if (gprs.preInit()) {
        Serial.println("SIM800 Begin PASS");
    } else {
        Serial.println("SIM800 Begin FAIL");
    }
    Serial.println("enter deepsleep now");

    delay(3000);

    esp_sleep_enable_ext0_wakeup(GPIO_NUM_33, HIGH); 

    esp_deep_sleep_start();
}

void loop()
{
}


