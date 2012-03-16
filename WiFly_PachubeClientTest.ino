//#include <SoftwareSerial.h>

// WiFly Pachube Client
// Send data to a Pachube Feed
// (Based on Ethernet's WebClient Example)
// (based upon Sparkfun WiFly Web Client example)
// Sparkfun WiFly library updated and can be found here
// https://github.com/jcrouchley/WiFly-Shield
// Built using Arduino IDE V0.22

#include "WiFly.h"

// using NewSoftSerial V11 beta
// downloaded from here http://arduiniana.org/2011/01/newsoftserial-11-beta/
// this will be included as Software Serial in Arduino IDE 1.0
//#include <SoftwareSerial.h>

// Wifly RN-XV (XBee shaped) module connected
//  WiFly Tx to pin 2 (SoftSerial Rx)
//  WiFly Rx to pin 3 (SoftSerial Tx)
//SoftwareSerial mySerial(7, 8);


// Edit credentials.h to provide your own credentials
#include "Credentials.h"
#include <string.h>
#define MAX_STRING_LEN  20
#define MAXDATASTREAMS 6
#define BUFFSIZ 90 // plenty big
char buffer[BUFFSIZ];

//char *parseptr;
char buffidx;
int id[MAXDATASTREAMS];
int data[MAXDATASTREAMS];
//char* id[MAXDATASTREAMS];
//char* data[MAXDATASTREAMS];
int numDataStreams;
// Using Pachube API V2
WiFlyClient client("api.pachube.com", 80);

void setup() {
  Serial.begin(9600);  // nice and fast
  //Serial1.begin(9600);  // default WiFly baud rate - good enough for this
  //Serial.println("SETUP");
  //pinMode(13,OUTPUT);
  //digitalWrite(8,HIGH);
  // lots of time for the WiFly to start up and also in case I need to stop the transmit
  //delay(10000);


  delay(2000);
  WiFly.setUart(&Serial1); // Tell the WiFly library that we are not using the SPIUart

  Serial.println("Wifly begin");

  WiFly.begin();    // startup the WiFly

  Serial.println("Wifly join");

  // Join the WiFi network
    if (!WiFly.join(ssid, passphrase, WEP_MODE)) {
      Serial.println("Association failed.");
      while (1) {
        // Hang on failure.
   }
  }  

}

uint32_t timeLastUpdated;
int i;
char buff[64];

void loop() {
  //if (millis() - timeLastUpdated > TIMETOUPDATE)
  //{  // time for the next update
    //timeLastUpdated = millis();

    // prepare the data to send
    // format (API V2)
    // multiple lines each with <datastreamID>,<datastreamValue>
    // feedID can be the datastream name of the numberic ID
    //sprintf(buff,"0,%d\n1,%d",i++,analogRead(0));
    Serial.println("connecting...");
    if (client.connect()) {
      Serial.println("connected");
      client.print("GET /v2/feeds/");  // APIV2
      client.print(PACHUBEFEED);
      client.println(".csv HTTP/1.1");
      client.println("Host: api.pachube.com");
      client.print("X-PachubeApiKey: ");
      client.println(APIKEY);

      client.println("User-Agent: Arduino (WiFly RN-XV)");
      //client.print("Content-Type: text/csv\nContent-Length: ");
      //client.println(strlen(buff));
      client.println("Connection: close");
      client.println();

      //client.print(buff);
      client.println();

    } 
    else {
      Serial.println("connection failed");
    }

    //delay(2000);
    numDataStreams = 0;
    boolean current_line_is_blank = false;
    boolean parsedHeader = false;
    boolean gotNewLine = false;
    //unsigned int BUFFSIZ = 90; // plenty big
    //char buffer[BUFFSIZ];
    int lineLength = 0;
    while(client.connected()) {
      
      if(client.available()){
        char c;
        if(parsedHeader == false){
          c = client.read();
        }
        // display the result
        if (c == '\n' && current_line_is_blank) {
          parsedHeader=true;
        }

        if(parsedHeader){
          //numDataStreams++;
          //Serial.println("GOT HEADER");
          readline();
          //data[numDataStreams] = buffer;
          //numDataStreams++;
          //Serial.println(buffer);
          parseDataStream(buffer);
          //Serial.println(numDataStreams);
          
        }
        // TODO verify success (HTTP/1.1 200 OK)

        if (c == '\n') {
          // we're starting a new line
          current_line_is_blank = true;
        } 
        else if (c != '\r') {
          // we've gotten a character on the current line
          current_line_is_blank = false;
        }
      }
    }
    //Serial.println("BEFORE DATA");
    Serial.print("NumDataSteams: ");
    Serial.println(numDataStreams);
    analogWrite(13, data[0]);
    analogWrite(12, data[1]);
    analogWrite(11, data[2]);
//    for(int j=0;j<numDataStreams;j++){
//      //Serial.println(j);
//      //Serial.print("ID: ");
//      //Serial.print(id[j]);
//      Serial.print("DATA: ");
//      Serial.println(data[j]);
//    }
//    if (client.connected()) {
      Serial.println("disconnecting.");
      //client.stop();
      Serial.println("disconnected.");
//    }
  delay(10000);
  }
