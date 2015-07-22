#include <gsm.h>

boolean firstTimeTheft = true;
boolean firstTimeStop = true;
boolean LOCKED = true;
boolean crashed = false;
int numberOfTexts = 0;
long checkerForSMS = 10000; //check every 10 seconds for a text
long globalStartTime = 0;
Gsm gsm = Gsm();
void setup()
{
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
      //startTheftRead
      firstTimeTheft = !firstTimeTheft;
    } 
    if(theftDetected())
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
          retrieved = gsm.checkForUnlock(numberOfTexts) ;
          //if false, this text is not an unlock text, ping theft again and wait for next text 
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
        if(gsm.checkForUnlock(numberOfTexts))
        {

          LOCKED = !LOCKED;

          firstTimeStop = true; //allow for stop reads to happen

          globalStartTime = millis(); //do now, assumes that LOCK happens first

          //flushReceive(LONGTIME); //to make read easy

        }   

      }

    }

  }



  if(!LOCKED)

  {  

    if(firstTimeStop)

    {

      firstTimeStop = !firstTimeStop; 

      setUpStop();

    }

    stopAndGoLightsFunction(); 

    if(crashDetected())

    {

      long time_start = millis();

      bool realStop = false;

      //is this worthwhile checking in?

      //don't check until bike is actually stopped

      while(millis() - time_start < 6000 && !realStop)

      {

        //you have 6 seconds to get me a real stop

          //loop dies if 6 seconds have passed or realstop is true

        if(detectStop())

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

          if(!stopDetected())

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

    if(stopped && (millis() - globalStartTime > checkerForSMS))

    {

      //done this way to minimize chance of missing a crash

      //i.e. a crash while reading number of texts

      int numCheck = gsm.getNumSMS();

      if(numberOfTexts < numCheck)

      {

        numberOfTexts = numCheck;

        if(gsm.checkForLock(numberOfTexts))

        {

          LOCKED = !LOCKED; //flips

          firstTimeTheft = true; //can now check for theft

        } 

      }

      globalStartTime = millis(); //its time to check for a text and update the "start" time to this time

    } 

  }

}  







