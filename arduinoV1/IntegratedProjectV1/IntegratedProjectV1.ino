#include <XBee.h>

XBee xbee = XBee();

void setup() {  
  pinMode(8, OUTPUT);
  pinMode(9,OUTPUT);
  
  Serial.begin(9600);
  xbee.setSerial(Serial);
  
  delay(10000);

}

void loop(){
  turn45Right(50);
//drive(50,.1);
delay(3000);
turn45Right(50);
//drive(50,.1);
delay(3000);
turn45Right(50);
//drive(50,.1);
delay(3000);
turn45Right(50);
//drive(50,.1);
delay(3000);
turn45Right(50);
//drive(50,.1);
delay(3000);
turn45Right(50);
//drive(50,.1);
delay(3000);
turn45Right(50);
//drive(50,.1);
delay(3000);
turn45Right(50);
//drive(50,.1);
delay(3000);
turn45Right(50);//9
//drive(50,.1);
delay(3000);
turn45Right(50);//10
//drive(50,.1);
delay(50000);

}
