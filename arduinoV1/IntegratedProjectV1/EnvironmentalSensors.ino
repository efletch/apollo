//Environmental Sensors

const int sensorIR = 2;
const int pwPin = 5;
int arraysize = 9;
int rangevalue[] = { 0, 0, 0 ,0, 0, 0, 0, 0, 0};
float irValue, irInches, irCm, ultrasonicInches;
long pulse;

int irSensor(){
  irValue = analogRead(sensorIR);
  irInches = 4192.936 * pow(irValue,-0.935) - 3.937;
  irCm = 10650.08 * pow(irValue,-0.935) - 10;
  //delay(300);
  //Xbee.print("IR:  ");
  //Xbee.print(irInches);
  //Xbee.print("in, ");
  //Xbee.print(irCm);
  //Xbee.print("cm");
  //Xbee.println();
  return (int)irInches;
}

int ultrasonicSensor(){
  pinMode(pwPin, INPUT);
  for(int i = 0; i < arraysize; i++)
  {								    
    pulse = pulseIn(pwPin, HIGH);
    rangevalue[i] = pulse/147;
    delay(10);
  }
  isort(rangevalue,arraysize);
  //pulse = pulseIn(pwPin, HIGH);
  //ultrasonicInches = pulse/147;
  return (int)rangevalue[8];
  
}

void isort(int *a, int n){
// *a is an array pointer function
  for (int i = 1; i < n; ++i)
  {
    int j = a[i];
    int k;
    for (k = i - 1; (k >= 0) && (j < a[k]); k--)
    {
      a[k + 1] = a[k];
    }
    a[k + 1] = j;
  }
}

void objectCheck(unsigned long driveTime, int ir, int ultra){
  //int ir = irSensor();
  //int ultra = ultrasonicSensor();

  unsigned long remainingTime, timeStamp1, timeStamp2, idleTime;
  //if (((ir+ultra)/2) < 15)//if average is less than 15 inches, turn
   if (ir < 15)
    {
      timer = millis();
      timeStamp1 = timer;
      remainingTime = (timerTracker + driveTime) - timeStamp1;
      
      //turn45Right(50);
      turn45Left(50);
      delay(200);
      turn45Left(50);
      delay(200);
      turn45Left(50);
      delay(200);
      timer = millis();
      timeStamp2 = timer;
      
      idleTime = timeStamp2 - timeStamp1;
      
      remainder = remainingTime + idleTime;
      
      servo1.attach(8);
      servo2.attach(9);
    }

}
