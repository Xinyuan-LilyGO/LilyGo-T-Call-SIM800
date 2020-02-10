// ================ Power IC IP5306 ===================

#define IP5306_ADDR 0x75
#define IP5306_REG_SYS_CTL0 0x00
#define IP5306_REG_SYS_CTL1 (0x01)
#define IP5306_REG_SYS_CTL2 (0x02)
#define IP5306_REG_READ0 (0x70)
#define IP5306_REG_READ1 (0x71)
#define IP5306_REG_READ3 (0x78)

//- REG_CTL0
#define BOOST_ENABLE_BIT (0x20)
#define CHARGE_OUT_BIT (0x10)
#define BOOT_ON_LOAD_BIT (0x04)
#define BOOST_OUT_BIT (0x02)
#define BOOST_BUTTON_EN_BIT (0x01)

//- REG_CTL1
#define BOOST_SET_BIT (0x80)
#define WLED_SET_BIT (0x40)
#define SHORT_BOOST_BIT (0x20)
#define VIN_ENABLE_BIT (0x04)

//- REG_CTL2
#define SHUTDOWNTIME_MASK (0x0c)
#define SHUTDOWNTIME_64S (0x0c)
#define SHUTDOWNTIME_32S (0x04)
#define SHUTDOWNTIME_16S (0x08)
#define SHUTDOWNTIME_8S (0x00)

//- REG_READ0
#define CHARGE_ENABLE_BIT (0x08)

//- REG_READ1
#define CHARGE_FULL_BIT (0x08)

//- REG_READ2
#define LIGHT_LOAD_BIT (0x20)
#define LOWPOWER_SHUTDOWN_BIT (0x01)

#define IP5306_SYS_CTRL0 0x00
#define IP5306_SYS_CTRL1 0x01
#define IP5306_SYS_CTRL2 0x02
#define IP5306_CHARGER_CTRL0 0x20
#define IP5306_CHARGER_CTRL1 0x21
#define IP5306_CHARGER_CTRL3 0x22
#define IP5306_CHARGER_CTRL4 0x23
#define IP5306_DIG_CTRL0 0x24
#define IP5306_CHARGER_STATUS0 0x70
#define IP5306_CHARGER_STATUS1 0x71
#define IP5306_CHARGER_STATUS2 0x72
#define IP5306_KEY_CTRL 0x77

//! Error Code
#define IP5306_PASS 0
#define IP5306_FAIL -1
#define IP5306_INVALID -2
#define IP5306_NOT_INIT -3

bool setPowerBoostKeepOn(bool en)
{
  Wire.beginTransmission(IP5306_ADDR);
  Wire.write(IP5306_REG_SYS_CTL0);
  if (en)
  {
    Wire.write(0b110111); 
    // 0x37 = 0b110111 TCALL example
       
  /*
 [1]      Boost EN (default 1)            [EXM note: if 0 ESP32 will not boot from battery]
 [1]      Charger EN (1)                  [EXM note: did  not observe difference]
 [1]      Reserved (1)                    [EXM note: did  not observe difference]
 [1]      Insert load auto power EN (1)   [EXM note: did  not observe difference]
 [1]      BOOST output normally open ( 1) [EXM note: if 0 will shutdown on ESP32 sleep after 32s]
 [1]      Key off EN: (0)                 [EXM note: could not detect difference]
  */
  
  }
  else
  {
    // Wire.write(0x35); // x35 => Autoshutdown - Will kill power of ESP32 while sleeping!
    //HEX 35 = 0b110101
  }
  return Wire.endTransmission() == 0;
}

bool setOtherPower1(bool en)
{
  if (en)
  {
    Wire.beginTransmission(IP5306_ADDR);
    Wire.write(IP5306_REG_SYS_CTL1);
    Wire.write(0x1D); // Set HEX:1D DEC:29 BIN:11101
   
   /*
[1]      Turn off boost control signal selection: short press twice
[1]      Switch WLED flashlight control signal selection: short press twice
[1]      Short press switch boost: disabled
[0]      Whether to turn on Boost after VIN is pulled out: opened
[1]      Batlow 3.0V Low Power Shutdown EN: enabled
   */
   
  
    return Wire.endTransmission() == 0;
  }
  else
    return false;
}

bool setOtherPower2(bool en)
{
  if (en)
  {
    Wire.beginTransmission(IP5306_ADDR);
    Wire.write(IP5306_REG_SYS_CTL2);
    Wire.write(0x64); // Set HEX:64 DEC:100 BIN:1100100
    return Wire.endTransmission() == 0;
  }
  else
    return false;
}

// bool setPowerBoostKeepOn(bool en){
// 	uint8_t data;
// 	Wire.beginTransmission(IP5306_ADDR);
// 	Wire.write(IP5306_REG_SYS_CTL0);
// 	Wire.endTransmission();

// 	if(Wire.requestFrom(IP5306_ADDR, 1))
// 	{
// 		data = Wire.read();

// 		Wire.beginTransmission(IP5306_ADDR);
// 		Wire.write(IP5306_REG_SYS_CTL0);
// 		if (en) Wire.write(data |  BOOST_OUT_BIT);
// 		else    Wire.write(data &(~BOOST_OUT_BIT));
// 		Wire.endTransmission();
// 		return true;
// 	}
// 	return false;
// }

