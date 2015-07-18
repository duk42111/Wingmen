#include "gsm.h"
/*
 * THIS LIBRARY USES THE HARDWARE SERIAL CONNECTION ON THE MSP430/432.
 * AS A RESULT, SERIAL DEBUGGING WILL NOT WORK UNLESS SERIAL1 IS USED FOR THE GSM
 *
 *
 *
 *
 *
 */


/*
 * Constructor not needed
 */


/*
 * Begin the GSM by pulsing reset pin and setting to appropriate baud rate.
 * For MSP430, baud rate = 4800 is acceptable. For MSP432 please use 9600.
 */
void Gsm::startGSM(int baud){

	//CURRENTLY THERE IS NO WAY TO CHECK IF IT TURNED ON....SERIAL READ OK?

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
}

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
 * Returns the number of texts currently stored. Used to get "most recent"
 * *** How to delete if fill up? ***
 */
int Gsm::getNumSMS(){
	Serial.println("AT+CMFG=1\r");
	delay(1000);
	Serial.println("AT+CPSMS?\r");
	delay(1000);
}



