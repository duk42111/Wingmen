
#include "Motion.h"
#include <Wire.h>

motion wingman;

void setup()
{
  wingman.setupMPU6050();
  wingman.runSafetySetup();
  //wingman.runMotionSetup();
}

void loop()
{
  //wingman.getCrashData();
  /*
  wingman.getRotData();
  Serial.print(" | GyX = "); Serial.print(wingman.rotX);
  Serial.print(" | GyY = "); Serial.print(wingman.rotY);
  Serial.print(" | GyZ = "); Serial.println(wingman.rotZ);
  delay(1000);

/*
  wingman.getRotData();
  if(wingman.crashDetected() == TRUE)
  {
    wingman.emergencyFlash();
    while(1)
    {
      // GSM code would go here
      digitalWrite(78, HIGH);
      delay(20);
      digitalWrite(78, LOW);
      delay(20);
    }
  }
  else
  {
    if(wingman.motionDetected() == false)
    {
      wingman.brakeLight(ON);
    }
    else
    {
      wingman.brakeLight(OFF);
    }
  }

  /*
  if(wingman.theftDetected())
  {
    for(;;)
    {
    //Serial.println(" 5-oh! 5-oh! 5-OH!");
   // wingman.getRotData();
    //Serial.print(" | GyX = "); Serial.print(wingman.rotX);
    //Serial.print(" | GyY = "); Serial.print(wingman.rotY);
    //Serial.print(" | GyZ = "); Serial.println(wingman.rotZ);
    //delay(500);
    digitalWrite(RED_LED, HIGH);
  }}
  else
  {
    digitalWrite(RED_LED, LOW);
  }

  //wingman.interruptDebug();

  /*
  wingman.getAllMotionData();
  Serial.print(" | resAcc = "); Serial.print(wingman.resultantAcc());
  Serial.print(" | resRot = "); Serial.println(wingman.resultantRot());
  delay(1000);
  /*
  Serial.print("AcX = "); Serial.print(wingman.accX);
  Serial.print(" | AcY = "); Serial.print(wingman.accY);
  Serial.print(" | AcZ = "); Serial.print(wingman.accZ);
  //Serial.print(" | Tmp = "); Serial.print(wingman.temp/340.00+36.53);  //equation for temperature in degrees C from datasheet
  Serial.print(" | GyX = "); Serial.print(wingman.rotX);
  Serial.print(" | GyY = "); Serial.print(wingman.rotY);
  Serial.print(" | GyZ = "); Serial.println(wingman.rotZ);
  delay(1000);
  */


}
