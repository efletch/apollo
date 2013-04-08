//Communication

//XBee Setup
//XBee xbee = XBee();
uint8_t payload[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
//X3 X2 X1 XDecimal, Y3 Y2 Y1 YDecimal, IR, Ultrasonic, Orientation
XBeeAddress64 addr64 = XBeeAddress64(0x0013a200, 0x408d6448);
Tx64Request tx = Tx64Request(addr64, payload, sizeof(payload));
TxStatusResponse txStatus = TxStatusResponse();

void xbeeSend(){
  
 xbee.send(tx);
 //Serial.print("Meters X: %d Meters Y: %d\n",metersX,metersY);
  
    // after sending a tx request, we expect a status response
    // wait up to 5 seconds for the status response
    if (xbee.readPacket(50)) {
        // got a response!

        // should be a znet tx status            	
    	if (xbee.getResponse().getApiId() == TX_STATUS_RESPONSE) {
    	   xbee.getResponse().getZBTxStatusResponse(txStatus); 
        }
    }
  
  
}

void xbeeCollect(int ir, int ultra){
  //Multiply meters by ten so that the decimal is the fourth most significant digit
  //Remember the transmitted values are in tenths of meters
  uint8_t negativeX = 1;
  uint8_t negativeY = 1;
  if (metersX<0)
    negativeX = 0;
  if (metersY<0)
    negativeY = 0;
  
  payload[0] = negativeX;
  payload[1] = (uint8_t)(((((int)abs(metersX))*10)/1000)%10);//Most significant digit
  payload[2] = (uint8_t)(((((int)abs(metersX))*10)/100)%10);//Second most significant
  payload[3] = (uint8_t)(((((int)abs(metersX))*10)/10)%10);//Third most significant
  payload[4] = (uint8_t)(((((int)abs(metersX))*10)/1)%10);//Fourth most significant
  payload[5] = negativeY;
  payload[6] = (uint8_t)(((((int)abs(metersY))*10)/1000)%10);
  payload[7] = (uint8_t)(((((int)abs(metersY))*10)/100)%10);
  payload[8] = (uint8_t)(((((int)abs(metersY))*10)/10)%10);
  payload[9] = (uint8_t)(((((int)abs(metersY))*10)/1)%10);
  //payload[10] = (uint8_t)irSensor();
  payload[10] = (uint8_t)ir;
  //payload[11] = (uint8_t)ultrasonicSensor();
  payload[11] = (uint8_t)ultra;
  payload[12] = orientationNum;
  
}
