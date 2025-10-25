#include <Arduino.h>
#include <Wire.h>

#define uS_TO_S_FACTOR 1000000ULL  /* Conversion factor for micro seconds to seconds */
#define TIME_TO_SLEEP  5*60        /* Time ESP32 will go to sleep (in seconds) */

// Set serial for debug console (to the Serial Monitor, default speed 115200)
#define SerialMon Serial
// Set serial for AT commands (to the module)
#define SerialAT  Serial1

#define I2C_SDA              21
#define I2C_SCL              22
#define LED_GPIO             13
#define LED_ON               HIGH
#define LED_OFF              LOW
#define IP5306_ADDR          0x75
#define IP5306_REG_SYS_CTL0  0x00

// Set the power chip to remain on in low power state
bool setBoostKeepOn()
{
    bool en = true;
    Wire.begin(I2C_SDA, I2C_SCL);
    Wire.beginTransmission(IP5306_ADDR);
    Wire.write(IP5306_REG_SYS_CTL0);
    if (en) {
        Wire.write(0x37); // Set bit1: 1 enable 0 disable boost keep on
    } else {
        Wire.write(0x35); // 0x37 is default reg value
    }
    return Wire.endTransmission() == 0;
}

void setup()
{
    // Set console baud rate
    SerialMon.begin(115200);

    if (setBoostKeepOn()) {
        SerialMon.println("Set Boost Keep On successfully");
    } else {
        SerialMon.println("Set Boost Keep On failed");
    }

    pinMode(LED_GPIO, OUTPUT);
    digitalWrite(LED_GPIO, LED_ON); 
    delay(2000);
    digitalWrite(LED_GPIO, LED_OFF);

    // Goto sleep 300 seconds , wake up by timer
    SerialMon.println("Going to sleep now");
    esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);

    esp_deep_sleep_start();
}

void loop()
{
}
