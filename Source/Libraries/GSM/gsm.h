#ifndef GSM_H
#define GSM_H

#include <Energia.h>
#include <msp432.h>
//#include <msp430.h>
#define GSM_RST 8

#define flushTime  15000
#define responseTime  15000
#define masterPhoneNumber "16144600335"
#define lovedOneNumber "5862918433"

class Gsm {

	public:

		/*
		 * SMS Functions
		 * Note: Using Energia String definition here
		 */
		bool sendSMS(String message, String phoneNumber);
		void readSMSFromList(int textNumber);
		void readSMSFromNumber(char* phoneNumber);
		int getNumSMS();
		String getSMSresponse();


		/*
		 * GPS Functions
		 *
		 */
		void pingGPS(long interval, String phoneNumber);


		/*
		 * Tools
		 *
		 */
		void startGSM(int baud);
		void startGPS();
		void stopGPS();
		bool checkForUnlock(int numberOfTexts);
		bool checkForLock(int numberOfTexts);
		void flushReceive(long time);
		void generateTempPass();


	private:
		bool sendAndExpect(String toSend, String toGet);
		int processNumSMSString(String response);
		String processGPSString(String gpsRet);
		String grabAllResponse(long time);
		String receiveToChar(char c);
};
#endif
