#include <SPI.h>
#include <Ethernet.h>

#include <Servo.h>

Servo driveservo;
Servo steeringservo;

// Enter a MAC address, IP address and Portnumber for your Server below.
// The IP address will be dependent on your local network:
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress serverIP(192,168,1,80);
int serverPort=8888;

//vars
const int RedLED = 5;
const int GreenLED = 2;
const int YellowLED = 3;

// Initialize the Ethernet server library
// with the IP address and port you want to use
EthernetServer server(serverPort);

void setup()
{
  // start the serial for debugging
  Serial.begin(9600);
  //setup LEDs
  pinMode(13, OUTPUT); //onboard LED
  pinMode(GreenLED, OUTPUT); //GREEN LED
  pinMode(RedLED, OUTPUT); //RED LED
  pinMode(YellowLED, OUTPUT);
  digitalWrite(GreenLED, LOW);
  digitalWrite(RedLED, LOW);
  digitalWrite(YellowLED, LOW);
  // start the Ethernet connection and the server:
  Ethernet.begin(mac, serverIP);
  server.begin();
  Serial.println("Car controller started...");//log
  digitalWrite(13, HIGH);
  //Connect to the cars servos
  /*
  Steering 1000 = Left, 1500 = Center, 2000 = right
  Drive 1000 = Forward, Reverse = ??
  */
  driveservo.attach(8);
  steeringservo.attach(9);
}

void loop()
{
  // listen for incoming clients
  EthernetClient client = server.available();
  if (client) {
    String clientMsg ="";
    while (client.connected()) {
      digitalWrite(GreenLED, HIGH);
      if (client.available()) {
        Serial.println("Client available...");
        char c = client.read();
        //Serial.print(c);
        if (c != '\n')
        {
          clientMsg+=c;//store the recieved chracters in a string
        }
        //if the character is an "end of line" the whole message is recieved
        if (c == '\n') {
          Serial.println("Message from Client: "+clientMsg+"\n");//print it to the serial
          client.println("You said:"+clientMsg);//modify the string and send it back
          digitalWrite(YellowLED, LOW);
          clientMsg.trim();
          if (clientMsg == "SS")
          {
            digitalWrite(RedLED, HIGH);
            Serial.println("ALL STOP COMMAND RECIEVED!");
            steeringservo.writeMicroseconds(1500);
            driveservo.writeMicroseconds(0);
          } else if (clientMsg == "FF") //forward straight
          {
            steeringservo.writeMicroseconds(1500);
            digitalWrite(RedLED, LOW);
            digitalWrite(YellowLED, HIGH);
            driveservo.writeMicroseconds(1000);
            Serial.println("FORWARD COMMAND RECIEVED!");
          } else if (clientMsg == "FL") //forward left
          {
            digitalWrite(RedLED, LOW);
            digitalWrite(YellowLED, HIGH);
            driveservo.writeMicroseconds(1000);
            steeringservo.writeMicroseconds(1000);
            Serial.println("FORWARD LEFT COMMAND RECIEVED!");
          } else if (clientMsg == "FR") //forward right
          {
            digitalWrite(RedLED, LOW);
            digitalWrite(YellowLED, HIGH);
            driveservo.writeMicroseconds(1000);
            steeringservo.writeMicroseconds(2000);
            Serial.println("FORWARD RIGHT COMMAND RECIEVED!");
          } else if (clientMsg == "LL") //left no forward drive
          {
            digitalWrite(RedLED, LOW);
            digitalWrite(YellowLED, HIGH);
            steeringservo.writeMicroseconds(1000);
            Serial.println("FORWARD RIGHT COMMAND RECIEVED!");
          } else if (clientMsg == "RR") //right no forward drive
          {
            digitalWrite(RedLED, LOW);
            digitalWrite(YellowLED, HIGH);
            steeringservo.writeMicroseconds(2000);
            Serial.println("FORWARD RIGHT COMMAND RECIEVED!");
          }
          else
          {
            Serial.println("UN-USED COMMAND RECIEVED!");
            digitalWrite(YellowLED, HIGH); 
          }
          clientMsg=""; //clear client msg
        }
      }
      steeringservo.writeMicroseconds(1500);
      driveservo.writeMicroseconds(0);
    }
    // give the Client time to receive the data
    delay(1);
    // close the connection:
    Serial.println("Client disconnected!");
    digitalWrite(GreenLED, LOW);
    digitalWrite(RedLED, LOW);
    digitalWrite(YellowLED, LOW);
    client.stop();
  }
}
