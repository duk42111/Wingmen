
#include <radio_receiver.h>
#include <AIR430BoostFCC.h>

/*
 * Receive only
 * Messages received will be from S for sender
 */
void Radio_Receiver::startRadioReceiver()
{
	Radio.begin(ADDRESS_LOCAL, CHANNEL_1, POWER_MAX);
	messagePacket.from = 'R'; //only set in case a message is sent from this
 	memset(messagePacket.message, 0 ,sizeof(messagePacket.message));
}

/*
 * This function will receive messages from the sender device.
 * The character that is sent is returned from this function.
 * Possible receptions include B for brake, H for hazard and S for stop.
 * Assumption is to be used as follows:
 * FOR __ALLOTED TIME__{ rec = receiveMessage}
 * if(rec == B) BRAKE else HAZARD;
 * LED settings for signaling receptions and sending should be done outside of this class
 * THIS FUNCTION WILL DO NOTHING IF NOTHING IS SENT
 */
char Radio_Receiver::ReceiveMessage(int time)
{
  //Turn on the receiver and listen for incoming data. Timeout after TIME seconds.
  //Recommended usage is TIME = 1000 for 1 seconds 
  // The receiverOn() method returns the number of bytes copied to rxData.
  //As in, if anything is received, return.
  //In use, set String ret = (char*)messagePacket.message and check that for equality
  if (Radio.receiverOn((unsigned char*)&messagePacket, sizeof(messagePacket), time) > 0)
  {
   		return messagePacket.message[0];
  }
}