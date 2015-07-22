
#include <gsm.h>


 char* replybuffer;
Gsm a = Gsm();
void setup()
{
  
  pinMode(RED_LED,OUTPUT); 
       digitalWrite(RED_LED,LOW);
       pinMode(GREEN_LED,OUTPUT); 
       digitalWrite(GREEN_LED,LOW);
  // put your setup code here, to run once:
 //a.startGSM(9600);
 a.startGSM(9600);
 /*String response = "";
 while(Serial.available() > 0)
    {
      char c = Serial.read();
      //response = response + c;
    }
    delay(3000);
    Serial.println("AT\r");
    //delay(5000);
    readRaw(120);
    //Serial.println(response);
    if(replybuffer[0] == 'O')
    {
       pinMode(RED_LED,OUTPUT); 
       digitalWrite(RED_LED,LOW);
       digitalWrite(RED_LED,HIGH);
    }*/
    //delay(5000);
    //String reply2 = sendRead("AT+CMGR=13","$third $");
    //String reply = sendRead("AT","OK");
    
    //flushLine();
    //flushLine();
    //String flushed = flushLine();
    //flushInput();
    //delay(10000);
    //Serial.println("AT+CMGR=15\r");
    //delay(10000);
   
    //String reply = String(Serial.available());
    //String flushed = flushInput();
   /* if(reply2.indexOf("OK") != -1)
    {
      flash(GREEN_LED);
      flash(GREEN_LED);
      flash(GREEN_LED);
    }*/
    //String reply = flushInput();
 //a.sendSMS(reply,"16144600335");

}

void loop()
{
  // put your main code here, to run repeatedly:
  /*while(Serial.available() > 0)
    {
      flash(RED_LED);
    }*/
    /*if(Serial.available() > 30){
    flash(GREEN_LED);}*/
    //Serial.flush();
   // Serial.println("AT+CMGR=11\r");
    //Serial.flush(); //waits for transmit to finish
    //prepareForResponse("AT+CMGR=11");
    //delay(15000);
    digitalWrite(GREEN_LED,true); //transmit done
    //delay(15000);
    //Serial.println("ATE0\r"); //stop echos
    //flushRecieve();
    //Serial.println("AT+CMGR=12\r");
    //readFullLine();
    //readFullLine();
    //readFullLine();
    /*digitalWrite(GREEN_LED,HIGH);
    delay(15000);
    digitalWrite(GREEN_LED,LOW);*/
    //String str = readFullLine();
    //readToChar();
    //readFullLine();
    //readFullLine();
    //String str = rxChar();
    
    //gets response
    //NEED TO GET THE CURRENT NUMBER OF TEXT IN ORDER TO READ THE LATEST TEXT
    /*
    flushRecieve();
    Serial.println("AT+CMGR=13\r");

    String str = rxChar();

    a.sendSMS(str,"16144600335");*/
    
    //checks for existence of response
    /*flushRecieve();
    bool response = sendAndExpect("AT+CMGR=13","third");
    if(response)
    {
      flash(RED_LED);
      flash(GREEN_LED);
      flash(RED_LED);
      flash(RED_LED);
      flash(GREEN_LED);
      flash(RED_LED);
      flash(RED_LED);
      flash(GREEN_LED);
      flash(RED_LED);
    }*/
    
    /*
    //working gps
    flushRecieve();
    //sendAndExpect("AT+CGPSPWR=1","OK"); //consumes OK
    Serial.println("AT+CPGPSPWR=1\r");
    Serial.println("AT+CGPSRST=0\r");
    Serial.println("WAIT=60\r");
    flushRecieve();
    Serial.println("AT+CGPSINF=0\r");
    Serial.println("AT+CGPSSTATUS?\r");
    String str = rx();
    //String str = readFullLine();
    a.sendSMS(str,"16144600335");*/
    
    /*
    // working theft gps
    flushRecieve();
    Serial.println("AT+CPGPSPWR=1\r");
    Serial.println("AT+CGPSRST=0\r");
    Serial.println("WAIT=60\r");
    flushRecieve();
    pingTheft();*/
    
    /*working grab number of texts
    flushRecieve();
    Serial.println("AT+CPMS?");
    String response = rx();
    int num = processNumSMSString(response);
    String r = String(num);
    a.sendSMS(r,"16144600335");*/
    
    while(1){}
}

