//Driving
#include <Servo.h>


Servo servo1;
Servo servo2;
int speed = 100;



void drive(int driveSpeed, double distance){
  
  long time = distance * 1000000 / .033;//25.4s per meter, length of time we need to drive for
  timerTracker = timer;//Timestamp at start of drive function
  Serial.println(time);
  
  int tempX = 0;
  int tempY = 0;
  
  servo1.attach(8);
  servo2.attach(9);
  
  while (timer < (timerTracker + time))//Not yet handling turns taken
  {
    servo1.write(1527-driveSpeed);
    servo2.write(1560+driveSpeed);
    
    //Let's say we sample every tenth of a meter, 2.54 seconds
   if((timer - timerTracker) % 2540000)//Remember micros
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
        
     xbeeCollect();
     xbeeSend();
      
    }
  }

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
    xbeeCollect();
    xbeeSend();

    delay(20);
  }
  
  return;
}
