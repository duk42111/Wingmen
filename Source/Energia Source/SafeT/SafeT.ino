/*
 //    														  
 //	The Wingman SafeT 								      
 //													          
 // This code uses an MSP430 and 3 NexFET N-channel		      
 //	MOSFETs to drive 45 LEDs.  It recieves accelerometer      
 //	and GPS data from a MSP432 via the C110L wireless          
 //	boosterpack. 											  
 //															  
 //	From this data, the MCSP430 determines:					  
 //															  
 //	(1) When to activate the LEDs (time and location based).  
 //	(2) How fast they should strobe.						  
 //	(3) Which side should be brighter.						  
 //	(4) When to activate the brake lights.					  
 //														      
 //	Created by SpEcTrUm										  
 //  06/20/15											      
 
 CHANGE LOG:
 
 06/20/15 created file, began LED coding and layout -- Billy
 06/22/15 updated code, sampled "simultaneous" blinking of LEDs -- Billy
 06/25/15 improved readability
 
 */

#include <radio_receiver.h>


// Declerations

const int GATE_PIN_L = P1_2; // Yellow LEFT SIDE -- This port allows for PWM
const int GATE_PIN_R = P2_1; // Yellow RIGHT SIDE -- This port allows for PWM
const int GATE_PIN_RED = P1_4; // RED for Braking -- This port is DIGITAL ONLY

const int interval_strobe = 50; // "Short" strobes
const int interval_flash = 500; // "Long" flashes
const int burst_duration = 100; // How long LEDs are "ON"

const int buttonPin = PUSH2;

int strobe_state = LOW; // Used for state changes
int flash_state = LOW; // Used for state changes

int brightness = 100; // Sets normal brightness (100 out of 255)

unsigned long current_strobe = 0;
unsigned long previous_strobe = 0; // Allows for strobing without delay
unsigned long previous_flash = 0; // Allows for strobing without delay

int buttonState = LOW; // For DEBUGGING purposes only

Radio_Receiver radio = Radio_Receiver();

void setup()
{	
  pinMode(GATE_PIN_L, OUTPUT);
  pinMode(GATE_PIN_R, OUTPUT);
  pinMode(GATE_PIN_RED, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);
}

void loop()
{	
  current_strobe = millis(); 
  SafeT_Flash();
  SafeT_Strobe();
  Let_There_Be_Light();
} 

void SafeT_Flash()
{
  if(flash_state == LOW)
  {
    if(current_strobe - previous_flash >= interval_flash)
    {
      flash_state = brightness;
      previous_flash += interval_flash;
    }
  }
  else {
    if(current_strobe - previous_flash >= burst_duration)
    {
      flash_state = LOW;
      previous_flash += burst_duration;
    }
  }
}

void SafeT_Strobe()
{
  if(strobe_state == LOW)
  {
    if(current_strobe - previous_strobe >= interval_strobe)
    {
      strobe_state = brightness;
      previous_strobe += interval_strobe;
    }
  }
  else {
    if(current_strobe - previous_strobe >= burst_duration)
    {
      strobe_state = LOW;
      previous_strobe += burst_duration;
    }
  }
}

void Let_There_Be_Light()
{
    //ensure this is always reset
    String switcher = "default";
    if(radio.ReceiveMessage(1000))
    {
      switcher = (char*)radio.messagePacket.message;
    }
  
    if(switcher == "B")
    {
      digitalWrite(GATE_PIN_RED, HIGH);
    }
    else if(switcher == "H")
    {
      digitalWrite(GATE_PIN_RED, LOW);
      analogWrite(GATE_PIN_L, strobe_state);
      analogWrite(GATE_PIN_R, strobe_state);
    }
    else
    {
      digitalWrite(GATE_PIN_RED, LOW);
      analogWrite(GATE_PIN_R, flash_state);
      analogWrite(GATE_PIN_R, flash_state);
    }
    
    //always have yellow lights doing some type of flash
    digitalWrite(GATE_PIN_R, HIGH);
    digitalWrite(GATE_PIN_L, HIGH);
}
