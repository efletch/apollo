//Communication



#define gyroAddress 0x68
#define adxlAddress 0x53

//XBee Setup
//XBee xbee = XBee();
uint8_t payload[] = {0, 0, 0, 0, 0};
XBeeAddress64 addr64 = XBeeAddress64(0x0013a200, 0x408d6448);
Tx64Request tx = Tx64Request(addr64, payload, sizeof(payload));
TxStatusResponse txStatus = TxStatusResponse();

void xbeeSend(){
  
 xbee.send(tx);
  
    // after sending a tx request, we expect a status response
    // wait up to 5 seconds for the status response
    if (xbee.readPacket(100)) {
        // got a response!

        // should be a znet tx status            	
    	if (xbee.getResponse().getApiId() == TX_STATUS_RESPONSE) {
    	   xbee.getResponse().getZBTxStatusResponse(txStatus); 
        }
    }
  
  
}
