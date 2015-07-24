#include <gsm.h>
Gsm gsm = Gsm();
void setup()
{
gsm.startGSM(9600);
delay(5000);
}

void loop()
{
  digitalWrite(BLUE_LED,HIGH);
  Serial.println("AT+CMGF=1\r");
  delay(1000);
  for(int i = 1; i<=30; i++)
  {
    String num;
    if(i < 10)
    {
     num = '00'+String(i);
    }
    else
    {
       num = '0'+String(i); 
    }
    String command = "AT+CMGD="+num+"\r";
    Serial.println(command);
    delay(2000);
  }
  digitalWrite(BLUE_LED,LOW);
  while(1){}
}