//write loop to flush param number of lines
String sendRead(String whatToSend, String expected)
{
   flushInput();
   Serial.print(whatToSend);
   Serial.println("\r");
   //delay(5000);
   //flushInputForResponse(':');
   //flushLine();
   //flushLine();
  String z = ""; //= flushInput();
   //String z = "";
   
   //first comment started here
  /* while(Serial.available() > 0)
   {
     char c = Serial.read(); //to convert to letter
     if(c != 13){
       //'\r'
         z =z + c;
     } //first comment died here
   }*/
     /*
     char c = Serial.read();
     if(c == '$')
     {
         boolean valid = true;
         while(valid){
         char next = Serial.read(); //need this line to convert from ascii
         if(next != '$'){
         z = z + next;
         flash(GREEN_LED);
       flash(GREEN_LED);
       flash(GREEN_LED);
         }
         else{ valid = false;} //kill
        }
     }
     else
     {
       //keep reading 
       Serial.read(); //throw away
     } 
   }
   z = processResponse(z);*/
   if(z == expected){
       flash(GREEN_LED);
       flash(GREEN_LED);
       flash(GREEN_LED);
   }
   return z;
  /*if(z.equals(expected)){
    return z;}
   if(z.length() == 0){
     pinMode(RED_LED,OUTPUT); 
       digitalWrite(RED_LED,LOW);
       digitalWrite(RED_LED,HIGH);
   }
   else
     return "false";*/ 
}


String flushInput()
{
  String ret = "";
  while(Serial.available() > 0){
    char c = Serial.read();
    ret = ret + c;
    delay(1);
  }
  return ret;
}

void flushInputForResponse(char checker)
{
   //flush until you find the checker, don't read the checker
   //or read it? we don't need it...
   bool found = false;
   while(Serial.available() > 0 && !found)
   {
     char c = Serial.peek();
     if(c == checker)
     {
       found = true;
       flash(GREEN_LED);
       flash(GREEN_LED);
     }
     if(!found){
       flash(RED_LED);
     Serial.read(); //flush
     }
     //o.w. keep looping
     //could possible not detect error may need to do that
   }
}

//it looks like this damn thing is reading EVERYTHING that it can
//lets put some characters to look for..........
uint16_t readRaw(uint16_t b) {
  uint16_t idx = 0;

  while (b && (idx < sizeof(replybuffer)-1)) {
    if (Serial.available() > 0) {
      replybuffer[idx] = Serial.read();
      idx++;
      b--;
    }
  }
  replybuffer[idx] = 0;

  return idx;
}

//ok awesome so you can look for indexOf() CHAR OR STRIN
//SO WE CAN LITERALLY JUST SEARCH FOR THE NUMBER OR WHATEVR PSASWORD ETC!!!!
String processResponse(String response)
{
   int index = response.indexOf('$'); //returns first occurence
   int indexEnd = response.indexOf('$',index+1); //look for next occurance of $
   return response.substring(index,indexEnd+1); //need whole string its [,)
  
}

void flash(int led)
{
  digitalWrite(led,LOW);
  delay(10);
       digitalWrite(led,HIGH);
       delay(10);
         digitalWrite(led,LOW); 
}

//returns line too
String flushLine(){
 //assumes you have already flushed to the line you need to flush
  bool found = false;
  String ret = "";
   while(!found)
  {
      char c = Serial.read();
       if(c == '\r' || c == '\n')
       {
        found = true;
        flash(RED_LED);
        flash(RED_LED);
        flash(RED_LED);
       } 
       else
       {
          ret = ret +c; //already converted
          //this will NOT include the \r.....
       }
  }
  return ret;
}
String readLine(){
  int replyidx = 0;
  String ret = "";
while(Serial.available()) {
      char c =  Serial.read();
      if (c == '\r') continue;
      if (c == 0xA) {
        if (replyidx == 0)   // the first 0x0A is ignored
          continue;
      }
      ret = ret + c;
      //Serial.print(c, HEX); Serial.print("#"); Serial.println(c);
      replyidx++;
    }
}


