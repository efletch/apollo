//Driving
#include <Servo.h>
#include <HardwareSerial.h>


Servo servo1;
Servo servo2;
int speed = 100;



void drive(int driveSpeed, double distance){
  
  //long time = distance * 1000 / .033;//25.4s per meter, length of time we need to drive for
  unsigned long time = distance * 1000 / .0592;//16.9s per meter
  //unsigned long time = distance * 1000 / .0489;//20.5s per meter
  int tempIR = 0;
  int tempUltra = 0;

  //Serial.println(time);
  //Serial.println(timerTracker);
  //Serial.println(timer);

  int tempX = 0;
  int tempY = 0;
  
  servo1.attach(8);
  servo2.attach(9);
  
  timer = millis();
  timerTracker = timer;//Timestamp at start of drive function
  
  remainder = 0;//remainder is time left + idle time (time it took to turn)
  
  while (timer < (timerTracker + time + remainder))//Not yet handling turns taken
  {
    //objectCheck(time);//Check if there is an object and act
    //servo1.write(1525-driveSpeed);
    //servo2.write(1560+driveSpeed);
    
    servo1.write(64);
    servo2.write(180);
    
    timer = millis();
    //Let's say we sample every tenth of a meter, 1.69 seconds
    if (((timer - timerTracker) % 169) == 0)
   //if(((timer - timerTracker) % 205) == 0)//Remember millis
    {
      switch(orientationNum)
      {
        case 0:
          metersX+=1.0;
          break;
        case 1:
          //Need to increase X and Y .707, not sure how to do that and be able to send
          //.0707 meters traveled
          metersX+=0.707;
          metersY+=0.707;
          break;
        case 2:
          metersY+=1.0;//Moved a tenth of a meter
          break;
        case 3:
          //Need to increase Y .707, decrease X .707
          metersX-=0.707;
          metersY+=0.707;
          break;
        case 4:
          metersX-=1.0;//Moved a tenth of a meter
          break;
        case 5:
          //Need to decrease X and Y .707
          metersX-=0.707;
          metersY-=0.707;
          break;
        case 6:
          metersY-=1;//Moved a tenth of a meter
          break;
        case 7:
          //Need to increase X .707, decrease Y .707
          metersX+=0.707;
          metersY-=0.707;
          break;
      }
     tempIR = irSensor();
     tempUltra = ultrasonicSensor();
     objectCheck(time, tempIR, tempUltra);//Check if there is an object and act   
     xbeeCollect(tempIR, tempUltra);
     
    
     xbeeSend();
     
     //Serial.print("Meters X: %d Meters Y: %d\n",metersX,metersY);
      //Serial.print(metersX);
      //Serial.print("%t");
      //Serial.println(metersY);
    }
    timer = millis();
  }

  servo1.detach();
  servo2.detach();
  
  return;
  
}

void turn45Left(int driveSpeed){
  //servo1.attach(8);
  servo1.detach();
  servo2.attach(9);
  
  //for (int i = 0; i < 70; i++)//110
  //{
  //servo1.write(1530+driveSpeed);
  //servo2.write(1535+driveSpeed);//1550
  
  timer = millis();
  turnTimerTracker = timer;
  
  //while(timer<(turnTimerTracker+4500)){//1800ms for a turn
  while (timer<(turnTimerTracker+4000)){
  //servo1.write(135);
  //servo2.write(135);
  //servo1.write(100);
  servo2.write(110);
  
  timer = millis();
  }
  //printRaw2();
  //delay(20);
  //
//}
  
  //servo1.write(1530);
  //servo2.write(1550);
  //servo1.detach();
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
  
  for (int i = 0; i < 70; i++)
  {
  //servo1.write(1530-driveSpeed);
  //servo2.write(1535-driveSpeed);
  servo1.write(64);
  servo2.write(64);
  //printRaw2();
  delay(20);
  }
  
  servo1.detach();
  servo2.detach();
  
  if (orientationNum == 0)
    orientationNum = 7;
  else
    orientationNum--;
  
  /*for (int j = 0; j < 10; j ++)
  {  
    xbeeCollect();
    xbeeSend();

    delay(20);
  }*/
  
  return;
}
