/*  Modified by Randy Tsuchiyama for my setup
 *  Based on
 *  WiFi IR Blaster by Buddy Crotty
 *  Use an ESP8266 module or dev board to receive HTTP GET request
 *  and then send IR codes to an attached IR LED based on those requests.
 *  This works best with another web server acting as a front end that
 *  sends cURL requests based on which buttons are pressed.
 *  cURL format: http://ESP8266/IRcode
 Codes were determined by using the adafruit guide found there
 https://learn.adafruit.com/using-an-infrared-library/hardware-needed
 using irlib2
 this worked for sony tv and onkyo receiver, but not for panasonic blu ray
 so I had to read raw codes for panny
 Sony TV codes
 off = F50 = dec 3920
 on = 750 = dec 1872
 pwr toggle = A90 = 2704
 v+ = 490 = 1168
 v- = c90 = 3216
 mute = 290 = 656

 used this for rx of raw rawData
 https://github.com/cyborg5/IRLib2/blob/master/IRLib2/examples/rawRecv/rawRecv.ino

 */

#include <ESP8266WiFi.h>
#include <IRremoteESP8266.h>
///////////////////////////////////////////////////////
//added to make work
#include <IRsend.h>  // Needed if you want to send IR commands.
#include <IRrecv.h>  // Needed if you want to receive IR commands.
///////////////////////////////////////////////////////
#include <ESP8266mDNS.h>

//  Wifi ssid and password here
const char* ssid = "";
const char* password = "";
//////////////////////////////////////////
// added WiFi before MDNSResponder
MDNSResponder mdns;

int khz = 38; // 38kHz carrier frequency for both NEC and Samsung

IRsend irsend(4); //an IR led is connected to GPIO4 (pin D2 on NodeMCU)

//////////////////////////////////////////////////
// changed types to uint16_t for all of the arrays below with the raw ir codes

  // Insert RAW IR signal for panny blu ray power toggle
  uint16_t irDVDpwr[100]={
          3450, 1778, 398, 502, 374, 1346, 398, 498,
          370, 502, 374, 498, 366, 506, 374, 498,
          366, 506, 370, 502, 374, 498, 370, 502,
          374, 498, 370, 502, 374, 1342, 402, 498,
          366, 506, 370, 502, 366, 506, 370, 502,
          366, 506, 370, 1346, 398, 1350, 390, 506,
          370, 1350, 394, 506, 370, 502, 366, 506,
          370, 502, 366, 506, 370, 498, 366, 506,
          374, 498, 366, 506, 370, 1350, 394, 502,
          374, 1346, 398, 502, 374, 498, 370, 502,
          374, 498, 366, 506, 374, 1342, 398, 502,
          366, 1350, 402, 1346, 398, 1346, 398, 502,
          374, 1342, 450, 1000};

uint16_t irDVDpwroff[]={ 3456U, 1728U, 432U, 432U, 432U, 1296U, 432U, 432U, 432U, 432U, 432U, 432U, 432U, 432U, 432U, 432U, 432U, 432U, 432U, 432U, 432U, 432U, 432U, 432U, 432U, 432U, 432U, 432U, 432U, 1296U, 432U, 432U, 432U, 432U, 432U, 432U, 432U, 432U, 432U, 432U, 432U, 432U, 432U, 1296U, 432U, 1296U, 432U, 432U, 432U, 1296U, 432U, 432U, 432U, 432U, 432U, 432U, 432U, 432U, 432U, 432U, 432U, 432U, 432U, 432U, 432U, 432U, 432U, 1296U, 432U, 432U, 432U, 1296U, 432U, 1296U, 432U, 1296U, 432U, 1296U, 432U, 432U, 432U, 432U, 432U, 1296U, 432U, 432U, 432U, 1296U, 432U, 1296U, 432U, 432U, 432U, 432U, 432U, 432U, 432U, 1296U, 432U, 65535U };

uint16_t irDVDplay[100]={
  3450, 1778, 398, 502, 374, 1346, 398, 498,
  370, 502, 374, 498, 366, 506, 374, 498,
  366, 506, 370, 502, 374, 498, 370, 502,
  374, 498, 370, 502, 374, 1342, 402, 498,
  366, 506, 370, 502, 366, 506, 370, 502,
  366, 506, 370, 1346, 398, 1350, 390, 506,
  370, 1350, 394, 506, 370, 502, 366, 506,
  370, 502, 366, 506, 370, 498, 366, 506,
  374, 498, 366, 506, 370, 1350, 394, 502,
  374, 1346, 398, 502, 374, 498, 370, 502,
  374, 498, 366, 506, 374, 1342, 398, 502,
  366, 1350, 402, 1346, 398, 1346, 398, 502,
  374, 1342, 450, 1000};