int8_t getBatteryLevel()
{
  Wire.beginTransmission(IP5306_ADDR);
  Wire.write(0x78);
  if (Wire.endTransmission(false) == 0 && Wire.requestFrom(0x75, 1))
  {
    switch (Wire.read() & 0xF0)
    {
    case 0xE0:
      return 25;
    case 0xC0:
      return 50;
    case 0x80:
      return 75;
    case 0x00:
      return 100;
    default:
      return 0;
    }
  }
  return -1;
}

int readByte(uint8_t reg, uint8_t nbytes, uint8_t *data)
{
  Wire.beginTransmission(IP5306_ADDR);
  Wire.write(reg);
  Wire.endTransmission();
  Wire.requestFrom(IP5306_ADDR, nbytes);
  uint8_t index = 0;
  while (Wire.available())
    data[index++] = Wire.read();
}

/*


    int setBoostEnable(bool en);
    int setChargerEnable(bool en);
    int setPlugLoadOpened(bool en);
    int setBoostNormallyOpen(bool en);
    int setButtonPowerOffEnable(bool en);
    
    int setShutdownOption(uint8_t arg);
    int setLedControl(uint8_t arg);
    int setShortPressBoost(bool en);
    int setVinRemoveBoostOn(bool en);
    int setLowBatteryShutdown(bool en);

    int setLightLoadShutdownTime(uint8_t arg);

    int setChargerFullVoltage(uint8_t arg);

    int setBatteryStopChargerCurrent(uint8_t arg);
    int setChargerVoltage(uint8_t arg);

    int setBatteryVoltage(uint8_t arg);
    int setConstantPressureCharging(uint8_t arg);


    int setChargerLoopSeletc(uint8_t arg);

    int setChargerCurrent(uint8_t arg);

    bool isChargerEnable();

    bool isChargerFull();

    bool isHeavyLoad();

    bool isButtonDouble();
    bool isButtonLongPress();
    bool isButtonShortPress();







0.423s     INFO  [IP5306]  Register APP_BATTERY_IP5306_REG_SYS_CTL0 (0x00) value: 0x30
 0.425s     INFO  [IP5306]      Boost EN: enabled
 0.430s     INFO  [IP5306]      Charger EN: enabled
 0.436s     INFO  [IP5306]      Insert load auto power EN: disabled
 0.443s     INFO  [IP5306]      BOOST output normally open: disabled
 0.450s     INFO  [IP5306]      Key off EN: disabled

 0.457s     INFO  [IP5306]  Register APP_BATTERY_IP5306_REG_SYS_CTL1 (0x01) value: 0x1d
 0.461s     INFO  [IP5306]      Turn off boost control signal selection: short press twice
 0.473s     INFO  [IP5306]      Switch WLED flashlight control signal selection: short press twice
 0.491s     INFO  [IP5306]      Short press switch boost: disabled
 0.498s     INFO  [IP5306]      Whether to turn on Boost after VIN is pulled out: opened
 0.507s     INFO  [IP5306]      Batlow 3.0V Low Power Shutdown EN: enabled

 0.511s     INFO  [IP5306]  Register APP_BATTERY_IP5306_REG_SYS_CTL2 (0x02) value: 0x64
 0.532s     INFO  [IP5306]      Light load shutdown time setting: 32s

 0.540s     INFO  [IP5306]  Register APP_BATTERY_IP5306_REG_CHARG_CTL0 (0x20) value: 0x01
 0.548s     INFO  [IP5306]      Charging full stop setting: 4.17/4.275/4.32/4.365

 0.571s     INFO  [IP5306]  Register APP_BATTERY_IP5306_REG_CHARG_CTL1 (0x21) value: 0x89
 0.574s     INFO  [IP5306]      Battery end charge current detection: 500mA
 0.581s     INFO  [IP5306]      Charge undervoltage loop setting: 4.55

 0.591s     INFO  [IP5306]  Register APP_BATTERY_IP5306_REG_CHARG_CTL2 (0x22) value: 0x02
 0.599s     INFO  [IP5306]      Battery voltage setting: 4.2V
 0.611s     INFO  [IP5306]      Constant voltage charging voltage pressurization setting: 28mV

 0.627s     INFO  [IP5306]  Register APP_BATTERY_IP5306_REG_CHG_DIG_CTL0 (0x24) value: 0xd6

 0.636s     INFO  [IP5306]  Register APP_BATTERY_IP5306_REG_READ0 (0x70) value: 0x99
 0.644s     INFO  [IP5306]      Charging enable flag: Charging on

 0.651s     INFO  [IP5306]  Register APP_BATTERY_IP5306_REG_READ1 (0x71) value: 0x70
 0.660s     INFO  [IP5306]      Full of flags: Still charging

 0.661s     INFO  [IP5306]  Register APP_BATTERY_IP5306_REG_READ2 (0x72) value: 0x61
 0.675s     INFO  [IP5306]      Output light load flag: heavy load

 0.683s     INFO  [IP5306]  Register APP_BATTERY_IP5306_REG_READ3 (0x77) value: 0x83
 0.690s     INFO  [IP5306]      KEY button double click: 0
 0.697s     INFO  [IP5306]      KEY button long press: 1
 0.703s     INFO  [IP5306]      KEY button short press: 1

 0.710s     INFO  [IP5306]  Register APP_BATTERY_IP5306_REG_LEVEL (0x78) value: 0x00
  */