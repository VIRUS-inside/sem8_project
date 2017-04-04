#include <Arduino.h>
//#include <TimerOne.h>
#include <SoftwareSerial.h>
//void timerIsr();
void go_green_blink(int lgid);
void go_red_blink(int rgid);
void get_density(void);
void go_green(int i);
void go_yellow_on(int j);
void go_yellow_off(int j);
void go_red(int k);

void emergency_request(int f);

SoftwareSerial sli(2,3); //serial lights interface
SoftwareSerial ser(4,5); //Serial emergency request

int td1=0,td2=0,td3=0,td4=0;
char erd=0;
uint8_t green_required_id=0;
uint8_t gx=0;
uint8_t last_green_id=0;

uint8_t active_green_id=0;
String inputString = "";
void setup() 
{
  Serial.begin(9600);
  sli.begin(9600);
  ser.begin(9600);
  delay(3000);
  ser.listen();
  delay(1000);
  ser.end();
  delay(1000);
  sli.listen();
  Serial.println("Wait for 5 seconds...");
  delay(5000);
  Serial.println("ALL SYSTEMS ONLINE.");
  //Timer1.initialize(1000000); //1 sec = 1,000,000
  //Timer1.attachInterrupt(timerIsr); // attach the service routine here
}

void loop() 
{   
    Serial.println("start==============================================================");
    get_density(1);
    delay(100);
    get_density(2);
    delay(100);
    get_density(3);
    delay(100);
    get_density(4);
    delay(100);
    Serial.println("Traffic density:");//////////////////////////
    Serial.println(td1);////////////////////////////
    Serial.println(td2);//////////////////////////////
    Serial.println(td3);///////////////////////////////
    Serial.println(td4);/////////////////////////////

    //erd=Serial.read();
    /*if(Serial.available()>0)
    {
        erd=Serial.read();
        if(erd=='/')
        {
            green_required_id = Serial.parseInt();
            Serial.print("Emergency clearance request acknowleged."); /////////////////////////////
            Serial.print("with Request route ID: ");
            Serial.print(green_required_id);
        }
    }*/

    if(td1>0 || td2>0 || td3>0 || td4>0) //if non zero found then only execute this
    {    //finding green_required_id
        if(td1>td2 && td1>td3 && td1>td4)
        {
            green_required_id=1;
            Serial.println("highest density detected on Route 1.");/////////////////////
        }
        else if(td2>td1 && td2>td3 && td2>td4)
        {
            green_required_id=2;
            Serial.println("highest density detected on Route 2.");/////////////////////
        }
        else if(td3>td1 && td3>td2 && td3>td4)
        {
            green_required_id=3;
            Serial.println("highest density detected on Route 3.");/////////////////////
        }
        else if(td4>td1 && td4>td2 && td4>td3)
        {
            green_required_id=4;
            Serial.println("highest density detected on Route 4.");/////////////////////
        }
    }
    else
    {
        Serial.println("Low traffic density: Normal functioning.");//////////////////////////////
        if(gx>4) gx = 1;
        if(gx==last_green_id) gx++;
        //if(gx==0) gx=1;

        green_required_id = gx;
        Serial.print("Normal functioning with green ID:");////////////////////////
        Serial.print(green_required_id);//////////////////////
        Serial.println();////////////////////////
        gx++;
    }
    if(green_required_id==0) green_required_id=1; 
    go_yellow_on(last_green_id); //go_yellow_on
    go_yellow_on(green_required_id);
    
    Serial.print("Yellow ON : ");///////////////////////
    Serial.print(last_green_id);////////////////////////
    Serial.print("        ");////////////////////////
    Serial.print(green_required_id);//////////////////////
    Serial.println();/////////////////////////
    Serial.println("Blinking ON for 6 sec same ID.");//////////////////////
    go_red_blink(green_required_id);
    go_green_blink(last_green_id);
    
    delay(6000);
    go_yellow_off(last_green_id);//go_yellow_off
    go_yellow_off(green_required_id);

    Serial.print("Yellow OFF : ");///////////////////////
    Serial.print(last_green_id);////////////////////////
    Serial.print("        ");////////////////////////
    Serial.print(green_required_id);//////////////////////
    Serial.println();/////////////////////////
    
    for(uint8_t z=1; z<5; z++)
    {
        if(z!=green_required_id)
        {
            go_red(z);
            Serial.println();//////////////////////////////
        }
    }
    go_green(green_required_id);
    Serial.print("End Result: Going Green ON ID: ");//////////////////////////////////
    Serial.print(green_required_id);///////////////////////////
    Serial.println();////////////////////////////////
    last_green_id=green_required_id;
    Serial.print("waiting====>");
    for(int tm=0; tm<24; tm++)
    {
        delay(1000);
        char x;
        if(digitalRead(8)==HIGH)
        {
            sli.end();
            delay(500);
            ser.listen();
            delay(500);
            x=ser.read();//<<<<<<<<<<<<<<<<<<<<<IMPORTANT 
            if(x=='/')
            {
                Serial.println("Emergency Request found!");
                char b=ser.parseInt();
                emergency_request(b);
            }
            else
            {
              ser.end();
              delay(500);
              sli.listen();
              delay(500);
              Serial.println("False Emergency Triggered! ");
            }
        }
        Serial.print("|");
    }
    Serial.println();
    Serial.println("end================================================================");
    Serial.println();
    Serial.println();
    Serial.println();
}
char c=0;
int rd;
void get_density(int id)
{
    //Serial.print("Sending request to ");
    //Serial.print(id);
    //Serial.println();
    int i=id;
    delay(10);
    if(id==1)
    {
          sli.println("1?");
              delay(20);
              if(sli.available()>0)
              {
                    Serial.println("Reading data from A");
                    while(sli.available()>0)
                    {
                      c = sli.read();
                      //Serial.write(c);
                      if(c=='A')
                      {
                          delay(10);
                          td1=sli.parseInt();
                          Serial.print("Density from A: ");
                          Serial.print(td1);
                          Serial.println();
                      }
                    }
              }
              else
              {
                Serial.println("No data available from A");
              }
    }
    if(id==2)
    {
          sli.println("2?");
              delay(20);
              if(sli.available()>0)
              {
                    
                    Serial.println("Reading data from B");
                    while(sli.available()>0)
                    {
                      c = sli.read();
                     // Serial.write(c);
                      if(c=='B')
                      {
                          delay(10);
                          td2=sli.parseInt();
                          Serial.print("Density from B: ");
                          Serial.print(td2);
                          Serial.println();
                      }
                    }
              }
              else
              {
                Serial.println("No data available from B");
              }
    }
    if(id==3)
    {
          sli.println("3?");
              delay(20);
              if(sli.available()>0)
              {
                
                    Serial.println("Reading data from C");
                    while(sli.available()>0)
                    {
                      c = sli.read();
                      //Serial.write(c);
                      if(c=='C')
                      {
                          delay(10);
                          td3=sli.parseInt();
                          Serial.print("Density from C: ");
                          Serial.print(td3);
                          Serial.println();
                      }
                    }
              }
              else
              {
                Serial.println("No data available from C");
              }
    }
    if(id==4)
    {
          sli.println("4?");
              delay(20);
              if(sli.available()>0)
              {
                    Serial.println("Reading data from D");
                    //Serial.println();
                    while(sli.available()>0)
                    {
                      c = sli.read();
                      //Serial.write(c);
                      if(c=='D')
                      {
                          delay(10);
                          td4=sli.parseInt();
                          Serial.print("Density from D: ");
                          Serial.print(td4);
                          Serial.println();
                      }
                    }
              }
              else
              {
                Serial.println("No data available from D");
              }
    }
}