String rx(){
  String readString = "";
  bool done = false;
  digitalWrite(RED_LED,HIGH);
  int time_start = millis();
         while (Serial.available() > 0 || millis()-time_start < 15000) {
        delay(10);  
    	if (Serial.available() >0) {
        char c = Serial.read();
        readString += c;}
        
        
      /*if (readString.length() >10) {
        done = true;
        //digitalWrite(GREEN_LED,true);
      } */
   }
   digitalWrite(RED_LED,LOW);
   return readString;
   
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


void flushRecieve(){
  int time_start = millis();
  while(Serial.available() > 0 || (millis() - time_start < 15000))
  {
    delay(10);
    if(Serial.available() > 0)
    {
      char c = Serial.read();
      if(c == '\r' || c == '\n'){ continue;}
    }
  }
  digitalWrite(GREEN_LED,LOW);
  
}

void prepareForResponse(String toSend)
{
  Serial.flush(); //wait for all transmit
  flushRecieve(); //clear everything out at this point
  Serial.print(toSend);
  Serial.println("\r");
  Serial.flush(); //wait for this transmit
  //now we can read
}

//reads until a \r or \n
String readFullLine()
{
  String ret = "";
  int count = 0;
  digitalWrite(RED_LED,HIGH);
  int time_start = millis();
  while(Serial.available() > 0 || (millis() - time_start < 15000))
  {
   
    delay(10);
    char c;
    if(Serial.available() > 0)
    {
      c = Serial.read();
      ret = ret + c;
    }
     if(c == '\r' || c == '\n')
    {
      count++;
    }
    if(count == 2)
    {
      break;
    }
  }
  digitalWrite(RED_LED,LOW);
  return ret;
}

//reads until a \r or \n
void readToChar()
{
  //String ret = "";
  int count = 0;
  digitalWrite(RED_LED,HIGH);
  int time_start = millis();
  bool done = false;
  while(!done)
  {
   
    delay(10);
    char c;
    if(Serial.available() > 0)
    {
      c = Serial.read();
     // ret = ret + c;
    }
     if(c == 'O')
    {
      done = true;
    }
  }
  digitalWrite(RED_LED,LOW);
  //return ret;
}

bool sendAndExpect(String toSend, String toGet)
{
  Serial.print(toSend);
  Serial.println("\r");
  bool done = false;
  while(!done)
  {
    String answer = rx();
    if(answer.indexOf(toGet) != -1)
    {
      done = true;
    }
  }
  return true;
}
/*
String sendAndGrab(String toSend, String toGet)
{
  Serial.print(toSend);
  Serial.println("\r");
  bool done = false;
  while(!done)
  {
    String answer = rx();
    if(answer.indexOf(toGet) != -1)
    {
      done = true;
    }
  }
  return answer;
}*/

void pingTheft()
{
  int time_start = millis();
  while(millis()-time_start < 60000)
  {
    Serial.println("AT+CGPSINF=0\r");
    //Serial.println("AT+CGPSSTATUS?\r");
    String str = rx();
    String message = processGPSString(str);
    a.sendSMS(message,"16144600335");
    flushRecieve();
    delay(1000);
  } 
}

String processGPSString(String gpsRet)
{
  String longitude = "";
  String latitude = "";
  int index = gpsRet.indexOf(','); //first index of ,
  String temp = gpsRet.substring(index+1); //from ehre on
  int index2 = temp.indexOf(','); //next comma
  longitude = temp.substring(0,index2); //+1 on index to get rid of , and no on last index for no comma
  //get latitiude;
  index = temp.indexOf(','); //find the next comma
  String temp2 = temp.substring(index+1);
  index2 = temp2.indexOf(',');
  latitude = temp2.substring(0,index2);
  return "Last seen at: Longitude: " +longitude + " Latitude: " + latitude;
  }
  
int processNumSMSString(String response)
{
  int index = response.indexOf(','); //first comma
  String temp = response.substring(index+1); //here to end
  int index2 = temp.indexOf(','); //next comma
  temp = temp.substring(0,index2); //grab number, do not include comma
  //grab integer from string
  int num = 0;
  for(int i = 0; i<temp.length(); i++)
  {
    char c = temp.charAt(i); //grab character
    if(i < temp.length() - 1)
    {
      //if there are more characters to read afterwords
      num = num + (c - '0'); //convert to integer
      num *= 10; //get ready for the next number
    }
    else
    {
      num += (c - '0'); 
    }
  }
  return num; 
}