//}
//void readline(char *buffer, unsigned int bufsize) {
void readline() {
  //#define BUFFSIZ 90 // plenty big
  //char buffer[BUFFSIZ];
  memset(buffer,0,sizeof(buffer));
  char c;
  int i=0;
  buffidx = 0; // start at begninning
  while (i<BUFFSIZ) {
    i++;
    //Serial.println("STUk");
    c=client.read();
    if (c == -1)
      //Serial.println("-1");
      continue;
    //Serial.print(c);
    //if (c == '\n')
    //continue;
    if ((buffidx == BUFFSIZ-1) || (c == '\n')) {
      buffer[buffidx] = 0;
      return;
    }
    buffer[buffidx++]= c;
    delay(2);
  }
  //parsedHeader = false;
}

void parseDataStream(char *stuff){
  //  Serial.print(numDataSreams);
  //  Serial.print(":");
  //  Serial.print(subStr(stuff, ",",1));
  //  Serial.print(":");
  //  Serial.println(subStr(stuff, ",",3));
  //str
  id[numDataStreams] = parsedecimal(subStr(stuff, ",",1));
  //id[numDataStreams] = subStr(stuff, ",",1);
  //data[numDataStreams] = parsedecimal(subStr(buffer, ",",2));
  data[numDataStreams] = parsedecimal(subStr(stuff, ",",3));
  //data[numDataStreams] = subStr(stuff, ",",3);
  //Serial.println(j);


  //     Serial.print("ID: ");
  //     Serial.print(id[numDataStreams]);
  //     Serial.print(" DATA: ");
  //     Serial.println(data[numDataStreams]);
  //     if(numDataStreams == 2){
  //       //Serial.println("BLINK");
  //        blinkLED(13,parsedecimal(data[numDataStreams]),100);
  //      }
  numDataStreams++;

}
char* subStr (char *str, char *delim, int index) {
  char *act, *sub, *ptr;
  static char copy[MAX_STRING_LEN];
  int i;

  // Since strtok consumes the first arg, make a copy
  strcpy(copy, str);

  for (i = 1, act = copy; i <= index; i++, act = NULL) {
    //Serial.print(".");
    sub = strtok_r(act, delim, &ptr);
    if (sub == NULL) break;
  }
  return sub;

}
uint32_t parsedecimal(char *str) {
  uint32_t d = 0;

  while (str[0] != 0) {
    if ((str[0] > '9') || (str[0] < '0'))
      return d;
    d *= 10;
    d += str[0] - '0';
    str++;
  }
  return d;
}
// this function blinks the an LED light as many times as requested, at the requested blinking rate
void blinkLED(byte targetPin, int numBlinks, int blinkRate) {
  for (int i=0; i<numBlinks; i++) {
    digitalWrite(targetPin, HIGH);   // sets the LED on
    delay(blinkRate);                     // waits for blinkRate milliseconds
    digitalWrite(targetPin, LOW);    // sets the LED off
    delay(blinkRate);
  }
}



