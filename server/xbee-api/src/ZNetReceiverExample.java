/**
 * Copyright (c) 2008 Andrew Rapp. All rights reserved.
 *  
 * This file is part of XBee-API.
 *  
 * XBee-API is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *  
 * XBee-API is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *  
 * You should have received a copy of the GNU General Public License
 * along with XBee-API.  If not, see <http://www.gnu.org/licenses/>.
 */



import java.io.FileOutputStream;
import java.io.FileWriter;
import java.io.PrintStream;
import java.io.PrintWriter;

import org.apache.log4j.Logger;
import org.apache.log4j.PropertyConfigurator;

import com.rapplogic.xbee.api.ApiId;
import com.rapplogic.xbee.api.AtCommand;
import com.rapplogic.xbee.api.AtCommandResponse;
import com.rapplogic.xbee.api.XBee;
import com.rapplogic.xbee.api.XBeeResponse;
import com.rapplogic.xbee.api.wpan.RxBaseResponse;
import com.rapplogic.xbee.api.wpan.RxResponse;
import com.rapplogic.xbee.api.wpan.TxStatusResponse;
import com.rapplogic.xbee.api.zigbee.ZNetRxResponse;
import com.rapplogic.xbee.util.ByteUtils;

/**
 * This class is the companion to ZNetSenderTest.java, and as such, it receives packets sent by ZNetSenderTest.java
 * See the ZNetSenderTest.java for information on how to configure your XBee for this demo
 * 
 * You can start ZNetSenderTest.java and this class in any order but it's generally best to start this class first.
 * 
 * @author andrew
 *
 */
public class ZNetReceiverExample {

        private final static Logger log = Logger.getLogger(ZNetReceiverExample.class);
        
        private ZNetReceiverExample() throws Exception {
                XBee xbee = new XBee();    
                FileOutputStream out;
                FileOutputStream out1;
                PrintStream prt;
                PrintStream prt1;

                try {                   
                        // replace with the com port of your receiving XBee (typically your end device)
//                      xbee.open("/dev/tty.usbserial-A6005uRz", 9600);
                        xbee.open("COM8", 9600);
                        
                        //create FileOutputStream and PrintStream for text file
                        out = new FileOutputStream("datafile.txt");
                        out1 = new FileOutputStream("datafile2.txt");
                        prt = new PrintStream(out);
                        prt1 = new PrintStream(out1);
                       
                        while (true) {

                                try {
                                	
                                		System.out.println("waiting for packet");
                                        // we wait here until a packet is received.
                                        XBeeResponse response = xbee.getResponse();
                                        System.out.println("got response");
                                        
                                       // log.info("received response " + response.toString());
                                        System.out.println("received response" + response.toString());
                                        String line = response.value();
                                        
                                       String [] array = line.split(" ");
                                       //scale data by multiplying by 50
                                       int xdata = (Integer.parseInt(array[0]))*50;  //x position
                                       int ydata = Integer.parseInt(array[1])*50; //y position
                                       
                                       //convert distance from inches to mm
                                       float infrared = (float) ((Integer.parseInt(array[2])) * 25.4); //ir value
                                       float ultrasonic = (float) ((Integer.parseInt(array[3])) * 25.4); //ultrasonic value
                                       
                                       int direction = Integer.parseInt(array[4]); //orientation
                                       
                                       float distance = (infrared + ultrasonic)/2; //averaging the two sensor values together
                                   
                                       //for infrared based points
                                       float pointx = 0;
                                       float pointy = 0;
                                       
                                       
                                       //for ultrasonic based points
                                       float pointxU = 0;
                                       float pointyU = 0;
                                       
                                     //direction 0: increase x
                                       if(direction ==0)
                                       {
                                       pointx = xdata + infrared;
                                       pointy = ydata;
                                       
                                       pointxU = xdata + ultrasonic;
                                       pointyU = ydata;
                                       
                                       }
                                       
                                     //direction 1: increase y
                                       if(direction ==1)
                                       {
                                       pointx = xdata;
                                       pointy = ydata + infrared;   
                                       
                                       pointxU = xdata;
                                       pointyU = ydata + ultrasonic;
                                       }
                                       
                                     //direction 2: decrease x
                                       if(direction ==2)
                                       {
                                       pointx = xdata - infrared;
                                       pointy = ydata;   
                                       
                                       pointxU = xdata - ultrasonic;
                                       pointyU = ydata;   
                                       }
                                       
                                     //direction 3: decrease y
                                       if(direction ==3)
                                       {
                                       pointx = xdata;
                                       pointy = ydata - infrared;   
                                       
                                       pointxU = xdata;
                                       pointyU = ydata - ultrasonic;
                                       }
                                        
                                        System.out.println(xdata + " " + ydata+ " " + infrared + " " + ultrasonic + " " + distance + " " + direction);
                                        
                                        prt.println(pointx + " " + pointy + " " + "130"); //130 is the set z value, infrared based points
                                        
                                        prt1.println(infrared + " " + ultrasonic); //ultrasonic based points
                                        
                                        System.out.println("Api id: " + response.getApiId());
                                        
                                        
                                        if (response.getApiId() == ApiId.TX_STATUS_RESPONSE) {
                                                // we received a packet from ZNetSenderTest.java
                                                TxStatusResponse rx = (TxStatusResponse) response;
                                                System.out.println("Tx_status_response");
                                               // log.info("Received RX packet, option is " + rx.getOption() + ", sender 64 address is " + ByteUtils.toBase16(rx.getRemoteAddress64().getAddress()) + ", remote 16-bit address is " + ByteUtils.toBase16(rx.getRemoteAddress16().getAddress()) + ", data is " + ByteUtils.toBase16(rx.getData()));

                                                // optionally we may want to get the signal strength (RSSI) of the last hop.
                                                // keep in mind if you have routers in your network, this will be the signal of the last hop.
                                                AtCommand at = new AtCommand("DB");
                                                xbee.sendAsynchronous(at);
                                                XBeeResponse atResponse = xbee.getResponse();
                                                
                                                if (atResponse.getApiId() == ApiId.AT_RESPONSE) {
                                                        // remember rssi is a negative db value
                                                       // log.info("RSSI of last response is " + -((AtCommandResponse)atResponse).getValue()[0]);
                                                } else {
                                                        // we didn't get an AT response
                                                       // log.info("expected RSSI, but received " + atResponse.toString());
                                                }
                                        } else {
                                                log.debug("received unexpected packet " + response.toString());
                                        }
                                } catch (Exception e) {
                                		System.out.println("exeption");
                                		prt.close();
                                		prt1.close();
                                        log.error(e);
                                }
                        }
                } finally {
                        if (xbee.isConnected()) {
                        		System.out.println("closing xbee");
                                xbee.close();
                        }
                }
        }

        public static void main(String[] args) throws Exception {
                // init log4j
                PropertyConfigurator.configure("C:\\Users\\Bhargavi\\Documents\\workspace\\xbee-api\\log4j.properties");
                System.out.println("new example");
                new ZNetReceiverExample();
        }
}