void go_green(int i)
{
    active_green_id = i;
    Serial.print("GREEN ON : ");////////////////////////
    Serial.print(i);/////////////////////////////
    Serial.println();/////////////////////////
    switch(i)
    {
        case 1:
        sli.println("1%");
        break;
        
        case 2:
        sli.println("2%");
        break;
        
        case 3:
        sli.println("3%");
        break;
        
        case 4:
        sli.println("4%");
        break;

        default:
        break;
    }
}
void go_yellow_on(int j)
{
    
    /*Serial.print("YELLOW ON : ");////////////////////////
    Serial.print(j);////////////////////////////
    Serial.print("\n");*/
    switch(j)
    {
        case 1:
        sli.println("1^");
        //Serial.println("Go Yellow on 1");/////////////////////////
        break;
        
        case 2:
        sli.println("2^");
        //Serial.println("Go Yellow on 2");////////////////////////
        break;
        
        case 3:
        sli.println("3^");
        //Serial.println("Go Yellow on 3");//////////////////////
        break;
        
        case 4:
        sli.println("4^");
        //Serial.println("Go Yellow on 4");////////////////////////
        break;

        default:
        break;
    }
}
void go_yellow_off(int jo)
{
    
    /*Serial.print("YELLOW ON : ");////////////////////////
    Serial.print(j);////////////////////////////
    Serial.print("\n");*/
    switch(jo)
    {
        case 1:
        sli.println("1]");
        //Serial.println("Go Yellow on 1");/////////////////////////
        break;
        
        case 2:
        sli.println("2]");
        //Serial.println("Go Yellow on 2");////////////////////////
        break;
        
        case 3:
        sli.println("3]");
        //Serial.println("Go Yellow on 3");//////////////////////
        break;
        
        case 4:
        sli.println("4]");
        //Serial.println("Go Yellow on 4");////////////////////////
        break;

        default:
        break;
    }
}

