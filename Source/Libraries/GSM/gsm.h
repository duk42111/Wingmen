#ifndef GSM_H
#define GSM_H

#include <Energia.h>
#include <msp432.h>
//#include <msp430.h>
#define GSM_RST 8
//8 for 432


#define flushTime  15000
#define responseTime  15000
#define masterPhoneNumber "16144600335"

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
		void pingTheft(long interval);


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

				String grabAllResponse(long time);


		String receiveToChar(char c);


	private:
		int processNumSMSString(String response);
		String processGPSString(String gpsRet);
		//String grabAllResponse(long time);
		bool sendAndExpect(String toSend, String toGet);
};

#endif
