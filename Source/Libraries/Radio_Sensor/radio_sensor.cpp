
#include <radio_sensor.h>

/*
 * Send only
 * Messages received from this device will receive from S for sender
 */
void Radio_Sensor::startRadioSensor()
{
	Radio.begin(ADDRESS_LOCAL, CHANNEL_1, POWER_MAX);
	messagePacket.from = 'S';
	memset(messagePacket, 0 ,sizeof(messagePacket.message));

}

/*
 * This function will send the appropriate message to the recieving device.
 * When used with radio receiver, characters 'S', B' and 'H' will be expected
 * for Stop, Brake and Hazard, respectively.
 * This function only sends one message. Assumption is that it will be used in the following way:
 * WHILE(NO RESPONSE){ CONTINUALLY SEND}
 */
void Radio_Sensor::sendMessage(char action)
{
	messagePacket.message[0] = action;
	Radio.transmit(ADDRESS_REMOTE, (unsigned char*)*messagePacket, sizeof(messagePacket));
}