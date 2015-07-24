#include <gsm.h>
#include "Motion.h"
#include <Wire.h>

boolean firstTimeTheft = true;
boolean firstTimeStop = true;
boolean LOCKED = false; //starts true
boolean crashed = false;
int numberOfTexts = 0;
long checkerForSMS = 10000; //check every 10 seconds for a text
long globalStartTime = 0;
long theftTime = 0;
long crashTime = 0;
Gsm gsm = Gsm();
motion accel = motion();
void setup()
{
  accel.setupMPU6050();
  gsm.startGSM(9600);
  delay(5000); //let it start up
Serial.println("AT+CMGF=1");
      delay(1000);
      numberOfTexts = gsm.getNumSMS(); //need to grab initial  
      theftTime = millis();
}



void loop()
{
  bool theft = false;
  if(LOCKED)
  {
    if(firstTimeTheft)
    {
      accel.runTheftSetup();
      firstTimeTheft = !firstTimeTheft;
      theftTime = millis();
    } 
   while(millis() - theftTime < 3000)
   {
     if(accel.motionDetected())
     {
       theft = true;
        break; 
     }
     
   }
    if(theft)
    {
      digitalWrite(BLUE_LED,HIGH);
      bool retrieved = false;
      gsm.startGPS();
      while(!retrieved)
      {
        gsm.pingTheft(60000);
        //function that compares current number of texts with stored 
        /*int numCheck = gsm.getNumSMS();
        if(numCheck > numberOfTexts)
        {
          digitalWrite(BLUE_LED,LOW);
          numberOfTexts = numCheck;
          bool check =  gsm.checkForUnlock(numberOfTexts);
          String ret = "";
          if(check){
             ret = "RETRIEVED";
          //if false, this text is not an unlock text, ping theft again and wait for next text 
          }  
          else
          {
            ret = "BIKE STILL IN MOTION";
          }
          gsm.sendSMS(ret,masterPhoneNumber);

      }*/
      } 
    }
    else{
    
    //not checking for theft
      digitalWrite(GREEN_LED,LOW);
      //o.w. check for texts
      //ok to do this way bc 1. itll take ~10 seconds to get this info 2. theft can still be detected afterwards
      //Serial.println("AT+CMGF=1");
      //delay(1000);
      //gsm.flushReceive(flushTime);
      int numCheck = gsm.getNumSMS();
      if(numCheck > numberOfTexts)
      {
              digitalWrite(BLUE_LED,HIGH);

        //numberOfTexts = numCheck;
        //bool check = false;//gsm.checkForUnlock(numberOfTexts);
        String ret = "";
        //if(check)
        //{
          ret = "LET'S RIDE";
          LOCKED = !LOCKED;

          firstTimeStop = true; //allow for stop reads to happen


          //flushReceive(LONGTIME); //to make read easy

        //}   
        //else
        //{
         // ret = "NO RIDE";
       // }
        /*gsm.flushReceive(flushTime);
        String number = String(numberOfTexts);
        String command = "AT+CMGR="+number+"\r";
      Serial.print(command);
      delay(1000);
      //ret = rxChar();
      gsm.flushReceive(flushTime);
      ret = gsm.grabAllResponse(25000);
         gsm.sendSMS(ret,masterPhoneNumber);*/
  /*gsm.flushReceive(flushTime);
  String number = String(numberOfTexts);
  String command = "AT+CMGR="+number+"\r";
  Serial.print(command);
  delay(1000);
  //String ret = rxChar();
  ret = gsm.grabAllResponse(responseTime);
  //ret = number;*/
  gsm.sendSMS(ret,masterPhoneNumber);
            globalStartTime = millis(); //do now, assumes that LOCK happens first

      }
   theftTime = millis(); //current implemenetation does not use this, incase failed text
    }


  }



  if(!LOCKED)

  {  
    bool crashDetect = false;
  digitalWrite(BLUE_LED,LOW);
  if(firstTimeStop)

    {

      firstTimeStop = !firstTimeStop; 

      accel.runSafetySetup();
      
      crashTime = millis();
      globalStartTime = millis();

    }

    long stopTime = millis();
    //while(millis() - stopTime < 1000)
    //{
    accel.checkForStop(); 
    //}
    
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

      digitalWrite(BLUE_LED,HIGH);
      delay(100);
      digitalWrite(BLUE_LED,LOW);
      digitalWrite(GREEN_LED,HIGH);
            delay(100);

            digitalWrite(GREEN_LED,LOW);

      digitalWrite(RED_LED,HIGH);
            delay(100);

            digitalWrite(RED_LED,LOW);
            digitalWrite(BLUE_LED,HIGH);
            delay(100);
            digitalWrite(BLUE_LED,LOW);
      digitalWrite(GREEN_LED,HIGH);
            delay(100);

            digitalWrite(GREEN_LED,LOW);

      digitalWrite(RED_LED,HIGH);
            delay(100);

            digitalWrite(RED_LED,LOW);

      /*long time_start = millis();

      bool realStop = false;

      //is this worthwhile checking in?

      //don't check until bike is actually stopped

      while(millis() - time_start < 6000 && !realStop)

      {

        //you have 6 seconds to get me a real stop

          //loop dies if 6 seconds have passed or realstop is true
        stopTime = millis();
        while(millis() - stopTime < 3000)

        {
          if(accel.stopDetected()){
          digitalWrite(RED_LED,HIGH);
          realStop = true;
          }

        }

      }

      if(realStop)

      {
        digitalWrite(RED_LED,LOW);
        long time_start = millis();

        bool stillStopped  = false;

        while(millis()- time_start < 30000 && !stillStopped)

        {

          if(!accel.stopDetected())

          {

            stillStopped = false; 

          }

        }

        if(stillStopped)

        {
        //digitalWrite(RED_LED,LOW);
          crashed = true;

          while(1)

          {

            gsm.startGPS();

            gsm.pingTheft(60000);

          } 

        }

      } */

      //o.w. not a valid crash
      
       while(1)

          {
            digitalWrite(BLUE_LED,HIGH);
            gsm.startGPS();

            gsm.pingTheft(60000);
            digitalWrite(BLUE_LED,LOW);

          } 

    }

    //only check for this if it is stopped!
    
    //initial stop
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
    stoppingTime = millis();
    bool fullStop =false;
    while(millis() - stoppingTime < 15000)
    {
      //can do because already stopped
       if(!accel.stopDetected())
       {
         fullStop = false; //false out 
         break;
       }
       else
       {
         
        fullStop = true; //can only be true if false is never returned
            
          }
         
       }
       if(fullStop)
       {
         
        int numCheck = gsm.getNumSMS();
          if(numCheck > numberOfTexts)
          {
            String ret = "RIDE OVER";
          LOCKED = !LOCKED; //flips
          firstTimeTheft = true; //can now check for theft
                  gsm.sendSMS(ret,masterPhoneNumber); 
         
       }
      
      
    }

    /*if(stopGet && (millis() - globalStartTime > checkerForSMS))

    {

      //done this way to minimize chance of missing a crash

      //i.e. a crash while reading number of texts

      int numCheck = gsm.getNumSMS();

      if(numberOfTexts < numCheck)

      {

        numberOfTexts = numCheck;
        bool check =  gsm.checkForLock(numberOfTexts);
        String ret = "";
        if(check){
          ret = "RIDE OVER";
          LOCKED = !LOCKED; //flips
          firstTimeTheft = true; //can now check for theft
        }
        else
        {
          ret = "NOT DONE RIDING?";
        }
        gsm.sendSMS(ret,masterPhoneNumber);
        } 
      globalStartTime = millis(); //its time to check for a text and update the "start" time to this time

      }*/


    
  }
}









