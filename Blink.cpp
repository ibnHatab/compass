
#include <Arduino.h>
#include <Wire.h>

#include "hmc5803l_library.h"
/*
  Blink
  Turns on an LED on for one second, then off for one second, repeatedly.

  This example code is in the public domain.
*/

// Pin 13 has an LED connected on most Arduino boards.
// give it a name:
int led = 13;

bool haveHMC5883L = false;

// the setup routine runs once when you press reset:
void no_setup() {
    // initialize the digital pin as an output.
    pinMode(led, OUTPUT);
    Serial.begin(9600);

    Wire.begin();
    // lower I2C clock http://www.gammon.com.au/forum/?id=10896
    TWBR = 78;  // 25 kHz
    TWSR |= _BV (TWPS0);  // change prescaler


    Serial.println("Powering Up...");
    delay(1000);               // wait for a second
}

// the loop routine runs over and over again forever:
void no_loop() {
    // heartbeat
    digitalWrite(led, HIGH);   // turn the LED on (HIGH is the voltage level)

    bool detect = Detect_HMC5883L();

    if(!haveHMC5883L)
    {
        if(detect)
        {
            haveHMC5883L = true;
            Serial.println("We have HMC5883L, moving on");

            Init_HMC5803L();
        }
        else
        {
            Serial.println("No HMC5883L detected!");
            delay(2000);
            return;
        }
    }
    else
    {
        if(!detect) {
            haveHMC5883L = false;
            Serial.println("Lost connection to HMC5883L!");
            delay(2000);
            return;
        }
    }


    int xvalue, yvalue, zvalue; //triple axis data
    if(HMC5803L_ReadAll(xvalue, yvalue, zvalue))
    {
        //Print out values of each axis
        Serial.print("x: ");
        Serial.print(xvalue);
        Serial.print("  y: ");
        Serial.print(yvalue);
        Serial.print("  z: ");
        Serial.println(zvalue);
    }
    else
    {
        Serial.println("Wire not available!");
    }



    digitalWrite(led, LOW);    // turn the LED off by making the voltage LOW
    delay(1250);               // wait for a second
}
