/*!
 * @file Adafruit_BMP085.h
 *
 * This is a library for the Adafruit BMP085/BMP180 Barometric Pressure + Temp
 * sensor
 *
 * Designed specifically to work with the Adafruit BMP085 or BMP180 Breakout
 * ----> http://www.adafruit.com/products/391
 * ----> http://www.adafruit.com/products/1603
 *
 * These displays use I2C to communicate, 2 pins are required to
 * interface
 * Adafruit invests time and resources providing this open source code,
 * please support Adafruit and open-source hardware by purchasing
 * products from Adafruit!
 *
 * Written by Limor Fried/Ladyada for Adafruit Industries.
 * BSD license, all text above must be included in any redistribution
 */

#ifndef ADAFRUIT_BMP085_H
#define ADAFRUIT_BMP085_H

#include <Adafruit_I2CDevice.h>
#include <Arduino.h>

#define BMP085_DEBUG 0 //!< Debug mode

#define BMP085_I2CADDR 0x77 //!< BMP085 I2C address

#define BMP085_ULTRALOWPOWER 0 //!< Ultra low power mode
#define BMP085_STANDARD 1      //!< Standard mode
#define BMP085_HIGHRES 2       //!< High-res mode
#define BMP085_ULTRAHIGHRES 3  //!< Ultra high-res mode
#define BMP085_CAL_AC1 0xAA    //!< R   Calibration data (16 bits)
#define BMP085_CAL_AC2 0xAC    //!< R   Calibration data (16 bits)
#define BMP085_CAL_AC3 0xAE    //!< R   Calibration data (16 bits)
#define BMP085_CAL_AC4 0xB0    //!< R   Calibration data (16 bits)
#define BMP085_CAL_AC5 0xB2    //!< R   Calibration data (16 bits)
#define BMP085_CAL_AC6 0xB4    //!< R   Calibration data (16 bits)
#define BMP085_CAL_B1 0xB6     //!< R   Calibration data (16 bits)
#define BMP085_CAL_B2 0xB8     //!< R   Calibration data (16 bits)
#define BMP085_CAL_MB 0xBA     //!< R   Calibration data (16 bits)
#define BMP085_CAL_MC 0xBC     //!< R   Calibration data (16 bits)
#define BMP085_CAL_MD 0xBE     //!< R   Calibration data (16 bits)

#define BMP085_CONTROL 0xF4         //!< Control register
#define BMP085_TEMPDATA 0xF6        //!< Temperature data register
#define BMP085_PRESSUREDATA 0xF6    //!< Pressure data register
#define BMP085_READTEMPCMD 0x2E     //!< Read temperature control register value
#define BMP085_READPRESSURECMD 0x34 //!< Read pressure control register value

/*!
 * @brief Main BMP085 class
 */
class Adafruit_BMP085 {
public:
  Adafruit_BMP085();
  /*!
   * @brief Starts I2C connection
   * @param mode Mode to set, ultra high-res by default
   * @param wire The I2C interface to use, defaults to Wire
   * @return Returns true if successful
   */
  bool begin(uint8_t mode = BMP085_ULTRAHIGHRES, TwoWire *wire = &Wire);
  /*!
   * @brief Gets the temperature over I2C from the BMP085
   * @return Returns the temperature
   */
  float readTemperature(void);
  /*!
   * @brief Gets the pressure over I2C from the BMP085
   * @return Returns the pressure
   */
  int32_t readPressure(void);
  /*!
   * @brief Calculates the pressure at sea level
   * @param altitude_meters Current altitude (in meters)
   * @return Returns the calculated pressure at sea level
   */
  int32_t readSealevelPressure(float altitude_meters = 0);
  /*!
   * @brief Reads the altitude
   * @param sealevelPressure Pressure at sea level, measured in pascals
   * @return Returns the altitude
   */
  float readAltitude(float sealevelPressure = 101325); // std atmosphere
  /*!
   * @brief Reads the raw temperature
   * @return Returns the raw temperature
   */
  uint16_t readRawTemperature(void);
  /*!
   * @brief Reads the raw pressure
   * @return Returns the raw pressure
   */
  uint32_t readRawPressure(void);

private:
  int32_t computeB5(int32_t UT);
  uint8_t read8(uint8_t addr);
  uint16_t read16(uint8_t addr);
  void write8(uint8_t addr, uint8_t data);

  Adafruit_I2CDevice *i2c_dev;
  uint8_t oversampling;

  int16_t ac1, ac2, ac3, b1, b2, mb, mc, md;
  uint16_t ac4, ac5, ac6;
};

#endif //  ADAFRUIT_BMP085_H
