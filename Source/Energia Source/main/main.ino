#include <gsm.h>
#include <Motion.h>

boolean firstTimeTheft = true;
boolean firstTimeStop = true;
boolean LOCKED = true;
boolean crashed = false;
int numberOfTexts = 0;
long checkerForSMS = 10000; //check every 10 seconds for a text
long globalStartTime = 0;
Gsm gsm = Gsm();
motion accel = motion();
void setup()
{
  accel.setupMPU6050();
  gsm.startGSM(9600);
  delay(5000); //let it start up
  numberOfTexts = gsm.getNumSMS(); //need to grab initial  
}



void loop()
{
  if(LOCKED)
  {
    if(firstTimeTheft)
    {
      accel.runTheftSetup();
      firstTimeTheft = !firstTimeTheft;
    } 
    if(accel.motionDetected())
    {
      bool retrieved = false;
      gsm.startGPS();
      while(!retrieved)
      {
        gsm.pingTheft(60000);
        //function that compares current number of texts with stored 
        int numCheck = gsm.getNumSMS();
        if(numCheck > numberOfTexts)
        {
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

      }
      } 
    }
    else
    {
      //o.w. check for texts
      //ok to do this way bc 1. itll take ~10 seconds to get this info 2. theft can still be detected afterwards
      int numCheck = gsm.getNumSMS();
      if(numCheck > numberOfTexts)
      {
        numberOfTexts = numCheck;
        bool check = gsm.checkForUnlock(numberOfTexts);
        String ret = "";
        if(check)
        {
          ret = "LET'S RIDE";
          LOCKED = !LOCKED;

          firstTimeStop = true; //allow for stop reads to happen

          globalStartTime = millis(); //do now, assumes that LOCK happens first

          //flushReceive(LONGTIME); //to make read easy

        }   
        else
        {
          ret = "NO RIDE";
        }
         gsm.sendSMS(ret,masterPhoneNumber);
      }

    }

  }



  if(!LOCKED)

  {  

    if(firstTimeStop)

    {

      firstTimeStop = !firstTimeStop; 

      accel.runSafetySetup();

    }

    accel.checkForStop(); 

    if(accel.crashDetected())

    {

      long time_start = millis();

      bool realStop = false;

      //is this worthwhile checking in?

      //don't check until bike is actually stopped

      while(millis() - time_start < 6000 && !realStop)

      {

        //you have 6 seconds to get me a real stop

          //loop dies if 6 seconds have passed or realstop is true

        if(accel.stopDetected())

        {

          realStop = true;

        }

      }

      if(realStop)

      {

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

          crashed = true;

          while(1)

          {

            gsm.startGPS();

            gsm.pingTheft(60000);

          } 

        }

      } 

      //o.w. not a valid crash

    }

    //only check for this if it is stopped!

    if(accel.stopDetected() && (millis() - globalStartTime > checkerForSMS))

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

      }

      globalStartTime = millis(); //its time to check for a text and update the "start" time to this time

    } 

  }









