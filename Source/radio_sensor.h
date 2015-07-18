#ifndef RADIOSENSOR_H
#define RADIOSENSOR_H

#include <Energia.h>
#include <msp432.h>
#include <SPI.h>
#include <AIR430BoostFCC.h>

#define ADDRESS_LOCAL 0x02
#define ADDRESS_REMOTE 0x01

class Radio_Sensor{
	
	public:
		struct sPacket{
			char from;
			char message[1];
		};

		struct sPacket messagePacket; //sending packet

		void startRadioSensor();

		void sendMessage(char action);
};
#endif