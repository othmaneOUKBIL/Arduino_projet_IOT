//Libraries
#include <DHT.h>;
//I2C LCD :
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
LiquidCrystal_I2C lcd(0x27,16,2) ;
#include <SPI.h>
#include <MFRC522.h>
#include <Servo.h>
 
#define SS_PIN 10
#define RST_PIN 9
MFRC522 mfrc522(SS_PIN, RST_PIN) ; // Create MFRC522 instance.
Servo myServo; //define servo name
//Constants
#define DHTPIN 2    // what pin we're connected to
#define DHTTYPE DHT11   // DHT 11
DHT dht(DHTPIN, DHTTYPE); //// Initialize DHT sensor for normal 16mhz Arduino
//Variables
//int chk;
int h;  //Stores humidity value
int t; //Stores temperature value
int ledPin = 3;
int mouvPin = 7;
void setup()
{
    Serial.begin(9600);
    Serial.println("Temperature and Humidity Sensor Test");
    dht.begin();
    lcd.init(); //initialize the lcd
    lcd.backlight(); //open the backlight
    pinMode(ledPin, OUTPUT);
    pinMode(mouvPin, INPUT);
    digitalWrite(ledPin, LOW);
    Serial.begin(9600);   // Initiate a serial communication
    SPI.begin();      // Initiate  SPI bus
    mfrc522.PCD_Init();   // Initiate MFRC522
    myServo.attach(8); //servo pin
    myServo.write(0); //servo start position
    Serial.println("Put your card to the reader...");
    Serial.println();
}
void led(){
     if(digitalRead(mouvPin) == HIGH)
  {
    digitalWrite(ledPin, HIGH);
  }
 
  else
  {
    digitalWrite(ledPin, LOW);
  }
  }
void lcd1(){
    //Read data and store it to variables h (humidity) and t (temperature)
    // Reading temperature or humidity takes about 250 milliseconds!
    h = dht.readHumidity();
    t = dht.readTemperature();
    
    //Print temp and humidity values to serial monitor
    Serial.print("Humidity: ");
    Serial.print(h);
    Serial.print(" %, Temp: ");
    Serial.print(t);
    Serial.println(" ° Celsius");
        
// set the cursor to (0,0):
// print from 0 to 9:
 
    lcd.setCursor(0, 0);
    lcd.println(" Now Temperature ");
    
    lcd.setCursor(0, 1);
    lcd.print("T:");
    lcd.print(t);
    lcd.print("C");
 
    lcd.setCursor(6, 1);
    lcd.println("2022");
     
    lcd.setCursor(11, 1);
    lcd.print("H:");
    lcd.print(h);
    lcd.print("%");
  } 
void serv(){
 
 
  // Look for new cards
  if ( ! mfrc522.PICC_IsNewCardPresent()) 
  {
    return;
  }
  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) 
  {
    return;
  }
  //Show UID on serial monitor
  Serial.print("UID tag :");
  String content= "";
  byte letter;
  for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
     Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
     Serial.print(mfrc522.uid.uidByte[i], HEX);
     content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  Serial.println();
  Serial.print("Message : ");
  content.toUpperCase();
  if (content.substring(1) == "43 51 0C 05" or "EC FC B9 6D") //Your own UID card
  {
    Serial.println("Authorized access");
    Serial.println();
    delay(500);
    myServo.write(90);
    delay(5000);
    myServo.write(0);
  }
else   {
    Serial.println(" Access denied");
  } 
  }  
void loop()
{
  led();
  serv();
  lcd1(); 
  delay(1000); //Delay 1 sec.
}

