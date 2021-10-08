//------- Import Library ----------//
#include "ThingSpeak.h"
#include <ESP8266WiFi.h>

//------- WI-FI details ----------//
char ssid[] = "Xav!er";             //SSID here
char pass[] = "12345678";           // Passowrd here
//--------------------------------//

//----------- Channel details ----------------//
unsigned long Channel_ID = 1347787;       // Channel ID
const int Field_1 = 1;                    // Don't change
const int Field_2 = 2;                    // Don't change
const char * myWriteAPIKey = "8UxxxxxxxxxxxxxOX"; //Your Thingspeak write API key
//-------------------------------------------//

int x;
int y;
WiFiClient  client;

void setup()
{
  Serial.begin(115200);

  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo native USB port only
  }
  
  WiFi.mode(WIFI_STA);
  ThingSpeak.begin(client);
  internet();
}

void loop()
{
  internet();
  get_value();
  upload();
}


void internet()
{
  if (WiFi.status() != WL_CONNECTED)
    {
      delay(100);
      Serial.print("Attempting to connect to SSID: ");
      Serial.println(ssid);
      while (WiFi.status() != WL_CONNECTED)
      {
        WiFi.begin(ssid, pass);
        Serial.print(".");
        delay(5000);
      }
      Serial.println("\nWIFI CONNECTED.");
    }
}


const byte numChars = 32;
char receivedChars[numChars];
char tempChars[numChars];        // temporary array for use when parsing

//============ variables to hold the parsed data ============//
float value1 = 0.0;
float value2 = 0.0;

boolean newData = false;

//========================================================== //

//============

void get_value() {
  if (Serial.available() > 0)
  {
    recvWithStartEndMarkers();
    if (newData == true) {
        strcpy(tempChars, receivedChars);
            // this temporary copy is necessary to protect the original data
            //   because strtok() used in parseData() replaces the commas with \0
        parseData();
        showParsedData();
        newData = false;
    }
  }
  else
  {
    get_value();
  }
}

//============

void recvWithStartEndMarkers() {
    static boolean recvInProgress = false;
    static byte ndx = 0;
    char startMarker = '<';
    char endMarker = '>';
    char rc;

    while (Serial.available() > 0 && newData == false) {
        rc = Serial.read();
        if (recvInProgress == true) {
            if (rc != endMarker) {
                receivedChars[ndx] = rc;
                ndx++;
                if (ndx >= numChars) {
                    ndx = numChars - 1;
                }
            }
            else {
                receivedChars[ndx] = '\0'; // terminate the string
                recvInProgress = false;
                ndx = 0;
                newData = true;
            }
        }
        else if (rc == startMarker) {
            recvInProgress = true;
        }
    }
}

//============

void parseData() {      // split the data into its parts

    char * strtokIndx; // this is used by strtok() as an index

    strtokIndx = strtok(tempChars, ","); // get the first part - the string
    value1 = atof(strtokIndx);     // convert this part to an integer

    strtokIndx = strtok(NULL, ",");    // this continues where the previous call left off
    value2 = atof(strtokIndx);     // convert this part to a float

}

//============

void showParsedData() {
    Serial.print("value1: ");
    Serial.println(value1);
    Serial.print("value2: ");
    Serial.println(value2);
}


void upload()
{ 
//========== set the fields with the values
  ThingSpeak.setField(Field_1, value1);
  ThingSpeak.setField(Field_2, value2);
  if (Serial.available() > 0)
  {  
     x = ThingSpeak.writeFields(Channel_ID,myWriteAPIKey);
    //  int statusCode = ThingSpeak.getLastReadStatus();
     
     if (x == 200){
       Serial.print("Data Updated.");
      }
     else{
       Serial.print("Data upload failed, retrying....");
       upload();
      }
  }
  else{
    Serial.print("Serial Pin Connection Error!!, retrying....");
  }
//  value = "";
     delay(15000);
}
