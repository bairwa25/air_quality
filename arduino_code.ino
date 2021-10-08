/*****************************************************************
 * Board: Arduino Uno (ATMega2560)
 * Developed by: Ajay Bairwa    Email: yourfriend9014@gmail.com
 * 
 * Nodemcu-ESP8266 WiFi Module is used to connect to Internet 
 * and send data to https://thingspeak.com server 
 * 
 * Output is shown using GRAPH on thinkspeak server.
 *  
 * NOTE: 
 * Internal 10 bit ADC (A0) is used to read the Analog output of the Temperature Sensor.
********************************************************************/
/****************************************************************** 
 * 
 * PIN Diagram of ESP01: (This module works on 3.3v) (NOT 5v)
 * +---------------------------+
 * | |=| |=| |=|============== |
 * | | | | | | |    ANTENA     |
 * | | |=| |=| | Facing ur side|
 * | ========================= |
 * |                           |
 * | GND   GPIO0   GPIO2   RxD |
 * |  o      o      o       o  |
 * |                           |
 * |  o      o      o       o  |
 * | TxD   CH_PD    RST    Vcc |
 * +---------------------------+
 *
 * Connections for Communication:
 * ESP01   ->  Arduino
 * Vcc     ->  3.3V
 * GND     ->  GND
 * TxD     ->  Rx1 (Pin 10)
 * RxD     ->  Tx1 (Pin 11)
 * CH_PD   ->  3.3V
 * 
 * Serial Communication with PC through USB cable
 *****************************************************************/

 /****************************************************************
 * STEPS:
 * 1. Sign up at https://thingspeak.com
 * 2. Channels > New Channel > Update "Name" and "Field 1", "Field2" so on...-> Save Channel
 * 3. Click On API keys > Copy "Write API key"
 * 4. Make all the Connections to Arduino Mega board mentioned Above.
 * 5. Change Following in below written program.
 *    a. apiKey by above copied "Write API key" (in step 3)
 *    b. NOTE: ESP-01 is connected to wifi hotspot in a different approach using AT commands
 *       AT, AT+CWMODE=1, AT+CWJAP="BKS","sai123456", AT+CWJAP?
 * 6. Upload Program to Arduino Mega Board
 * 7. Open Arduino Serial Monitor on PC (Set Baud Rate to 9600 and set "Both NL & CR"
 * 8. Go to https://thingspeak.com and login  
 *    Channels > My Channels > Click on Channel Name (created in step 2) > Private View
 *    Here you can observe the Grapth of Temperature Vs Day.
 ****************************************************************/

//---------------------------------------------------------------------------------------------------------------
//                                                  LIBRARIES
//---------------------------------------------------------------------------------------------------------------
#include <SoftwareSerial.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16,2);

//---------------------------------------------------------------------------------------------------------------
//                                                   DEFINES VARIABLES
//---------------------------------------------------------------------------------------------------------------
int buz = 5;        //buzzer connected to pin 7
int fan = 6;         // fan connected to pin 6
int greenled = 8;   //green led connected to pin 8
int redled = 9;     //red led connected to pin 9

SoftwareSerial Ser(2, 3); // RX, TX for Nodemcu-ESP8266

//========== MQ135 Sensor variable
int gas_sensor = A0;    //Sensor pin 
float m = -0.353;       //Slope 
float c = 0.711;        //Y-Intercept 
float R0 = 23.30;       //Sensor Resistance in fresh air from previous code 21.30

//========== MQ7 Sensor variable
int CO_sensor = A1; //Sensor pin 
float m1 = -0.67; //Slope 
float c1 = 1.34; //Y-Intercept 
float R01 = 5.80; //Sensor Resistance 4.80

//---------------------------------------------------------------------------------------------------------------
//                                                  SETUP
//---------------------------------------------------------------------------------------------------------------
void setup() {
  Serial.begin(9600);     // PC to Arduino Serial Monitor
  Ser.begin(115200);      // Arduino to ESP01 Communication or 115200

  lcd.init();                      // initialize the lcd 
  lcd.backlight();
  lcd.print(" Air  Pollution ");
  lcd.setCursor(0,1);
  lcd.print(" Monitor System ");
  delay(4000);
  lcd.clear();
  
  
  pinMode(buz,OUTPUT);         // buzzer is connected as Output from Arduino
  pinMode(greenled,OUTPUT);    // green led is connected as output from Arduino
  pinMode(redled, OUTPUT);      // red led is connected as output from Arduino
  pinMode(fan, OUTPUT);
  pinMode(gas_sensor, INPUT);
  pinMode(CO_sensor,INPUT);

  //------------------// Connection of Nodemcu-ESP8266 //------------------//
  unsigned char check_connection=0;
  unsigned char times_check=0;
  Serial.println("Connecting to Wifi");
  lcd.print("Connect to Wifi.");
  delay(1000);
  lcd.clear();
  
  while(check_connection==0)
  {
     if(Ser.find("\nWIFI CONNECTED.")==1 )
     {
        Serial.println("WIFI CONNECTED.");
        lcd.print("WIFI CONNECTED.");
        delay(2000);
        lcd.clear();   
        break;
     }  
    times_check++;
    if(times_check>3) 
     {
        times_check=0;
        Serial.println("Trying to Reconnect..");
        lcd.setCursor(0,0);
        lcd.print("    Trying to   ");
        lcd.setCursor(0,1);
        lcd.print("  Reconnect...  ");
        delay(2000);
        lcd.clear();
      }
  }
}
  //------------------// Connection of Nodemcu-ESP8266 //------------------//
