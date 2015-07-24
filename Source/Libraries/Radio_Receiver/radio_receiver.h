#ifndef RADIORECEIVE_H
#define RADIORECEIVE_H

#include <Energia.h>
#include <msp430.h>


#define ADDRESS_LOCAL 0x01

class Radio_Receiver{
	
	public:
		struct rPacket{
			char from;
			char message[1];
		};

		struct rPacket messagePacket; //sending packet

		void startRadioReceiver();

		char ReceiveMessage(int time);
};
#endif