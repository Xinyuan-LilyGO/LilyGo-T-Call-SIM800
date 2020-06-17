/*

Enclosure conditions test via TCALL SIM800 & BME280

 */

#define TINY_GSM_MODEM_SIM800
#define TINY_GSM_RX_BUFFER 1024 // Set RX buffer to 1Kb
//#define DUMP_AT_COMMANDS

#include "TinyGsmClient.h"
#include "ThingsBoard.h"
#include "SparkFunBME280.h"
#include "utilities.h"
#include "driver/adc.h"
#include <esp_wifi.h>

#define uS_TO_S_FACTOR 1000000 /* Conversion factor for micro seconds to seconds */
#define TIME_TO_SLEEP 40       /* Time ESP32 will go to sleep (in seconds) */
RTC_DATA_ATTR int bootCount = 0;

#define PIN_TX 25
#define PIN_RX 26

HardwareSerial serialGsm(1);
#define SerialAT serialGsm
#define TINY_GSM_USE_GPRS true
#define TINY_GSM_USE_WIFI false

// Your GPRS credentials
// Leave empty, if missing user or pass
const char apn[] = "internet";
const char user[] = "";
const char pass[] = "";

// TTGO T-Call pin definitions
#define MODEM_RST 5       // SIM800 RESET but also IP5306 IRQ: use IRQ Analyzing signals IP5306 It is in working condition or in standby mode: IRQ = 1 Work, IRQ = 0 When in standby
#define MODEM_PWKEY 4     // PWRKEY SIM800
#define MODEM_POWER_ON 23 // EN SY8089 4v4 regulator for SIM800
#define MODEM_TX 27
#define MODEM_RX 26
#define I2C_SDA 21
#define I2C_SCL 22

#define ADC_BAT 35 // TCALL 35
int bat_mv = 0;

BME280 mySensor;
bool isEnvSensor = true;
void shutdown();
void getBatteryFromADC();

#ifdef DUMP_AT_COMMANDS
#include "StreamDebugger.h"
StreamDebugger debugger(serialGsm, Serial);
TinyGsm modem(debugger);
#else
TinyGsm modem(serialGsm);
#endif

// See https://thingsboard.io/docs/getting-started-guides/helloworld/
// to understand how to obtain an access token

#define TOKEN "xxxxxxxxxxxxxxxxxxx" // Thingsboard token
 
#define THINGSBOARD_SERVER "192.168.1.1" // Thingsboard server

// Baud rate for debug serial
#define SERIAL_DEBUG_BAUD 115200

// Initialize GSM client
TinyGsmClient client(modem);

// Initialize ThingsBoard instance
ThingsBoard tb(client);

// Set to true, if modem is connected
bool modemConnected = false;

void print_wakeup_reason()
{
  esp_sleep_wakeup_cause_t wakeup_reason;

  wakeup_reason = esp_sleep_get_wakeup_cause();

  switch (wakeup_reason)
  {
  case ESP_SLEEP_WAKEUP_EXT0:
    Serial.println("Wakeup caused by external signal using RTC_IO");
    break;
  case ESP_SLEEP_WAKEUP_EXT1:
    Serial.println("Wakeup caused by external signal using RTC_CNTL");
    break;
  case ESP_SLEEP_WAKEUP_TIMER:
    Serial.println("Wakeup caused by timer");
    break;
  case ESP_SLEEP_WAKEUP_TOUCHPAD:
    Serial.println("Wakeup caused by touchpad");
    break;
  case ESP_SLEEP_WAKEUP_ULP:
    Serial.println("Wakeup caused by ULP program");
    break;
  default:
    Serial.printf("Wakeup was not caused by deep sleep: %d\n", wakeup_reason);
    break;
  }
}

void GSM_ON(uint32_t time_delay)
{
  // Set-up modem reset, enable, power pins
  pinMode(MODEM_PWKEY, OUTPUT);
  pinMode(MODEM_POWER_ON, OUTPUT);
  pinMode(MODEM_RST, OUTPUT);

  Serial.println("MODEM_RST & IP5306 IRQ: HIGH"); // IP5306 HIGH
  digitalWrite(MODEM_RST, HIGH);
  delay(time_delay);

  Serial.println("MODEM_PWKEY: HIGH");
  digitalWrite(MODEM_PWKEY, HIGH); // turning modem OFF
  delay(time_delay);

  Serial.println("MODEM_POWER_ON: HIGH");
  digitalWrite(MODEM_POWER_ON, HIGH); //Enabling SY8089 4V4 for SIM800 (crashing when in battery)
  delay(time_delay);

  Serial.println("MODEM_PWKEY: LOW");
  digitalWrite(MODEM_PWKEY, LOW); // turning modem ON
  delay(time_delay);
}

void GSM_OFF()
{
  pinMode(MODEM_PWKEY, OUTPUT);
  pinMode(MODEM_POWER_ON, OUTPUT);
  pinMode(MODEM_RST, OUTPUT);

  digitalWrite(MODEM_PWKEY, HIGH);   // turn of modem in case its ON from previous state
  digitalWrite(MODEM_POWER_ON, LOW); // turn of modem psu in case its from previous state
  digitalWrite(MODEM_RST, HIGH);     // Keep IRQ high ? (or not to save power?)
}

void PowerManagment(uint32_t time_delay)
{

  Wire.begin(I2C_SDA, I2C_SCL);
  delay(100);
  bool isOk = setPowerBoostKeepOn(true); // Disables auto-standby, to keep 5V high during ESP32 sleep after 32Sec

  // Set console baud rate
  Serial.begin(SERIAL_DEBUG_BAUD);
  Serial.println(F("Started"));
  Serial.println(String("IP5306 setPowerBoostKeepOn: ") + (isOk ? "OK" : "FAIL"));
  Serial.println(String("IP5306 Battery level:") + getBatteryLevel());
  getBatteryFromADC();
}

