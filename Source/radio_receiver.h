#ifndef RADIORECEIVE_H
#define RADIORECEIVE_H

#include <Energia.h>
#include <msp432.h>
#include <SPI.h>
#include <AIR430BoostFCC.h>

#define ADDRESS_LOCAL 0x01

class Radio_Receiver{
	
	public:
		struct rPacket{
			char from;
			char message[1];
		};

		struct rPacket messagePacket; //sending packet

		void startRadioReceive();

		char ReceiveMessage(int time);
};
#endif