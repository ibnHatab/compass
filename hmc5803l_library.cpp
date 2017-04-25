
#include <Arduino.h>
#include <Wire.h>
#include <limits.h>

#include "hmc5803l_library.h"


void Init_HMC5803L(void)
{
    /* Put the HMC5883 IC into the correct operating mode */
    Wire.beginTransmission(HMC5803L_Address); //open communication with HMC5883

    /* Set the module to 8x averaging and 15Hz measurement rate */
    Wire.write(0x00);
    Wire.write(0x70);

    /* Set a gain of 5 */
    Wire.write(0x01);
    Wire.write(0xA0);
    Wire.endTransmission();
}

int HMC5803L_Read(byte Axis)
{
    int Result;

    Wire.beginTransmission(HMC5803L_Address); //open communication with HMC5883
    Wire.write(0x02); //select mode register
    Wire.write(0x01); //not continuous measurement mode
    Wire.endTransmission();
    delay(6);

    /* Move modules the resiger pointer to one of the axis data registers */
    Wire.beginTransmission(HMC5803L_Address);
    Wire.write(Axis);
    Wire.endTransmission();

    /* Read the data from registers (there are two 8 bit registers for each axis) */
    Wire.requestFrom(HMC5803L_Address, 2);
    Result = Wire.read() << 8;
    Result |= Wire.read();

    return Result;
}

bool HMC5803L_ReadAll(int& x, int& y, int& z)
{
    Wire.beginTransmission(HMC5803L_Address); //open communication with HMC5883
    Wire.write(0x02); //select mode register
    Wire.write(0x01); //not continuous measurement mode
    Wire.endTransmission();
    delay(6);

    /* Move modules the resiger pointer to the X axis data registers */
    Wire.beginTransmission(HMC5803L_Address);
    Wire.write(X);
    Wire.endTransmission();

    /* Read the data from registers (there are two 8 bit registers for each axis) */
    Wire.requestFrom(HMC5803L_Address, 6);
    if(Wire.available() >= 6)
    {
        x = Wire.read() << 8;
        x |= Wire.read();
        z = Wire.read() << 8;
        z |= Wire.read();
        y = Wire.read() << 8;
        y |= Wire.read();

        return true;
    }

    return false;
}


bool Detect_HMC5883L(void)
{
    // read identification registers
    Wire.beginTransmission(HMC5803L_Address); //open communication with HMC5883
    Wire.write(10); //select Identification register A
    Wire.endTransmission();
    Wire.requestFrom(HMC5803L_Address, 3);
    if(3 == Wire.available()) {
        char a = Wire.read();
        char b = Wire.read();
        char c = Wire.read();
        if(a == 'H' && b == '4' && c == '3')
            return true;
    }

    return false;
}


static float hmc5883_Gauss_LSB_XY = 1100.0F;  // Varies with gain
static float hmc5883_Gauss_LSB_Z  = 980.0F;   // Varies with gain

void HMC5883::write8(byte address, byte reg, byte value)
{
    Wire.beginTransmission(address);
    Wire.write((uint8_t)reg);
    Wire.write((uint8_t)value);
    Wire.endTransmission();
}

byte HMC5883::read8(byte address, byte reg)
{
    byte value;

    Wire.beginTransmission(address);
    Wire.write((uint8_t)reg);
    Wire.endTransmission();

    Wire.requestFrom(address, (byte)1);
    value = Wire.read();
    Wire.endTransmission();

    return value;
}

void HMC5883::read()
{

    Wire.beginTransmission(HMC5803L_Address); //open communication with HMC5883
    Wire.write(HMC5883_REGISTER_MAG_MR_REG_M); //select mode register
    Wire.write(0x01); //not continuous measurement mode
    Wire.endTransmission();
    delay(6);

    // Read the magnetometer
    Wire.beginTransmission((byte)HMC5883_ADDRESS_MAG);
    Wire.write(HMC5883_REGISTER_MAG_OUT_X_H_M);
    Wire.endTransmission();

    Wire.requestFrom((byte)HMC5883_ADDRESS_MAG, (byte)6);

    // Wait around until enough data is available
    while (Wire.available() < 6);

    // Note high before low (different than accel)
    uint8_t xhi = Wire.read();
    uint8_t xlo = Wire.read();
    uint8_t zhi = Wire.read();
    uint8_t zlo = Wire.read();
    uint8_t yhi = Wire.read();
    uint8_t ylo = Wire.read();

    // Shift values to create properly formed integer (low byte first)
    _magData.x = (int16_t)(xlo | ((int16_t)xhi << 8));
    _magData.y = (int16_t)(ylo | ((int16_t)yhi << 8));
    _magData.z = (int16_t)(zlo | ((int16_t)zhi << 8));

    // ToDo: Calculate orientation
    _magData.orientation = 0.0;
}

