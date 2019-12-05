#include <WiFiClient.h>
#include <WiFiUdp.h>
#include <WiFi.h>
#include <WiFiServer.h>

#include <ThingSpeak.h>
#include <LiquidCrystal.h>

#define dht_dpin A0 // THIS NEXT PINS CAN BE CHANGED IF NEEDED.


LiquidCrystal lcd(8, 9, 4, 5, 6, 7);
// define some values used by the panel and buttons
int lcd_key = 0;
int adc_key_in = 0;
#define btnRIGHT 0
#define btnUP 1
#define btnDOWN 2
#define btnLEFT 3
#define btnSELECT 4
#define btnNONE 5

// These constants won't change:
 float  sensorPin = A1;    // pin that the sensor is attached to
const int ledPin = 7;        // pin that the LED is attached to
float celsius = 0;
float fahrenheit = 0;

//Wifi channel and Key ID
const unsigned long channel_id = 885680;
const char write_api_key[] = "I74RF2Q0K2ABXNDG";

//Login and Password
char ssid[] = "cs";
const char password[] = "gkmrxuW6";

WiFiClient client;

void setup() {
//wifi connection
  Serial.begin(9600);
  delay(100);

  Serial.print("Connecting to  ");
  lcd.println("Connecting      ");
  Serial.print(ssid);
  
//boot up wifi
 WiFi.begin (ssid, password);
 while ( WiFi.status() != WL_CONNECTED ) {
  delay(500);
  Serial.print(".");
 }
 //if connection found
Serial.println();
Serial.println("Connected!");
lcd.println("Connected!");

ThingSpeak.begin(client);

  
  // turn on LED to signal the start of the calibration period:
  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);

//pinMode(photocell, INPUT); // we will be reading the photocell pin
  Serial.begin(9600); // starts serial communication at 9,600 baud (the rate)
  
  
  pinMode(ledPin, OUTPUT);        // Makes the pin 13 output
  digitalWrite(ledPin, LOW);       // Turn off the board led attached to pin 13 off.
  lcd.begin(16, 2);               // Starts the LCD
  lcd.clear();                    // Clears the LCD
  lcd.display();                  // Turns the LCD on
  lcd.print("Light M");
    }//end "setup()"
                   

void findTemps()
{
float voltage = 0;
float sensor = 0;
sensor = analogRead(A2);
voltage = (sensor * 5000) / 1024; // convert the raw sensor value to millivolts
voltage = voltage - 500; // remove the voltage offset
celsius = voltage / 10; // convert millivolts to Celsius
}

void displayTemps()
{
Serial.print("Temperature is ");
Serial.print(celsius, 2);
Serial.println(" deg. C / ");
}


void loop() {
  // read the sensor:

  float sensor_light = analogRead(A1);
float voltage_light = (sensor_light * 5000) / 1024; 
float voltage = voltage_light - 500; 
float light_data = voltage/ 10; 

  lcd.setCursor(0, 0);            // Puts the cursor on the first digit of the first lineline
  lcd.println("Light M         ");  // Writes the first line
  lcd.setCursor(0, 1);            // Puts the cursor on the first digit of the second line
  lcd.print(light_data);    // Writes the second line
  delay(2000);      // Wait rest of 2000ms recommended delay before accessing sensor  
  Serial.println(" Light"); 
  Serial.print(light_data);

Serial.println("Posting " + String(light_data ) + " to ThingSpeak");
ThingSpeak.setField(1, String(light_data));

delay(10000);


         
  findTemps();
displayTemps();
delay(2000);
lcd.setCursor(0, 0);            // Puts the cursor on the first digit of the first lineline
  lcd.println("Temp:         ");  // Writes the first line
  lcd.setCursor(0, 1);            // Puts the cursor on the first digit of the second line
  lcd.print(celsius );    // Writes the second line
  delay(2000);      // Wait rest of 2000ms recommended delay before accessing sensor  

  Serial.println("Posting " + String(celsius) + " to ThingSpeak");
ThingSpeak.setField(2, String(celsius));

ThingSpeak.writeFields(channel_id, write_api_key);

delay(10000);
}  
