/*** FINAL UPDATE 7/23/15 ***/
/***   Taumer Anabtawi    ***/


#include <gsm.h>
#include "Motion.h"
#include <Wire.h>


/***TRIGGER VARIABLES***/

boolean firstTimeTheft = true;
boolean firstTimeStop = true;
boolean LOCKED = true;
boolean crashed = false;

/***INFORMATION VARIABLES***/
int numberOfTexts = 0;

/***TIMER VARIABLES***/
long checkerForSMS = 10000; //check every 10 seconds for a text
long globalStartTime = 0;
long theftTime = 0;
long crashTime = 0;

/***DEVICE OBJECTS***/
Gsm gsm = Gsm();
motion accel = motion(); //MPU6050

void setup()
{
  accel.setupMPU6050();
  gsm.startGSM(9600);
  delay(5000);
  //Allow ample time for devices to startup
  
  //Start GSM text mode
  Serial.println("AT+CMGF=1");
  delay(1000);
  
  //Make initial read of stored text messages
  numberOfTexts = gsm.getNumSMS(); 
  
  theftTime = millis();
}



void loop()
{
  bool theft = false;
  if(LOCKED)
  {
    if(firstTimeTheft)
    {
      //Switch MPU modes to read for theft
      accel.runTheftSetup();
      firstTimeTheft = !firstTimeTheft;
      theftTime = millis();
     } 
     
   //Attempt to read a theft for 3 seconds every iteration  
   while(millis() - theftTime < 3000)
   {
     if(accel.motionDetected())
     {
       //A theft has occured if motion is sensed while the bike is LOCKED
       theft = true;
       break; 
     }
   }
   
   if(theft)
   {
      //digitalWrite(BLUE_LED,HIGH);
      bool retrieved = false;
      gsm.startGPS();
      while(!retrieved)
      {
        gsm.pingGPSTheft(60000, masterPhoneNumber);
      } 
    }
    else{
    
      //No theft detected at this time
      //digitalWrite(GREEN_LED,LOW);
      
      //Check for incoming text messages
      int numCheck = gsm.getNumSMS();
      if(numCheck > numberOfTexts)
      {
        //digitalWrite(BLUE_LED,HIGH);
        String ret = "";
        //If a text was received from the master phone number, unlock bike and initialize unlock mode
        ret = "LET'S RIDE";
        LOCKED = !LOCKED;
        firstTimeStop = true;
        
        //notify user
        gsm.sendSMS(ret,masterPhoneNumber);
        globalStartTime = millis(); 
      }
      //incase of non-unlocking text, allow for more theft reads
     theftTime = millis(); 
    }
  }
  
  if(!LOCKED)
  {  
    bool crashDetect = false;
    //digitalWrite(BLUE_LED,LOW);
    if(firstTimeStop)
    {
      firstTimeStop = !firstTimeStop; 
      
      //initialize for stop and go detection
      accel.runSafetySetup();
      
      crashTime = millis();
      globalStartTime = millis();

    }

    long stopTime = millis();
   
    //constantly poll for motion/stopping
    accel.checkForStop(); 

    //allow ample time for a crash reading
    crashTime = millis();
    while(millis() - crashTime < 1000)
    {
      if(accel.crashDetected())
      {
       crashDetect = true;
       break;
      }
      else
      {
        crashDetect = false;
      }  
    }

    if(crashDetect)
    {
       //when a crash occures, send out gps ping's to helper phone number
       digitalWrite(BLUE_LED,HIGH);
       gsm.startGPS();
       while(1)
       { 
            gsm.pingGPSCrash(60000,lovedOneNumber);
            digitalWrite(BLUE_LED,LOW);
       } 
    }
    
    //grab a stop reading
    long stoppingTime = millis();
    bool stopGet = false;
    while(millis() - stoppingTime < 3000)
    {
       if(accel.stopDetected())
       {
         stopGet = true;
         break; 
       }
      
    }
    
    //ensure stop reading is a real stop reading by checking a stop for 15 seconds
    stoppingTime = millis();
    bool fullStop =false;
    while(stopGet && millis() - stoppingTime < 15000)
    {
       //can do because already stopped
       if(!accel.stopDetected())
       {
         fullStop = false; //false out 
         break;
       }
       else
       { 
         fullStop = true;   
       }
         
     }
     
     //if a real stop (no more biking)
     if(fullStop)
     {
          //now check for an incoming LOCK text
          int numCheck = gsm.getNumSMS();
          if(numCheck > numberOfTexts)
          {
            String ret = "RIDE OVER";
            LOCKED = !LOCKED; 
            firstTimeTheft = true; //can now check for theft
            gsm.sendSMS(ret,masterPhoneNumber); 
           } 
    }
  }
}









