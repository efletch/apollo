#include <Wire.h>
#include <Servo.h>
#include <XBee.h>

#define gyroAddress 0x68
#define adxlAddress 0x53

//XBee Setup
XBee xbee = XBee();
uint8_t payload[] = {0, 0, 0, 0, 0};
XBeeAddress64 addr64 = XBeeAddress64(0x0013a200, 0x408d6448);
Tx64Request tx = Tx64Request(addr64, payload, sizeof(payload));
TxStatusResponse txStatus = TxStatusResponse();

//SENSORS
const int sensorIR = 2;
const int pwPin = 5;
int arraysize = 1;
float irValue, irInches, irCm, ultrasonicInches;
long pulse;


Servo servo1;
Servo servo2;
int speed = 100;

int metersX = 0;
int metersY = 0;
char *orientation = "+X";

double zeroValue[5] = {-200, 44, 660, 52.3, -18.5}; // Found by experimenting

double calibrated[5] = {0, 0, 0, 0, 0};

int calibrateIterations = 50;
int geforceIterations = 10;

/* All the angles start at 180 degrees */
double gyroXangle = 180;
double gyroYangle = 180;

double compAngleX = 180;
double compAngleY = 180;

unsigned long timer;

void setup() {  
  pinMode(8, OUTPUT);
  pinMode(9,OUTPUT);
  
  /*servo1.attach(8);
  servo2.attach(9);
  servo1.write(1530);
  servo2.write(1550);*/
  
  //Serial.begin(115200);
  Serial.begin(9600);
  xbee.setSerial(Serial);
  /*Wire.begin();
  
  i2cWrite(adxlAddress,0x31,0x09); // Full resolution mode
  i2cWrite(adxlAddress,0x2D,0x08); // Setup ADXL345 for constant measurement mode
  
  i2cWrite(gyroAddress,0x16,0x1A); // this puts your gyro at +-2000deg/sec  and 98Hz Low pass filter
  i2cWrite(gyroAddress,0x15,0x09); // this sets your gyro at 100Hz sample rate
*/
  timer = micros();
  delay(10000);

  //calibrate();
}

/*void loop() {    
  double gyroXrate = -(((double)readGyroX()-zeroValue[3])/14.375);
  gyroXangle += gyroXrate*((double)(micros()-timer)/1000000); // Without any filter
  
  double gyroYrate = (((double)readGyroY()-zeroValue[4])/14.375);
  gyroYangle += gyroYrate*((double)(micros()-timer)/1000000); // Without any filter
  
  double accXangle = getXangle();
  double accYangle = getYangle();
  
  compAngleX = (0.93*(compAngleX+(gyroXrate*(double)(micros()-timer)/1000000)))+(0.07*accXangle);
  compAngleY = (0.93*(compAngleY+(gyroYrate*(double)(micros()-timer)/1000000)))+(0.07*accYangle);
  
  double xAngle = kalmanX(accXangle, gyroXrate, (double)(micros() - timer)); // calculate the angle using a Kalman filter
  double yAngle = kalmanY(accYangle, gyroYrate, (double)(micros() - timer)); // calculate the angle using a Kalman filter
  
  timer = micros();
  
  /* print data to processing */
  /*Serial.print(gyroXangle);Serial.print("\t");
  Serial.print(gyroYangle);Serial.print("\t");
  
  Serial.print(accXangle);Serial.print("\t");
  Serial.print(accYangle);Serial.print("\t");
  
  Serial.print(compAngleX);Serial.print("\t");  
  Serial.print(compAngleY); Serial.print("\t"); 
  
  Serial.print(xAngle);Serial.print("\t");
  Serial.print(yAngle);Serial.print("\t");
   
  Serial.print("\n");*/
  
 // delay(10);
//}

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
  /*for(int i = 0; i < arraysize; i++)
  {								    
    pulse = pulseIn(pwPin, HIGH);
    rangevalue[i] = pulse/147;
    delay(10);
  }
  */
  pulse = pulseIn(pwPin, HIGH);
  ultrasonicInches = pulse/147;
  return (int)ultrasonicInches;
  
}