void go_red(int k)
{
    Serial.print("RED ON : ");////////////////////////
    Serial.print(k);////////////////////////////
    //Serial.print("\n");
    switch(k)
    {
        case 1:
        sli.println("1$");
        //Serial.println("Go Red on 1");
        break;
        
        case 2:
        sli.println("2$");
        //Serial.println("Go Red on 2");
        break;
        
        case 3:
        sli.println("3$");
        //Serial.println("Go Red on 3");
        break;
        
        case 4:
        sli.println("4$");
        //Serial.println("Go Red on 4");
        break;

        default:
        break;
    }
}

void go_green_blink(int lgid)
{
    Serial.print("Blinking GREEN : ");//////////////////////////
    Serial.print(lgid);//////////////////////////
    Serial.println();//////////////////////////

    switch(lgid)
    {
        case 1:
        sli.println("1.");
        break;
        
        case 2:
        sli.println("2.");
        break;
        
        case 3:
        sli.println("3.");
        break;
        
        case 4:
        sli.println("4.");
        break;

        default:
        break;
    }
}

void go_red_blink(int rgid)
{

    Serial.print("Blinking RED : ");//////////////////////////
    Serial.print(rgid);//////////////////////////
    Serial.print("\n");//////////////////////////
    switch(rgid)
    {
        case 1:
        sli.println("1+");
        break;
        
        case 2:
        sli.println("2+");
        break;
        
        case 3:
        sli.println("3+");
        break;
        
        case 4:
        sli.println("4+");
        break;

        default:
        break;
    }
}
void emergency_request(int f) 
{
        
        Serial.println("Emergency_Route_Start>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>");
        int errid=f;
        Serial.print("Emergency code Route ID Recieved:");
        Serial.print(errid);
        Serial.println();
        uint8_t temp1 = green_required_id;
        go_green_blink(temp1);
        go_yellow_on(temp1);
        go_yellow_on(errid);
        go_red_blink(errid); //emergency request route id(errid);
        delay(5000); //emergency request route id
        Serial.println("Emergency_Route_End>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>");
        go_green(errid);
        go_red(temp1);
        delay(25000); //emergency request route id;
        
        go_green_blink(errid);
        go_red_blink(temp1);
        go_yellow_on(temp1);
        go_yellow_on(errid);
        delay(5000);
        go_yellow_off(errid);
        go_yellow_off(temp1);
        go_red(errid);
        go_green(temp1);
        ser.end();
        delay(1000);
        sli.listen();
        delay(1000);
}
