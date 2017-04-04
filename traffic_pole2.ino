#include <Arduino.h>
#include <SoftwareSerial.h>

void send_density(void);
void light_command(char xd);

SoftwareSerial neighbour(8,7);

int obPin1 = A5; //Input pins
int obPin2 = A4;
int obPin3 = A3;
int obPin4 = A2;

int ledRed = 13; //Output pins
int ledYel = 12;
int ledGre = 11;

int ob1 = 0; //obstacle status
int ob2 = 0;
int ob3 = 0;
int ob4 = 0;

int yellowset=0;
char s=0;
void setup() 
{
  pinMode(obPin1, INPUT);
  pinMode(obPin2, INPUT);
  pinMode(obPin3, INPUT);
  pinMode(obPin4, INPUT);
  pinMode(ledRed, OUTPUT);
  pinMode(ledYel, OUTPUT);
  pinMode(ledGre, OUTPUT);
  
  Serial.begin(9600);
  neighbour.begin(9600);
  
  digitalWrite(ledRed, HIGH);
  delay(500);
  digitalWrite(ledRed, LOW);
  digitalWrite(ledYel, HIGH);
  delay(500);
  digitalWrite(ledYel, LOW);
  digitalWrite(ledGre, HIGH);
  delay(500);
  digitalWrite(ledGre,LOW);
}
int obcount=0;
uint8_t br=0;
uint8_t bg=0;
uint8_t rstate = 0 ;
uint8_t gstate = 0;
char rec=0;
void loop() 
{
  ob1 = digitalRead(obPin1);
  ob2 = digitalRead(obPin2);
  ob3 = digitalRead(obPin3);
  ob4 = digitalRead(obPin4);
  obcount=0;
  if(ob1==1 || ob2==1 || ob3==1 || ob4==1)
  {
      //obcount=0;
      if(ob1==1) obcount++;
      if(ob2==1) obcount++;
      if(ob3==1) obcount++;
      if(ob4==1) obcount++;
  }

  if(neighbour.available()>0)
  {
      while(neighbour.available()>0)
      {
          char c = neighbour.read();
          Serial.write(c);
      }
  }

  if(br==1)
  { 
       rstate ^= 1;
       digitalWrite(ledRed,rstate);
       delay(200);
                //Blinking Red
  }
  if(bg==1)
  { 
       gstate ^= 1;
       digitalWrite(ledGre,gstate);
       delay(200);
                //Blinking Red
  }
}
void serialEvent()
{
    while(Serial.available()>0)
    {
        char data = Serial.read();
        /*if(data=='?')
        {
            send_density();
            neighbour.print('?');
        }*/
        if(data=='2')/////////////////////////////////////////////////////////////////
        {
            delay(10);
            data=Serial.read();
            //Serial.print("Read command: ");
            //Serial.print(data);
            //Serial.println();
            light_command(data);
        }
        else
        {
            neighbour.print(data);
        }
    }
}
void light_command(char xd)
{
     char cr = xd;
     //Serial.print("light command recieved: ");
     //Serial.print(cr);
     if(cr=='?')
     {
        send_density(); 
     }
     if(cr=='$' || cr=='R')
     {
         digitalWrite(ledGre,LOW);
         //digitalWrite(ledYel,LOW);
         digitalWrite(ledRed,HIGH);
         digitalWrite(ledYel,LOW);
         //s='r';
         //Serial.println("Red went on ");
         br=0;
         bg=0;
     }
     if(cr=='%' || cr=='G')
     {
         br=0;
         bg=0;
         digitalWrite(ledRed,LOW);
         //digitalWrite(ledYel,LOW);
         digitalWrite(ledGre,HIGH);
         digitalWrite(ledYel,LOW);
         //s='g';
     }
     if(cr=='^' || cr=='Y')
     {
            digitalWrite(ledYel,HIGH);
     }
     if(cr==']')
     {
            digitalWrite(ledYel,LOW);
     }
     if(cr=='.' || bg==1) //Blinking green
     {
          digitalWrite(ledRed,LOW);
          bg=1;
          br=0;
     }
     if(cr=='+'|| br ==1)
     {
          digitalWrite(ledGre,LOW);
          br=1;
          bg=0;      //Blinking Red
     }
}


void send_density(void)
{
        Serial.print('B');//////////////////////////////////////////////////////////////
        Serial.print(obcount);
        Serial.println();    
}