//---------------------------------------------------------------------------------------------------------------
//                                                  SETUP
//---------------------------------------------------------------------------------------------------------------


//---------------------------------------------------------------------------------------------------------------
//                                               MAIN LOOP
//---------------------------------------------------------------------------------------------------------------
void loop() { 
  //========== MQ135 Sensor variable
  float sensor_volt;          //Define variable for sensor voltage 
  float RS_gas;               //Define variable for sensor resistance  
  float ratio;                //Define variable for ratio
  float sensorValue = analogRead(gas_sensor);   //Read analog values of sensor  
  sensor_volt = sensorValue*(5.0/1024.0);       //Convert analog values to voltage 
  RS_gas = ((5.0*10.0)/sensor_volt)-10.0;       //Get value of RS in a gas
  ratio = RS_gas/R0;                            // Get ratio RS_gas/RS_air
  double ppm_log = (log10(ratio)-c)/m;          //Get ppm value in linear scale according to the the ratio value  
  double ppm = pow(10, ppm_log);                //Convert ppm value to log scale 
  
  Serial.print("Our Air Qualit PPM = ");
  Serial.println(ppm);
  Serial.println(ratio);
  
    
  lcd.setCursor(0,0);             // set cursor of lcd to 1st row and 1st column
  lcd.print("CO2: ");             // print message on lcd
  lcd.print(ppm);                 // print value of MQ135
  
  //========== MQ7 Sensor variable
  float sensor_volt1;             //Define variable for sensor voltage 
  float RS_gas1;                  //Define variable for sensor resistance  
  float ratio1;                   //Define variable for ratio
  float sensorValue1 = analogRead(CO_sensor);   //Read analog values of sensor  
  sensor_volt1 = sensorValue1*(5.0/1024.0);     //Convert analog values to voltage 
  RS_gas1 = ((5.0*10.0)/sensor_volt1)-10.0;     //Get value of RS in a gas
  ratio1 = RS_gas1/R01;                         // Get ratio RS_gas/RS_air
  double ppm_log1 = (log10(ratio1)-c1)/m1;      //Get ppm value in linear scale according to the the ratio value  
  double ppm1 = pow(10, ppm_log1);              //Convert ppm value to log scale 
  
  Serial.print("CO PPM = ");
  Serial.println(ppm1);

  Serial.println(analogRead(gas_sensor));
  Serial.println(analogRead(CO_sensor));
  
  lcd.setCursor(0,1);             // set cursor of lcd to 1st row and 1st column
  lcd.print("CO PPM = ");         // print message on lcd
  lcd.print(ppm1);                // print value of MQ7
  delay(5000);


  //------Sending Data to Nodemcu ESP8266--------//
    Ser.print('<'); // Starting char
    Ser.print(ppm); // float data
    Ser.print(',');
    Ser.print(ppm1); // float data
    Ser.println('>'); // Ending char
    lcd.clear();
    lcd.print("Upload to Cloud.");

     //------Check condition for buzzer and LED--------//
    if (ppm >= 10 || ppm1 >= 10) {
      digitalWrite(greenled, LOW);
      digitalWrite(buz, HIGH);
      digitalWrite(redled, HIGH);
      digitalWrite(fan, HIGH);
      lcd.setCursor(0,1);
      lcd.print("Polluted Air ಠ_ಠ");
      Serial.println("Alert!!!");
      delay(2000); // wait 2000ms
      lcd.clear();
    }
  
    else {
      digitalWrite(greenled, HIGH);
      digitalWrite(redled, LOW);
      digitalWrite(buz, LOW);
      digitalWrite(fan, LOW);
      lcd.setCursor(0,1);
      lcd.print(" Normal Air ◉‿◉ ");
      Serial.println("Normal");
      delay(2000); // wait 500ms
      lcd.clear();
    }
   //------Check condition for buzzer and LED--------//
    
  //--------------------------------------------//



 

  // thingspeak free version needs 15-20 sec delay between every push
//  delay(15000);                            // wait for 16sec
 }
