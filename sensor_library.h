#ifndef _SENSOR_LIBRARY_H
#define _SENSOR_LIBRARY_H

#include <Arduino.h>

#define SENSORS_DPS_TO_RADS               (0.017453293F)          /**< Degrees/s to rad/s multiplier */
#define SENSORS_GAUSS_TO_MICROTESLA       (100)                   /**< Gauss to micro-Tesla multiplier */

/** Sensor types */
typedef enum
{
    SENSOR_TYPE_ACCELEROMETER         = (1),   /**< Gravity + linear acceleration */
    SENSOR_TYPE_MAGNETIC_FIELD        = (2),
    SENSOR_TYPE_ORIENTATION           = (3),
    SENSOR_TYPE_GYROSCOPE             = (4),
    SENSOR_TYPE_LIGHT                 = (5),
    SENSOR_TYPE_PRESSURE              = (6),
    SENSOR_TYPE_PROXIMITY             = (8),
    SENSOR_TYPE_GRAVITY               = (9),
    SENSOR_TYPE_LINEAR_ACCELERATION   = (10),  /**< Acceleration not including gravity */
    SENSOR_TYPE_ROTATION_VECTOR       = (11),
    SENSOR_TYPE_RELATIVE_HUMIDITY     = (12),
    SENSOR_TYPE_AMBIENT_TEMPERATURE   = (13),
    SENSOR_TYPE_VOLTAGE               = (15),
    SENSOR_TYPE_CURRENT               = (16),
    SENSOR_TYPE_COLOR                 = (17)
} sensors_type_t;


typedef struct {
    union {
        float v[3];
        struct {
            float x;
            float y;
            float z;
        };
        /* Orientation sensors */
        struct {
            float roll;    /**< Rotation around the longitudinal axis (the plane body, 'X axis'). Roll is positive and increasing when moving downward. -90°<=roll<=90° */
            float pitch;   /**< Rotation around the lateral axis (the wing span, 'Y axis'). Pitch is positive and increasing when moving upwards. -180°<=pitch<=180°) */
            float heading; /**< Angle between the longitudinal axis (the plane body) and magnetic north, measured clockwise when viewing from the top of the device. 0-359° */
        };
    };
    int8_t status;
    uint8_t reserved[3];
} sensors_vec_t;

/** struct sensor_event_s is used to provide a single sensor event in a common format. */
typedef struct
{
    sensors_type_t type;  /**< sensor type */
    int32_t timestamp;    /**< time is in milliseconds */
    union
    {
        sensors_vec_t   magnetic;             /**< magnetic vector values are in micro-Tesla (uT) */
        sensors_vec_t   orientation;          /**< orientation values are in degrees */
        float           distance;             /**< distance in centimeters */
        float           light;                /**< light in SI lux units */
    };
} sensors_event_t;

typedef struct
{
    char     name[12];   /**< sensor name */
    int32_t  type;       /**< this sensor's type (ex. SENSOR_TYPE_LIGHT) */
    float    max_value;                       /**< maximum value of this sensor's value in SI units */
    float    min_value;                       /**< minimum value of this sensor's value in SI units */
    float    resolution;                      /**< smallest difference between two values reported by this sensor */
    int32_t  min_delay;                       /**< min delay in microseconds between events. zero = not a constant rate */
} sensor_t;

class Sensor {
public:
    // Constructor(s)
    Sensor() {}
    virtual ~Sensor() {}

    // These must be defined by the subclass
    virtual bool getEvent(sensors_event_t*) = 0;
    virtual void getSensor(sensor_t*) = 0;

    void displaySensorDetails(void);
};


#endif
