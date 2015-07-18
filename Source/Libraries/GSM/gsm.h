#ifndef GSM_H
#define GSM_H

#include <Energia.h>
#include <msp432.h>
//#include <msp430.h>
#define GSM_RST 8
//8 for 432


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


		/*
		 * GPS Functions
		 *
		 */
		void getLocation();


		/*
		 * Tools
		 *
		 */
		void startGSM(int baud);
		void unlockBikeMaster();
		void unlockBikeTemp();
		void generateTempPass();
};

#endif
