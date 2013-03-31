//Driving
#include <Servo.h>


Servo servo1;
Servo servo2;
int speed = 100;

int metersX = 0;
int metersY = 0;
char *orientation = "+X";
uint8_t orientationNum = 0;

void drive(int driveSpeed, double distance){
  
  long time = distance * 1000 / .033;//25.4
  Serial.println(time);
  
  
  
  int tempX = 0;
  int tempY = 0;
  
  servo1.attach(8);
  servo2.attach(9);
  for (int i = 0; i < (time / 15); i++)
  {
  servo1.write(1527-driveSpeed);
  servo2.write(1560+driveSpeed);
  Serial.println(i);
  
  //printRaw2();
  //actually working in milimeters
  
  if (orientationNum == 0){
  metersX+=1;
  //orientationNum = 0;
  }
  else if (orientationNum == 1){
    
    
  }
  else if (orientationNum == 2){
  metersY+=1;
  //orientationNum = 1;
  }
  else if (orientationNum == 3){
    
  }
  else if (orientationNum == 4){
  metersX-=1;
  //orientationNum = 2;
  }
  else if (orientationNum == 5){
    
  }
  else if (orientationNum == 6){
  metersY-=1;
  //orientationNum = 3;
  }
  else if (orientationNum == 7){
    
  }
  
  //tempX = (int)metersX/5;
  //tempY = (int)metersY/5;
  
  //payload[0] = (uint8_t)tempX;
  //payload[1] = (uint8_t)tempY;
  if((metersX % 50 == 0) && (metersY % 50 == 0))
  {
  payload[0] = (uint8_t)(metersX / 50);
  payload[1] = (uint8_t)(metersY / 50);
  payload[2] = (uint8_t)irSensor();
  payload[3] = (uint8_t)ultrasonicSensor();
  payload[4] = orientationNum;
  xbeeSend();
  }
  
  //delay(15);
  }
  
  //servo1.write(1530);
  //servo2.write(1570);
  
  servo1.detach();
  servo2.detach();
  
  return;
  
}

void turn45Left(int driveSpeed){
  servo1.attach(8);
  servo2.attach(9);
  
  for (int i = 0; i < 200; i++)//175 200
  {
  servo1.write(1530+driveSpeed);
  servo2.write(1535+driveSpeed);//1550
  
  //printRaw2();
  delay(20);
  }
  
  //servo1.write(1530);
  //servo2.write(1550);
  servo1.detach();
  servo2.detach();
  
  if (orientationNum == 7)
    orientationNum = 0;
  else
    orientationNum++;
  
  return;
}

void turn45Right(int driveSpeed){
  servo1.attach(8);
  servo2.attach(9);
  
  for (int i = 0; i < 185; i++)
  {
  servo1.write(1530-driveSpeed);
  servo2.write(1535-driveSpeed);
  
  //printRaw2();
  delay(20);
  }
  
  servo1.detach();
  servo2.detach();
  
  if (orientationNum == 0)
    orientationNum = 7;
  else
    orientationNum--;
  
  for (int j = 0; j < 10; j ++)
  {  
    payload[0] = 0;
    payload[1] = 0;
    payload[2] = (uint8_t)irSensor();
    payload[3] = (uint8_t)ultrasonicSensor();
    payload[4] = orientationNum;
    xbeeSend();
    delay(20);
  }
  
  return;
}