void xbeeSend(){
  
 xbee.send(tx);
  
    // after sending a tx request, we expect a status response
    // wait up to 5 seconds for the status response
    if (xbee.readPacket(300)) {
        // got a response!

        // should be a znet tx status            	
    	if (xbee.getResponse().getApiId() == TX_STATUS_RESPONSE) {
    	   xbee.getResponse().getZBTxStatusResponse(txStatus); 
        }
    }
  
  
}
void drive(int driveSpeed, int time){
  
  uint8_t orientationNum = 0;
  
  int tempX = 0;
  int tempY = 0;
  
  servo1.attach(8);
  servo2.attach(9);
  for (int i = 0; i < (time / 15); i++)
  {
  servo1.write(1527-driveSpeed);
  servo2.write(1560+driveSpeed);
  
  //printRaw2();
  //actually working in milimeters
  if (orientation == "+X"){
  metersX+=1;
  orientationNum = 0;
  }
  else if (orientation == "+Y"){
  metersY+=1;
  orientationNum = 1;
  }
  else if (orientation =="-X"){
  metersX-=1;
  orientationNum = 2;
  }
  else if (orientation =="-Y"){
  metersY-=1;
  orientationNum = 3;
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
  
  delay(15);
  }
  
  //servo1.write(1530);
  //servo2.write(1570);
  
  servo1.detach();
  servo2.detach();
  
  return;
  
}

void turnRight(int driveSpeed){
  servo1.attach(8);
  servo2.attach(9);
  
  for (int i = 0; i < 310; i++)
  {
  servo1.write(1530-driveSpeed);
  servo2.write(1535-driveSpeed);
  
  //printRaw2();
  delay(20);
  }
  
  //servo1.write(1530);
  //servo2.write(1550);
  servo1.detach();
  servo2.detach();
  
  if (orientation == "+X")
  orientation = "-Y";
  else if (orientation == "+Y")
  orientation = "+X";
  else if (orientation =="-X")
  orientation = "+Y";
  else if (orientation =="-Y")
  orientation = "-X";
  
  return;
}

void turnLeft(int driveSpeed){
  servo1.attach(8);
  servo2.attach(9);
  
  for (int i = 0; i < 310; i++)
  {
  servo1.write(1530+driveSpeed);
  servo2.write(1550+driveSpeed);
  
  //printRaw2();
  delay(20);
  }
  
  //servo1.write(1530);
  //servo2.write(1550);
  servo1.detach();
  servo2.detach();
  
  if (orientation == "+X")
  orientation = "+Y";
  else if (orientation == "+Y")
  orientation = "-X";
  else if (orientation =="-X")
  orientation = "-Y";
  else if (orientation =="-Y")
  orientation = "+X";
  
  return;
}


void printRaw(){
  
   for(int i = 0; i<250; i++)
    {
      Serial.print(readAccX());Serial.print("\t");
      Serial.print(readAccY());Serial.print("\t");
      Serial.print(readAccZ());Serial.print("\n");
      delay(20);
    } 
}

void printRaw2(){
  
      Serial.print(readAccX());Serial.print("\t");
      Serial.print(readAccY());Serial.print("\t");
      Serial.print(readAccZ());Serial.print("\n");
      
      return;  
}
void loop(){
  //delay(20);
//calibrate();

//geforce();

//turnRight(50);
//turnLeft(50);
//delay(200);
//turnRight(50);

drive(50,13000);
delay(200);
turnLeft(50);
delay(200);
drive(50,27000);
delay(200);
turnLeft(50);
delay(200);
drive(50,13000);
delay(200);
turnLeft(50);
delay(200);
drive(50,27000);
delay(50000);
    
}

void i2cWrite(uint8_t address, uint8_t registerAddress, uint8_t data){
  Wire.beginTransmission(address);
  Wire.write(registerAddress);
  Wire.write(data);
  Wire.endTransmission();
}
uint8_t* i2cRead(uint8_t address, uint8_t registerAddress, uint8_t nbytes) {
  uint8_t data[nbytes];  
  Wire.beginTransmission(address);
  Wire.write(registerAddress);
  Wire.endTransmission();  
  Wire.beginTransmission(address);
  Wire.requestFrom(address, nbytes);
  for(uint8_t i = 0; i < nbytes; i++)
    data[i] = Wire.read();
  Wire.endTransmission();  
  return data;
}
int readGyroX() { // This really measures the y-axis of the gyro
  uint8_t* data = i2cRead(gyroAddress, 0x1F,2);    
  return ((data[0] << 8) | data[1]);
}
int readGyroY() { // This really measures the x-axis of the gyro
  uint8_t* data = i2cRead(gyroAddress, 0x1D,2);     
  return ((data[0] << 8) | data[1]);
}
double getXangle() {
  double delta = .04*256*3.3/255;
  //double accXval = (double)readAccX()-zeroValue[0];
  double accXval = (double)readAccX()*3.3/255;
  double accXcalibrated = accXval - delta;
  double accXg = accXcalibrated / (256*3.3/255);
  
  
  double accZval = (double)readAccZ()-zeroValue[2];
  double angle = (atan2(accXval,accZval)+PI)*RAD_TO_DEG;
  return angle;
}
double getYangle() {
  double accYval = (double)readAccY()-zeroValue[1];
  double accZval = (double)readAccZ()-zeroValue[2];
  double angle = (atan2(accYval,accZval)+PI)*RAD_TO_DEG;
  return angle;
}

void calibrate(){
  
  int XAdd = 0;
  int YAdd = 0;
  int ZAdd = 0;
  
  int checkX = 0;
  int checkY = 0;
  int checkZ = 0;
  
  for (int i = 0; i < calibrateIterations; i++)
  {
      /*uint8_t* dataX = i2cRead(adxlAddress, 0x32,2);
      uint8_t* dataY = i2cRead(adxlAddress, 0x34,2);
      uint8_t* dataZ = i2cRead(adxlAddress, 0x36,2);
    
      uint8_t dataTempX0 = dataX[0];
      uint8_t dataTempX1 = dataX[1];
      
      uint8_t dataTempY0 = dataY[0];
      uint8_t dataTempY1 = dataY[1];
      
      uint8_t dataTempZ0 = dataZ[0];
      uint8_t dataTempZ1 = dataZ[1];
  
      int dataTempX = 0;
      dataTempX = ((dataTempX1 & 0x03) << 8) | dataTempX0;
      
      int dataTempY = 0;
      dataTempY = ((dataTempY1 & 0x03) << 8) | dataTempY0;
      
      int dataTempZ = 0;
      dataTempZ = ((dataTempZ1 & 0x03) << 8) | dataTempZ0;*/
      checkX = readAccX();
      if (checkX < 1000)
      XAdd += checkX;
      
      checkY = readAccY();
      if (checkY < 1000)
      YAdd += checkY;
      
      checkZ = readAccZ();
      ZAdd += checkZ;
    
    
  }
  
  calibrated[0] = (double)XAdd / calibrateIterations;
  calibrated[1] = (double)YAdd / calibrateIterations;
  calibrated[2] = (double)ZAdd / calibrateIterations;
  
  Serial.print(calibrated[0]);Serial.print("\t");
  Serial.print(calibrated[1]);Serial.print("\t");
  Serial.print(calibrated[2]);Serial.print("\n");
}

void geforce(){
  
  int XAdd = 0;
  int YAdd = 0;
  int ZAdd = 0;
  
  int checkX = 0;
  int checkY = 0;
  int checkZ = 0;
  
  double finalX = 0;
  double finalY = 0;
  double finalZ = 0;
  
  for (int i = 0; i < geforceIterations; i++)
  {
      
      checkX = readAccX();
      if (checkX < 1020)
      XAdd += checkX;
      
      checkY = readAccY();
      if (checkY < 1020)
      YAdd += checkY;
      
      checkZ = readAccZ();
      if (checkZ < 1020)
      ZAdd += checkZ;
      
    
    
  }
  
  finalX = (double)XAdd / geforceIterations;
  finalY = (double)YAdd / geforceIterations;
  finalZ = (double)ZAdd / geforceIterations;
  
  //Serial.print((double)readAccX());Serial.print("\t");
  
  double delta = .04*1024*3.3/1023;
  double accXval = finalX*3.3/1023;
  double accXcalibrated = accXval - delta;
  double accXg = accXcalibrated / (256*3.3/1023);
  
 // Serial.print((double)readAccY());Serial.print("\t");
  
  double accYval = finalY*3.3/1023;
  double accYcalibrated = accYval - delta;
  double accYg = accYcalibrated / (256*3.3/1023);

  //Serial.print((double)readAccZ());Serial.print("\n");
  
  double accZval = finalZ*3.3/1023;
  double accZcalibrated = accZval - delta*2;
  double accZg = accZcalibrated / (256*3.3/1023);
  
  /*Serial.print(accXg - calibrated[0]);Serial.print("\t");
  Serial.print(accYg - calibrated[1]);Serial.print("\t");
  Serial.print(accZg - calibrated[2]);Serial.print("\n");*/
  
  Serial.print(accXg);Serial.print("\t");
  Serial.print(accYg);Serial.print("\t");
  Serial.print(accZg);Serial.print("\n");
  
}

int readAccX() {
  uint8_t* data = i2cRead(adxlAddress, 0x32,2);
//Serial.print(data[0]);Serial.print("\t"); 
//Serial.print(data[1] << 8);Serial.print("\t");
//Serial.print(data[0] | (data[1] << 8));Serial.print("\n");
  //return (data[0] | (data[1] << 8));
  //Serial.print(data[1]);Serial.print("\t");
  //Serial.print(data[0]);Serial.print("\t");
  
  uint8_t dataTemp0 = data[0];
  uint8_t dataTemp1 = data[1];
  
  int dataTemp = 0;
  dataTemp = ((dataTemp1 & 0x03) << 8) | dataTemp0;
  /*Serial.print(dataTemp);Serial.print("\t");
  Serial.print(dataTemp0);Serial.print("\t");
  Serial.print(dataTemp1);Serial.print("\t");
  Serial.print(data[0]);Serial.print("\t");
  Serial.print(data[1]);Serial.print("\n");
  Serial.print(data[1] & 0x03,HEX);Serial.print("\t");
  Serial.print(((data[1] & 0x03) << 8),BIN);Serial.print("\t");
  Serial.print((((data[1] & 0x03) << 8) | data[0]),BIN);Serial.print("\n");*/
  return (dataTemp);
}
int readAccY() {
  uint8_t* data = i2cRead(adxlAddress, 0x34,2);
  
  uint8_t dataTemp0 = data[0];
  uint8_t dataTemp1 = data[1];
  
  int dataTemp = 0;
  dataTemp = ((dataTemp1 & 0x03) << 8) | dataTemp0;
//Serial.print(data[0]);Serial.print("\t"); 
//Serial.print(data[1]);Serial.print("\t");
//Serial.print(data[0] | (data[1] << 8));Serial.print("\n");  
  //return (data[0] | (data[1] << 8));
  return (dataTemp);
}
int readAccZ() {
  uint8_t* data = i2cRead(adxlAddress, 0x36,2);
  
  uint8_t dataTemp0 = data[0];
  uint8_t dataTemp1 = data[1];
  
  int dataTemp = 0;
  dataTemp = ((dataTemp1 & 0x03) << 8) | dataTemp0;
/*Serial.print(data[0]);Serial.print("\t"); 
Serial.print(data[1]);Serial.print("\t");
Serial.print(data[0] | (data[1] << 8));Serial.print("\n");  */
  //return (data[0] | (data[1] << 8));
  return (dataTemp);
}
