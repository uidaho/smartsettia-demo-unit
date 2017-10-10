/*
  Web Server

 A simple web server that shows the value of the analog input pins.
 using an Arduino Wiznet Ethernet shield.

 Circuit:
 * Ethernet shield attached to pins 10, 11, 12, 13
 * Analog inputs attached to pins A0 through A5 (optional)

 created 18 Dec 2009
 by David A. Mellis
 modified 9 Apr 2012
 by Tom Igoe
 modified 02 Sept 2015
 by Arturo Guadalupi

 */

#include "Sensors_Servo.h"
#include <SPI.h>
#include <Ethernet.h>

// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network:
byte mac[] = {
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED
};
IPAddress ip(192, 168, 2, 205);

// Initialize the Ethernet server library
// with the IP address and port you want to use
// (port 80 is default for HTTP):
EthernetServer server(80);


// prototypes
void parseRequest(char * HTTP_req);

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(115200);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  // Initialize sensors & servo pins
  initSensors();

  // start the Ethernet connection and the server:
  Ethernet.begin(mac, ip);
  server.begin();
  Serial.print(F("server is at "));
  Serial.println(Ethernet.localIP());

  // disable cd
  pinMode(4, OUTPUT);
  digitalWrite(4, HIGH);

  Serial.println(F("Initialization Done."));
  Serial.println(F("================================"));
}


void loop() {
  processWebServer();
  ServoUpdate();
  getSwitches();
  delay(20);
}




void processWebServer()
{
  char HTTP_req[50];
  HTTP_req[0] = '\0';
  unsigned short HTTP_reqPtr = 0;
  boolean GetDataStart = false;    // true when '?' hit
    // listen for incoming clients
  EthernetClient client = server.available();
  if (client) {
    Serial.println(F("new client"));
    // an http request ends with a blank line
    boolean currentLineIsBlank = true;
    while (client.connected()) {
        if (client.available()) {   // client data available to read
            char c = client.read(); // read 1 byte (character) from client
            if (c == '?' || GetDataStart) {
              GetDataStart = true;
              if (c == ' ') {  // hit space. End of Get
                GetDataStart = false;
              }
              else if (HTTP_reqPtr < 49) {   // test if over 50
                   HTTP_req[HTTP_reqPtr++] = c;
                   HTTP_req[HTTP_reqPtr] = '\0';  // null the end of the string
               }
             } // if '?'
               
            // last line of client request is blank and ends with \n
            // respond to client only after last line received
            if (c == '\n' && currentLineIsBlank) {
                // send a standard http response header
                client.println(F("HTTP/1.1 200 OK"));
                client.println(F("Content-Type: text/html"));
                client.println(F("Connection: close"));
                client.println();
                // send web page
                //Serial.println(FreeRam());
                client.println(F("<HTML>"));
                client.println(F("<META HTTP-EQUIV=\"refresh\" CONTENT=\"5 url='192.168.2.205'\">"));
                client.println(F("<HEAD>"));
                client.println(F("<TITLE>Smartsettia Demo</TITLE>"));
                client.println(F("</HEAD>"));
                client.println(F("<BODY>"));
                client.println(F("<H1>Smartsettia Demo</H1>"));
                client.println(F("<hr />"));
                client.println(F("<br />"));
                client.println(F("<form action=\"/action.php\" method=\"get\">"));
                 client.println(F("<button name=\"cover\" type=\"submit\" value=\"close\">Close</button>"));
                 client.println(F("<button name=\"cover\" type=\"submit\" value=\"open\">Open</button>"));
                client.println(F("</form>"));
                client.println(F("</BODY>"));
                client.println(F("</HTML>"));
                
                break;
            }
            // every line of text received from the client ends with \r\n
            if (c == '\n') {
                // last character on line of received text
                // starting new line with next character read
                currentLineIsBlank = true;
            } 
            else if (c != '\r') {
                // a text character was received from client
                currentLineIsBlank = false;
            }
        } // end if (client.available())
    } // end while (client.connected())
    delay(1);      // give the web browser time to receive the data
    client.stop(); // close the connection
    Serial.println(F("client disconnected\n"));
    if (HTTP_req[0] != '\0')
      parseRequest(HTTP_req);
  } // end if (client)
}


void parseRequest(char * HTTP_req)
{
     Serial.print(F("Req: \""));
     Serial.print(HTTP_req);
     Serial.println("\"");

     char *strTest = HTTP_req +1;
     strTest[11] = '\0';   // set null char after what could be "cover=close"
     
     if (strcmp(strTest, "cover=close" ) == 0){
         Serial.println(F("EVENT CLOSE"));
         ServoSet(SERVO_CLOSE);
     }
     strTest[10] = '\0';   // set null char after what could be "cover=open"
     if (strcmp(strTest, "cover=open" ) == 0){
         Serial.println(F("EVENT OPEN"));
         ServoSet(SERVO_OPEN);
     }
}