uint16_t irDVDpause[100]={
  3454, 1806, 370, 502, 374, 1342, 402, 498,
  366, 506, 370, 502, 366, 506, 370, 502,
  366, 506, 370, 502, 366, 506, 370, 502,
  374, 494, 370, 502, 378, 1342, 398, 502,
  366, 506, 370, 502, 366, 506, 370, 498,
  366, 506, 374, 1346, 394, 1350, 394, 506,
  370, 1346, 398, 502, 374, 498, 370, 502,
  374, 498, 366, 506, 370, 502, 366, 506,
  370, 502, 366, 506, 370, 1346, 398, 1350,
  390, 506, 374, 498, 366, 506, 370, 502,
  374, 498, 370, 502, 374, 1346, 398, 1346,
  398, 502, 374, 1342, 402, 1346, 394, 502,
  378, 1342, 398, 1000};

uint16_t irDVDff[100]={
  3446, 1786, 402, 498, 370, 1346, 394, 506,
  370, 502, 366, 506, 370, 502, 366, 506,
  370, 502, 362, 510, 370, 498, 378, 494,
  370, 502, 374, 498, 370, 1378, 366, 506,
  370, 502, 362, 506, 374, 498, 366, 506,
  370, 502, 374, 1374, 370, 1346, 398, 502,
  374, 1342, 402, 498, 366, 506, 374, 498,
  366, 506, 370, 502, 366, 506, 370, 502,
  374, 498, 370, 1374, 366, 506, 374, 1342,
  398, 502, 374, 498, 370, 502, 374, 498,
  370, 502, 374, 1346, 398, 498, 378, 1342,
  398, 502, 366, 1350, 402, 1342, 402, 498,
  370, 1350, 402, 1000};

uint16_t irDVDrev[100]={
  3450, 1778, 398, 502, 374, 1346, 398, 498,
  366, 506, 374, 498, 366, 506, 370, 502,
  374, 498, 370, 502, 374, 498, 370, 502,
  374, 498, 366, 506, 374, 1342, 398, 502,
  366, 506, 370, 502, 374, 498, 370, 502,
  374, 498, 366, 1350, 394, 1354, 398, 498,
  370, 1350, 394, 506, 370, 502, 374, 498,
  370, 502, 374, 494, 370, 502, 374, 498,
  370, 502, 374, 498, 370, 502, 374, 1346,
  398, 498, 378, 494, 370, 502, 374, 498,
  370, 502, 374, 498, 370, 502, 374, 1346,
  450, 450, 426, 1290, 398, 1346, 398, 502,
  374, 1346, 398, 1000};

uint16_t irDVDstop[100]={
  3454, 1778, 398, 502, 366, 1350, 402, 498,
  370, 502, 374, 498, 366, 506, 370, 502,
  366, 506, 370, 502, 366, 506, 370, 502,
  366, 506, 370, 498, 378, 1342, 402, 498,
  366, 506, 370, 502, 366, 506, 370, 502,
  366, 502, 374, 1346, 394, 1350, 394, 506,
  370, 1350, 394, 502, 374, 498, 370, 502,
  374, 498, 366, 506, 374, 498, 366, 506,
  370, 502, 366, 506, 370, 502, 374, 498,
  370, 502, 374, 498, 366, 506, 374, 498,
  366, 502, 374, 498, 370, 502, 374, 498,
  370, 502, 374, 1346, 398, 1346, 394, 506,
  370, 1346, 398, 1000};

// Create an instance of the server
// specify the port to listen on as an argument
WiFiServer server(80);


void setup() {
  Serial.begin(115200);
  delay(10);

  irsend.begin();

  // Connect to WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi Connected");
  // Start the server
  server.begin();
  Serial.println("HTTP Server Started");
  // Print the IP address
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  if (mdns.begin("IRBlasterLR", WiFi.localIP())) {
    Serial.println("MDNS Responder Started");
  }
  Serial.println();
  Serial.println();
}

