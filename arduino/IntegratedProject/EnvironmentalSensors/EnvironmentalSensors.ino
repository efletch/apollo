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

