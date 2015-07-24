#include "gsm.h"
/*
 * THIS LIBRARY USES THE HARDWARE SERIAL CONNECTION ON THE MSP430/432.
 * AS A RESULT, SERIAL DEBUGGING WILL NOT WORK UNLESS SERIAL1 IS USED FOR THE GSM
 */


/*
 * Constructor not needed
 */

/*** PRIVATE HELPERS ***/

/*
 * Private helper function to read up to a special character
 * This will flush the entire recieve buffer and search for the special character.
 * Recommended to use '$' or other "unique" equivalent to avoid errors.
 * This function will return the string between the two special characters.
 * Example "$hello$" will be searched for and hello will be returned if c = $
 */
 String Gsm::receiveToChar(char in)
 {
  	String readString = "";
  	bool done = false;
         while (Serial.available() || !done) {
        delay(10); 
	    char c = Serial.read(); 
    	if (c == in)
        {
          do{
            digitalWrite(RED_LED,HIGH);
            if(c != in){
            readString = readString + c;}
            c = Serial.read();
          }while(c != in);
          digitalWrite(RED_LED,LOW);
          done = true;
        }
   }
   return readString;
 }

/*
 * Private helper function used to process the GSM response when requesting the number of text messages stored in device.
 * Returns an int variable with the number of texts that are stored. Used to read most recent text and to check for new messages
 */
int Gsm::processNumSMSString(String response)
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

/*
 * Function used to grab everything coming into the serial monitor for a specific
 * amount of time, passed in milliseconds. 15000 ms is recommended (15 seconds).
 * RED led flashes to designate that a flush of serial receive buffer is happening.
 * Returns the response string.
 * To use, flushReceive should have been called before the desired command to allow this function
 * to have the right output.
 */
String Gsm::grabAllResponse(long time)
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


/*
 * Send a command and expect a response. Returns true if desired is in response
 */
bool Gsm::sendAndExpect(String toSend, String toGet)
{
  Serial.print(toSend);
  Serial.println("\r");
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

/*
 * Process GPS locate command into longitude and latitude string.
 * This cleaned string can be sent to be read easily.
 */
String Gsm::processGPSString(String gpsRet)
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

/*** START AND STOP FUNCTIONS ***/

/*
 * Begin the GSM by pulsing reset pin and setting to appropriate baud rate.
 * For MSP430, baud rate = 4800 is acceptable. For MSP432 please use 9600.
 */
void Gsm::startGSM(int baud){

	pinMode(GSM_RST,OUTPUT);
	digitalWrite(GSM_RST,HIGH);
	delay(100);
	digitalWrite(GSM_RST,LOW);
	delay(100);
	digitalWrite(GSM_RST,HIGH);
	delay(300);

	Serial.begin(baud);

	//send 3 times to ensure activation
    Serial.println("AT\r");
    Serial.println("AT\r");
    Serial.println("AT\r");
    Serial.println("ATE0\r"); //stop echos
}

/*
 * Prepare the GPS for reading by powering it on and reseting it. 
 * Will need to be turned off afterwards.
 */
void Gsm::startGPS()
{
  flushReceive(flushTime);
  //sendAndExpect("AT+CGPSPWR=1","OK");
  Serial.println("AT+CGPSPWR=1\r");
  delay(2000); //let it process
  Serial.println("AT+CGPSRST=0\r");
  Serial.println("WAIT=60\r");
  //delay(500);
}

/*
 * Power off the GPS
 */
 void Gsm::stopGPS()
 {
 	Serial.println("AT+CPGPSPWR=0\r");
  delay(2000);
 }

/*** GPS TOOLS ***/

/*
 * This function pings for theft or crash. If a theft/crash is occured, this function will send 
 * a location text with latitiude and longitude to the passed phone number at the given
 * interval. 
 * Recommended interval is 60000 ms or 1 minute
 * Best use : while(!recovereed){pingGPS();}
 */
void Gsm::pingGPS(long interval, String phoneNumber)
{
  int time_start = millis();
  flushReceive(flushTime); //must flush first time try to ping
  while(millis()-time_start < interval)
  {
    Serial.println("AT+CGPSINF=0\r");
    String str = grabAllResponse(responseTime);
    String message = processGPSString(str);
    this->sendSMS(message,phoneNUmber);
    flushReceive(flushTime);
    delay(1000);
  } 
}


/*** GSM TOOLS ***/

/*
 * Message should be <= 140 characters.
 * Phone number of the format 1 ### ### #### (without spaces)
 */
bool Gsm::sendSMS(String message, String phoneNumber){
	Serial.println("AT\r");
	delay(1000);
	Serial.println("AT+CMGF=1\r");
	delay(1000);
	Serial.print("AT+CMGS=\"");
	delay(1000);
	Serial.print(phoneNumber);
	delay(1000);
	Serial.println("\"\r");
	delay(1000);
	Serial.println(message);
	delay(1000);
	Serial.println((char)26);
}


/*
 * Returns int with the number of texts currently stored. Used to get most recent text and to check for incoming messages
 */
int Gsm::getNumSMS(){
	flushReceive(flushTime);
    Serial.println("AT+CPMS?\r");
    String response = grabAllResponse(responseTime);
    return processNumSMSString(response);
}

/*
 * Send the command to read the latest text and see if the UNLOCK has been sent
 * *** OPTION to only expect from master? ***
 */
 bool Gsm::checkForUnlock(int numberOfTexts)
 {
	flushReceive(flushTime);
    String number = String(numberOfTexts);
    String command = "AT+CMGR="+number+"\r";
    Serial.print(command);
    delay(1000);
    String ret = receiveToChar('$');
    if(ret.indexOf("UNLOCK") != -1)
    	return true;
    return false;
 }

/*
 * Check for LOCK command
 * Eseentially checking for the word LOCK in the most recent text
 */
 bool Gsm::checkForLock(int numberOfTexts)
 {
 	flushReceive(flushTime);
    String number = String(numberOfTexts);
    String command = "AT+CMGR="+number+"\r";
    Serial.print(command);
    delay(1000);
    String ret = receiveToChar('$');
    if(ret.indexOf("LOCK") != -1)
    	return true;
    return false;
 }

/*
 * Flush the receive buffer in preparation for a read. Best used before desired command
 * is called. Recommended time is 15000 ms or 15 seconds.
 * Green LED signals flushing is occuring.
 * WARNING: The received info is not retained.
 */
 void Gsm::flushReceive(long time)
 {
  digitalWrite(GREEN_LED,HIGH);
  int time_start = millis();
  while(Serial.available() > 0 || (millis() - time_start < time))
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
