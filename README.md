# ESP_ir_blaster
program for an ESP to turn on my tv

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
 or put this in your browswer
 192.168.xxx.xxx/irTVpwron
 
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
