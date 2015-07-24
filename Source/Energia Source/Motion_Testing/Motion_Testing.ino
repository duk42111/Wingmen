
#include "Motion.h"
#include <Wire.h>

motion wingman;

//boolean stopped = false;

void setup()   
{
  wingman.setupMPU6050();
  //wingman.runDefaultSetup(); // THIS IS BREAKING THINGS
  wingman.runSafetySetup(); // THIS IS BREAKING THINGS
  //wingman.runTheftSetup(); // THIS IS BREAKING THINGS
  //Serial.println("Waiting...");
  pinMode(78, OUTPUT); // DEBUG
}

void loop()
{
  digitalWrite(78, HIGH); // DEBUG

 wingman.checkForStop();
 /*
 if(wingman.crashDetected())
 {
   //if(wingman.stopDetected())
   //{
     while(1)
     {
       // GSM code here
       wingman.emergencyFlash();
     }
   //}
 }*/

    /*if(!stopped && wingman.stopDetected())
    {
      wingman.brakeLight(ON);
      stopped = true;
    }
    if(stopped)
    {
      if(wingman.motionDetected())
      {
        wingman.brakeLight(OFF);
        stopped = false;
      }
    }*/
  
}
/*
void loop()
{
  //wingman.getAllMotionData();
  //wingman.crashDetected();
  //wingman.getCrashData();
  //delay(1000);
/*
  if(wingman.crashDetected())
  {
    wingman.emergencyFlash();
    while(1)
    {
      // placeholder -- GSM code would go here
      digitalWrite(78, HIGH);
      delay(20);
      digitalWrite(78, LOW);
      delay(20);
    }
  }
  /*else
  {*/

/*
// THE GOD CODE:
    if(!stopped && wingman.stopDetected())
    {
      wingman.brakeLight(ON);
      delay(5000);
      stopped = true;
      wingman.runTheftSetup();
    }
    if(stopped)
    {
      if(wingman.theftDetected())
      {
        stopped = false;
        wingman.brakeLight(OFF);
        //flashing lights
        delay(5000);
        wingman.runSafetySetup();
      }
    }
*/



  ///}
/*
  
  if(wingman.theftDetected()) // theft-detection
  {
    while(1)
    {
    //Serial.println(" 5-oh! 5-oh! 5-OH!");
    //delay(500);
    digitalWrite(BLUE_LED, HIGH);
    }
  }
  else
  {
    digitalWrite(BLUE_LED, LOW);
  }*/
//}


