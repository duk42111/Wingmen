#ifndef RADIO_H
#define RADIO_H

#include <Energia.h>
#include <msp432.h>
#include <SPI.h>
#include <AIR430BoostFCC.h>

#define ADDRESS_LOCAL 0x01

class Radio{
	
	public:
		struct sPacket{
			uint8_t from;
			uint8_t message[1];
		};

		struct sPacket messagePacket;

		void startRadio(int i);

		int recieveMessage();
		int sendMessage();
};
#endif