HMC5883::HMC5883()
{
}

bool HMC5883::begin()
{
    // Enable I2C
    Wire.begin();

    // read identification registers
    Wire.beginTransmission(HMC5883_ADDRESS_MAG); //open communication with HMC5883
    Wire.write(10); //select Identification register A
    Wire.endTransmission();
    Wire.requestFrom(HMC5883_ADDRESS_MAG, 3);

    if(3 == Wire.available()) {
        char a = Wire.read();
        char b = Wire.read();
        char c = Wire.read();
        if(! (a == 'H' && b == '4' && c == '3'))
            return false;
    }

    // Set the module to 8x averaging and 15Hz measurement rate
    write8(HMC5883_ADDRESS_MAG, HMC5883_REGISTER_MAG_CRA_REG_M, 0x70);

    // Set the gain to a known level
    //setMagGain(HMC5883_MAGGAIN_4_7);
    setMagGain(HMC5883_MAGGAIN_1_3);

    // Enable the magnetometer
    write8(HMC5883_ADDRESS_MAG, HMC5883_REGISTER_MAG_MR_REG_M, 0x00);

    return true;
}

void HMC5883::setMagGain(hmc5883MagGain gain)
{
    write8(HMC5883_ADDRESS_MAG, HMC5883_REGISTER_MAG_CRB_REG_M, (byte)gain);

    _magGain = gain;

    switch(gain)
    {
    case HMC5883_MAGGAIN_1_3:
        hmc5883_Gauss_LSB_XY = 1100;
        hmc5883_Gauss_LSB_Z  = 980;
        break;
    case HMC5883_MAGGAIN_1_9:
        hmc5883_Gauss_LSB_XY = 855;
        hmc5883_Gauss_LSB_Z  = 760;
        break;
    case HMC5883_MAGGAIN_2_5:
        hmc5883_Gauss_LSB_XY = 670;
        hmc5883_Gauss_LSB_Z  = 600;
        break;
    case HMC5883_MAGGAIN_4_0:
        hmc5883_Gauss_LSB_XY = 450;
        hmc5883_Gauss_LSB_Z  = 400;
        break;
    case HMC5883_MAGGAIN_4_7:
        hmc5883_Gauss_LSB_XY = 400;
        hmc5883_Gauss_LSB_Z  = 255;
        break;
    case HMC5883_MAGGAIN_5_6:
        hmc5883_Gauss_LSB_XY = 330;
        hmc5883_Gauss_LSB_Z  = 295;
        break;
    case HMC5883_MAGGAIN_8_1:
        hmc5883_Gauss_LSB_XY = 230;
        hmc5883_Gauss_LSB_Z  = 205;
        break;
    }
}

bool HMC5883::getEvent(sensors_event_t *event) {
    /* Clear the event */
    memset(event, 0, sizeof(sensors_event_t));

    /* Read new data */
    read();

    event->type      = SENSOR_TYPE_MAGNETIC_FIELD;
    event->timestamp = 0;
    event->magnetic.x = _magData.x / hmc5883_Gauss_LSB_XY * SENSORS_GAUSS_TO_MICROTESLA;
    event->magnetic.y = _magData.y / hmc5883_Gauss_LSB_XY * SENSORS_GAUSS_TO_MICROTESLA;
    event->magnetic.z = _magData.z / hmc5883_Gauss_LSB_Z * SENSORS_GAUSS_TO_MICROTESLA;

    return true;
}

void HMC5883::getSensor(sensor_t *sensor) {
    /* Clear the sensor_t object */
    memset(sensor, 0, sizeof(sensor_t));

    /* Insert the sensor name in the fixed length char array */
    strncpy (sensor->name, "HMC5883", sizeof(sensor->name) - 1);
    sensor->name[sizeof(sensor->name)- 1] = 0;
    sensor->type        = SENSOR_TYPE_MAGNETIC_FIELD;
    sensor->min_delay   = 0;
    sensor->max_value   = 800; // 8 gauss == 800 microTesla
    sensor->min_value   = -800; // -8 gauss == -800 microTesla
    sensor->resolution  = 0.2; // 2 milligauss == 0.2 microTesla
}
