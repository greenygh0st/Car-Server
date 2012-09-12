#include <SPI.h>
#include <Ethernet.h>

// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network:
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(192,168,40,177);

// Initialize the Ethernet server library
// with the IP address and port you want to use
// (port 80 is default for HTTP):
EthernetServer server(1500); //for our purposes we will use 1500

void setup()
{
  //start serial library
  Serial.begin(9600);
  
  //set our pins
  //LED PINS
  pinMode(13, OUTPUT); //ONBOARD LED
  pinMode(9, OUTPUT); //RED LED 
  pinMode(8, OUTPUT); //GREEN LED
  
  //DRIVE PINS
  pinMode(2, OUTPUT); //FORWARD
  pinMode(3, OUTPUT); //BACK
  pinMode(5, OUTPUT); //LEFT
  pinMode(6, OUTPUT); //RIGHT
  
  //turn on RED LED
  digitalWrite(9, HIGH);
  
  //make sure greed led is off
  digitalWrite(8, LOW);
  
  // start the Ethernet connection and the server:
  Ethernet.begin(mac, ip);
  server.begin();
}

void loop()
{
  // listen for incoming clients
  EthernetClient client = server.available();
  if (client) {
    while (client.connected()) {
      //turn on green light/turn off red light
      digitalWrite(9, LOW);      
      digitalWrite(8, HIGH);
      if (client.available()) {
        Serial.println("Client Available--");
        //
        int driveMe = client.read();               
        //so right here is where we are going to tell the car what
        //to do based on the information being sent
        Serial.println("Char: " + driveMe);
        drive(driveMe);
      }
      //turn RED light back on if the client is not connected
      digitalWrite(9, HIGH);
      digitalWrite(8, LOW);      
    }
    //turn RED light back on if the client is not connected
    digitalWrite(9, HIGH);
    digitalWrite(8, LOW);
    //turn on red light/turn off green light    
    delay(1);//time to recieve data
    // close the connection: (after the client disconnects)
    client.stop();
  }
}
void drive(int dNumber)
{
  if (dNumber == 1)
  {
    digitalWrite(2, HIGH);
  }
  delay(2000);
  //end the function by reseting the pin outputs
  digitalWrite(2, LOW);
  digitalWrite(3, LOW);
  digitalWrite(5, LOW);
  digitalWrite(6, LOW);
}

