# ESP_ir_blaster
program for an ESP to turn on my tv

More info here at my wiki
https://sites.google.com/view/randyhomeassistant/ir 

/*  Modified by Randy Tsuchiyama for my setup.  
Some minor changes to make it compile and work with current libraries.
 *  Based on
 *  WiFi IR Blaster by Buddy Crotty
 https://www.hackster.io/BuddyC/wifi-ir-blaster-af6bca
 *  Use an ESP8266 module or dev board to receive HTTP GET request
 *  and then send IR codes to an attached IR LED based on those requests.
 *  This works best with another web server acting as a front end that
 *  sends cURL requests based on which buttons are pressed.
 *  cURL format: http://ESP8266/IRcode

To test, put this in your browser address line
 
 192.168.xxx.xxx/irTVvup
 
 using the ip address of the esp to. For me, this will turn tv volume up
 
 The esp will respond, and you should see this:command receivedHTTP/1.1 200 OK
 
 show up in the browser window.
 
 Debugging works best if you have the esp connected with usb to a pc, and open Serial Monitor in Platformio, select you serial port with  baud rage of 115200
 
 If you push the ESP reset button with the serial monitor on, you can get the ESP's ip address as it initializes.
 
 Built on Platformio running in Atom on a macbook
 
 Platformio has library IRremoteESP8266 by Ken Shirriff installed from Platformio Home
 
 IR codes were determined by using the adafruit guide found there
 
 https://learn.adafruit.com/using-an-infrared-library/hardware-needed
 using irlib2
 
 this worked for sony tv and onkyo receiver, but not for panasonic blu ray
 so I had to read raw codes for panny
 Sony TV codes.
 
 Program gives you codes as hex, I converted to decimal for use in this program
 
 sony tv codes:
 
 off = $F50 = dec 3920
 
 on = $750 = dec 1872
 
 pwr toggle = $A90 = 2704
 
 v+ = $490 = 1168
 
 v- = $c90 = 3216
 
 mute = $290 = 656
 
 
 used this for rx of raw rawData
 
 https://github.com/cyborg5/IRLib2/blob/master/IRLib2/examples/rawRecv/rawRecv.ino
