#include <gsm.h>
Gsm gsm = Gsm();

int numSMS = 0;
void setup()
{
  // put your setup code here, to run once:
  pinMode(RED_LED,OUTPUT); 
       digitalWrite(RED_LED,LOW);
       pinMode(GREEN_LED,OUTPUT); 
       digitalWrite(GREEN_LED,LOW);
  gsm.startGSM(9600);
  delay(5000);
 // numSMS = gsm.getNumSMS();
}

void loop()
{
  // put your main code here, to run repeatedly:
  /*a.flushReceive(flushTime);
  if(a.checkForUnlock()){
    
    digitalWrite(BLUE_LED,HIGH);
  }
  a.flushReceive(flushTime);
  if(a.checkForLock())
  {
    digitalWrite(BLUE_LED,HIGH);
    digitalWrite(RED_LED,HIGH);
  }*/
  
  //String a = String(a.getNumSMS());
  //a.sendSMS(a,masterPhoneNumber);
  
 /* bool check = a.checkForUnlock(numSMS);
  String ret = "true";
  if(!check)
    ret = "false";
  //String ret = ""+a.checkForUnlock(numSMS);*/
  
  //Serial.println("AT+CMGF=1\r");
  gsm.flushReceive(flushTime);
 	//calls getNumSMS() as numberOfTexts globally
 	/*String numTexts = String(17);
        Serial.println("AT+CMGF=1\r");
	delay(1000);
 	String command = "AT+CMGR=" + numTexts + "\r";
 delay(1000);*/
     
     //works    
/* Serial.println("AT+CMGR=21\r");

  String ret = rxChar();
  gsm.sendSMS(ret,masterPhoneNumber);*/
  
  //works
  /*numSMS = gsm.getNumSMS();
  gsm.flushReceive(flushTime);
  String number = String(numSMS);
  String command = "AT+CMGR="+number+"\r";
  Serial.print(command);
  delay(1000);
  String ret = rxChar();
    gsm.sendSMS(ret,masterPhoneNumber);*/
    
    /* working with main
    numSMS = gsm.getNumSMS();
     bool check =  gsm.checkForUnlock(numSMS);
     String ret = "NO RIDE";
     if(check)
       ret = "LET'S RIDE";
        gsm.sendSMS(ret,masterPhoneNumber);*/ 
        
        /* works gps
        gsm.startGPS();
        gsm.pingTheft(60000);*/


  while(1){}
  
}

String grabAllResponse(long time)
{
  String readString = "";
  bool done = false;
  digitalWrite(RED_LED,HIGH);
  int time_start = millis();

  while (Serial.available() > 0 || millis()-time_start < time) {
    delay(10);  
	if (Serial.available() >0) {
    	char c = Serial.read();
    	readString += c;
    }
   }
   digitalWrite(RED_LED,LOW);
   return readString;
}

bool sendAndExpect(String toGet)
{
  //Serial.print(toSend);
  //Serial.println("\r");
  bool done = false;
  while(!done)
  {
    String answer = grabAllResponse(responseTime);
    if(answer.indexOf(toGet) !=  -1)
    {
      done = true;
    }
  }
  return done;
}

String rxChar(){
  String readString = "";
  bool done = false;
         while (Serial.available() || !done) {
        delay(10); 
       char c = Serial.read(); 
    	if (c == '$')
        {
          do{
            digitalWrite(RED_LED,HIGH);
            if(c != '$'){
            readString = readString + c;}
            c = Serial.read();
          }while(c != '$');
          digitalWrite(RED_LED,LOW);
          done = true;
        }
   }
      return readString;
   
}
