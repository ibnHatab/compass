#ifndef _HMC5803L_LIBRARY_H
#define _HMC5803L_LIBRARY_H

#include <Arduino.h>
#include <Wire.h>

#include "sensor_library.h"

#ifdef __cplusplus
extern "C" {
#endif


/* The I2C address of the module */
#define HMC5803L_Address 0x1E

/* Register address for the X Y and Z data */
#define X 3
#define Y 7
#define Z 5

/*
  This function will initialise the module and only needs to be run once
  after the module is first powered up or reset
*/
    void Init_HMC5803L(void);

/*
   This function will read once from one of the 3 axis data registers
   and return the 16 bit signed result.
*/
    int HMC5803L_Read(byte Axis);

/*
   This function will read all 3 axis data registers
   and return the wire availability status.
*/
    bool HMC5803L_ReadAll(int& x, int& y, int& z);

/*
   This function will detect if wire is present
*/
    bool Detect_HMC5883L(void);

#ifdef __cplusplus
}
#endif

//    I2C ADDRESS/BITS

#define HMC5883_ADDRESS_MAG            (0x3C >> 1)         // 0011110x

/// REGISTERS
typedef enum
{
    HMC5883_REGISTER_MAG_CRA_REG_M             = 0x00,
    HMC5883_REGISTER_MAG_CRB_REG_M             = 0x01,
    HMC5883_REGISTER_MAG_MR_REG_M              = 0x02,
    HMC5883_REGISTER_MAG_OUT_X_H_M             = 0x03,
    HMC5883_REGISTER_MAG_OUT_X_L_M             = 0x04,
    HMC5883_REGISTER_MAG_OUT_Z_H_M             = 0x05,
    HMC5883_REGISTER_MAG_OUT_Z_L_M             = 0x06,
    HMC5883_REGISTER_MAG_OUT_Y_H_M             = 0x07,
    HMC5883_REGISTER_MAG_OUT_Y_L_M             = 0x08,
    HMC5883_REGISTER_MAG_SR_REG_Mg             = 0x09,
    HMC5883_REGISTER_MAG_IRA_REG_M             = 0x0A,
    HMC5883_REGISTER_MAG_IRB_REG_M             = 0x0B,
    HMC5883_REGISTER_MAG_IRC_REG_M             = 0x0C,
    HMC5883_REGISTER_MAG_TEMP_OUT_H_M          = 0x31,
    HMC5883_REGISTER_MAG_TEMP_OUT_L_M          = 0x32
} hmc5883MagRegisters_t;

typedef enum
{
    HMC5883_MAGGAIN_1_3                        = 0x20,  // +/- 1.3
    HMC5883_MAGGAIN_1_9                        = 0x40,  // +/- 1.9
    HMC5883_MAGGAIN_2_5                        = 0x60,  // +/- 2.5
    HMC5883_MAGGAIN_4_0                        = 0x80,  // +/- 4.0
    HMC5883_MAGGAIN_4_7                        = 0xA0,  // +/- 4.7
    HMC5883_MAGGAIN_5_6                        = 0xC0,  // +/- 5.6
    HMC5883_MAGGAIN_8_1                        = 0xE0   // +/- 8.1
} hmc5883MagGain;

typedef struct hmc5883MagData_s
{
    float x;
    float y;
    float z;
    float orientation;
} hmc5883MagData;

/* Unified sensor driver for the magnetometer */
class HMC5883 : public Sensor
{
  public:
    HMC5883();

    bool begin(void);
    void setMagGain(hmc5883MagGain gain);
    virtual bool getEvent(sensors_event_t*);
    virtual void getSensor(sensor_t*);

  private:
    hmc5883MagGain   _magGain;
    hmc5883MagData   _magData;     // Last read magnetometer data will be available here

    void write8(byte address, byte reg, byte value);
    byte read8(byte address, byte reg);
    void read(void);
};


#endif