void loop() {
  // Check if a client has connected
  WiFiClient client = server.available();
  if (!client) {
    return;
  }

  // Wait until the client sends some data
  Serial.println("new client");
  while(!client.available()){
    delay(1);
  }

  client.print("command received");

  // Read the first line of the request
  String req = client.readStringUntil('\r');
  Serial.println(req);
  client.flush();

  // Match the request
  if (req.indexOf("/irTVpon") != -1){
      irsend.sendSony(1872, 12);  //1872 = $750
      Serial.println("IRreq irTVpon sent");
  }
  else if (req.indexOf("/irTVpoff") != -1){
      irsend.sendSony(3920, 12);  //3920 = $f50
      Serial.println("IRreq irTVpoff sent");
  }
  else if (req.indexOf("/irTVmute") != -1){
      irsend.sendSony(656, 12);
      Serial.println("IRreq irTVmute sent");
  }
  else if (req.indexOf("/irTVvdn") != -1){
      irsend.sendSony(3216, 12);
      Serial.println("IRreq irTVvdn sent");
  }
  else if (req.indexOf("/irTVvup") != -1){
      irsend.sendSony(1168, 12);
      Serial.println("IRreq irTVvup sent");
  }
  else if (req.indexOf("/irTVvid1") != -1){
      irsend.sendSony(48, 12);
      Serial.println("IRreq video 1 sent");
  }
  else if (req.indexOf("/irTVvid2") != -1){
      irsend.sendSony(2096, 12);
      Serial.println("IRreq video 2 sent");
  }
  else if (req.indexOf("/irTVvid3") != -1){
      irsend.sendSony(1072, 12);
      Serial.println("IRreq video 3 sent");
  }
  else if (req.indexOf("/irTVvid4") != -1){
      irsend.sendSony(3632, 12);
      Serial.println("IRreq video 4 sent");
  }
  else if (req.indexOf("/irTVvid5") != -1){
      irsend.sendSony(304, 12);
      Serial.println("IRreq video 5 sent");
  }
  else if (req.indexOf("/irTVvid6") != -1){
      irsend.sendSony(2352, 12);
      Serial.println("IRreq video 6 sent");
  }
  else if (req.indexOf("/irTVvid7") != -1){
      irsend.sendSony(8681, 15);
      Serial.println("IRreq video 7 sent");
  }
  else if (req.indexOf("/irTVvid8") != -1){
      irsend.sendSony(2640, 12);
      Serial.println("IRreq video 8 sent");
  }
  else if (req.indexOf("/irTVvid9") != -1){
      irsend.sendSony(13294, 15);
      Serial.println("IRreq video 9 sent");
  }
  //else if (req.indexOf("/irALLpwr") != -1){
//      irsend.sendRaw(irRECpwrON, sizeof(irRECpwrON) / sizeof(irRECpwrON[0]), khz);
//      irsend.sendRaw(irTVpwr, sizeof(irTVpwr) / sizeof(irTVpwr[0]), khz);
//      delay(2000);
//      irsend.sendRaw(irRECsrc, sizeof(irRECsrc) / sizeof(irRECsrc[0]), khz);
//      Serial.println("IRreq irALLpwr sent");
//  }
  else if (req.indexOf("/irDVD1pwr") != -1){
      irsend.sendRaw(irDVDpwroff, 100U, 37U);
      Serial.println("IRreq irDVDpwr0 sent");
  }
  else if (req.indexOf("/irDVD2pwr") != -1){
      irsend.sendPanasonic(176, 62);
      Serial.println("IRreq irDVDpwr1 sent");
  }
  else if (req.indexOf("/irDVD3pwr") != -1){
      irsend.sendPanasonic(176U, 62U);
      Serial.println("IRreq irDVDpwr2 sent");
  }
  else if (req.indexOf("/irDVDplay") != -1){
    irsend.sendRaw(irDVDplay, 100U, 37U);
    Serial.println("IRreq irDVDplay sent");
  }
  else if (req.indexOf("/irDVDstop") != -1){
    irsend.sendRaw(irDVDstop, 100U, 37U);
    Serial.println("IRreq irDVDstop sent");
  }
  else if (req.indexOf("/irRECpwrON") != -1){
      irsend.sendNEC(1270227167, 32);
      Serial.println("IRreq irRECpwrON sent");
  }
  else if (req.indexOf("/irRECpwrOFF") != -1){
      irsend.sendNEC(1261888029, 32);
      Serial.println("IRreq irRECpwrOFF sent");
  }
  else if (req.indexOf("/irRECmute") != -1){
      irsend.sendNEC(1270227167, 32);
      Serial.println("IRreq irRECmute sent");
  }
  else if (req.indexOf("/irRECvdn") != -1){
      irsend.sendNEC(1270267967, 32);
      Serial.println("IRreq irRECvdn sent");
  }
  else if (req.indexOf("/irRECvup") != -1){
      irsend.sendNEC(1270235327, 32);
      Serial.println("IRreq irRECvup sent");
  }
  else if (req.indexOf("/irRECTV") != -1){
      irsend.sendNEC(1270263887, 32);
      Serial.println("IRreq irRECTV sent");
  }
  else if (req.indexOf("/irRECAux") != -1){
      irsend.sendNEC(1270282502, 32);
      Serial.println("IRreq irRECAux sent");
  }
  else if (req.indexOf("/irRECCBL") != -1){
      irsend.sendNEC(1270247567, 32);
      Serial.println("IRreq irRECCBL sent");
  }
  else if (req.indexOf("/irRECDVD") != -1){
      irsend.sendNEC(1261842894, 32);
      Serial.println("IRreq irRECDVD sent");
  }

  else {
    Serial.println("invalid request");
    client.stop();
    return;
  }

  client.flush();

  // Send the response to the client
  //client.print(s);
  client.print("HTTP/1.1 200 OK\r\n");
  delay(1);
  Serial.println("Client Disconnected");
  Serial.println();
  // The client will actually be disconnected
  // when the function returns and 'client' object is detroyed
}