void setup()
{

  Serial.begin(115200);
  //Increment boot number and print it every reboot
  ++bootCount;
  Serial.println("Boot number: " + String(bootCount));

  //Print the wakeup reason for ESP32
  print_wakeup_reason();

  /*
  First we configure the wake up source
  We set our ESP32 to wake up every 5 seconds
  */
  esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);
  Serial.println("Setup ESP32 to sleep for " + String(TIME_TO_SLEEP) +
                 " Seconds");

  PowerManagment(100);

  if (bootCount == 1)
  {
    Serial.println("Going to sleep now, will skip GSM this time to confirm wake after 32s");
    esp_deep_sleep_start();
  }
}

void loop()
{
  Serial.println("In loop");
  GSM_ON(1000);

  // Set GSM module baud rate and UART pins
  SerialAT.begin(115200, SERIAL_8N1, MODEM_RX, MODEM_TX);
  delay(3000);

  String modemInfo = modem.getModemInfo();
  Serial.print(F("Modem: "));
  Serial.println(modemInfo);

  isEnvSensor = true;
  mySensor.setI2CAddress(0x76); //purple modules are at 0x76, default 0x77
  if (mySensor.beginI2C() == false)
  { //Begin communication over I2C
    Serial.println("The sensor did not respond. Please check wiring.");
    isEnvSensor = false;
  }

  if (isEnvSensor)
  {
    Serial.print("Humidity: ");
    Serial.print(mySensor.readFloatHumidity(), 0);

    Serial.print(" Pressure: ");
    Serial.print(mySensor.readFloatPressure(), 0);

    Serial.print(" Alt: ");
    //Serial.print(mySensor.readFloatAltitudeMeters(), 1);
    Serial.print(mySensor.readFloatAltitudeFeet(), 1);

    Serial.print(" Temp: ");
    Serial.println(mySensor.readTempC(), 1);
  }
  Serial.print("GSM Battery: ");
  Serial.print(modem.getBattPercent());
  Serial.print("%   ");
  Serial.print(modem.getBattVoltage());
  Serial.println("mV");

  Serial.println(String("IP5306 Battery level:") + getBatteryLevel());
  getBatteryFromADC();

  delay(1000);

  if (!modemConnected)
  {
    Serial.print(F("Waiting for network..."));
    if (!modem.waitForNetwork(240000L))
    {
      Serial.println(" fail");
      shutdown();
    }
    Serial.println(" OK");

    Serial.print("Signal quality:");
    Serial.println(modem.getSignalQuality());

    Serial.print(F("Connecting to "));
    Serial.print(apn);
    if (!modem.gprsConnect(apn, user, pass))
    {
      Serial.println(" fail");
      shutdown();
    }

    modemConnected = true;
    Serial.println(" OK");
  }

  if (!tb.connected())
  {
    // Connect to the ThingsBoard
    Serial.print("Connecting to: ");
    Serial.print(THINGSBOARD_SERVER);
    Serial.print(" with token ");
    Serial.println(TOKEN);
    if (!tb.connect(THINGSBOARD_SERVER, TOKEN))
    {
      Serial.println("Failed to connect");
      shutdown();
    }
  }

  Serial.println("Sending data...");

  // Uploads new telemetry to ThingsBoard using MQTT.
  // See https://thingsboard.io/docs/reference/mqtt-api/#telemetry-upload-api
  // for more details

  if (tb.sendAttributeInt("bootCount", bootCount))
    Serial.println("send bootCount");

  if (isEnvSensor)
  {
    if (tb.sendTelemetryInt("d_temp", mySensor.readTempC()))
      Serial.println("send d_temp");

    if (tb.sendTelemetryInt("d_hum", (int)mySensor.readFloatHumidity()))
      Serial.println("d_hum");

    if (tb.sendTelemetryInt("d_pres", (int)mySensor.readFloatPressure()))
      Serial.println("d_press");
  }

  if (tb.sendTelemetryInt("d_bat_p", getBatteryLevel()))
    Serial.println("d_bat_p");

  if (tb.sendTelemetryInt("d_bat_mv", bat_mv))
    Serial.println("d_bat_mv");

  if (tb.sendTelemetryInt("d_gsm_rssi", modem.getSignalQuality())) //CSQ need to convert to RSSI
    Serial.println("d_gsm_rssi");

  delay(10000); //GSM post seems aynchronous?, need to give it some time

  shutdown();
}

void shutdown()
{
  modemConnected = false;
  Serial.println(F("GPRS disconnect"));
  modem.gprsDisconnect();

  Serial.println("Radio off");
  modem.radioOff();

  Serial.println("GSM power off");
  GSM_OFF();

  // adc_power_off(); //soposed to save power ?

  Serial.println("Going to sleep now");
  Serial.flush();
  esp_deep_sleep_start();
}

void getBatteryFromADC()
{
  bat_mv = 0;
  uint32_t oversample = 0;
  for (size_t i = 0; i < 100; i++)
  {
    oversample += (uint32_t)analogRead(ADC_BAT);
  }
  bat_mv = (int)oversample / 100;
  bat_mv = ((float)bat_mv / 4096) * 3600 * 2;

  Serial.print("Battery from ADC: ");
  Serial.print(bat_mv);
  Serial.println("mV");
}
