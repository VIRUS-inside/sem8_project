#define BLYNK_PRINT Serial


#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#define ON 255
#define OFF 0
// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "1fd1deb105694be2be6e2f5edf898efa";

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "NETGEAR05";
char pass[] = "jaggedflute336";
int e_state=0;
int id=0;
void setup()
{
  // Debug console
  BLYNK_PRINT.begin(9600);
  BLYNK_PRINT.println("Emergency Recieve Ready.");
  //Blynk.begin(auth, ssid, pass);
  // You can also specify server:
  //Blynk.begin(auth, ssid, pass, "blynk-cloud.com", 8442);
  Blynk.begin(auth, ssid, pass, IPAddress(188,166,206,43), 8442);
  pinMode(D5,OUTPUT);
  pinMode(D0,OUTPUT);
}
BLYNK_WRITE(V0) //Function to get status from BLYNK widget to PHOTON
{
    int state0 = param.asInt();
    if (state0)
    {
        Blynk.virtualWrite(V4, ON);
        id=1;
        e_state=1;
    }
    else Blynk.virtualWrite(V4, OFF); 
}

BLYNK_WRITE(V1) //Function to get status from BLYNK widget to PHOTON
{
    int state1 = param.asInt();
    if (state1)
    {
        
        Blynk.virtualWrite(V5, ON);
        id=2;
        e_state=1; 
    }
    else Blynk.virtualWrite(V5, OFF); 
}

BLYNK_WRITE(V2) //Function to get status from BLYNK widget to PHOTON
{
    int state2 = param.asInt();
    if (state2)
    {
        Blynk.virtualWrite(V6, ON);
        id=3;
        e_state=1; 
    } 
    else Blynk.virtualWrite(V6, OFF);
}

BLYNK_WRITE(V3) //Function to get status from BLYNK widget to PHOTON
{
    int state3 = param.asInt();
    if (state3)
    {
        Blynk.virtualWrite(V7, ON);
        id=4; 
        e_state=1;
    }
    else Blynk.virtualWrite(V7, OFF);
}
void loop()
{
  Blynk.run();
  if(e_state==1 && id!=0)
  {
      digitalWrite(D5,HIGH);
      digitalWrite(D0,HIGH);
      delay(1000);
      for(int x=0; x<5; x++)
      {
          Serial.print("/");
          Serial.print(id);
          delay(200);
      }
      e_state=0;  
  }
  else
  {
    
      digitalWrite(D5,LOW);
      digitalWrite(D0,LOW);
  }
  // You can inject your own code or combine it with other sketches.
  // Check other examples on how to communicate with Blynk. Remember
  // to avoid delay() function!
}
