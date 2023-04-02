#include <SoftwareSerial.h>
#include <TinyGPS++.h>

TinyGPSPlus gps;
int IRSensor = 2; // CONNECT IR SESNOR TO ARDUINO PIN 2
int Flame_Sensor = 12; // CONNECT FLAME SENSOR TO ARDUINO PIN 3
int Red_LED = 8; // CONNECT RED LED TO ARDUINO PIN 8;
SoftwareSerial mySerial(10,11); // CONNECT RX OF GSM TO 11 & TX OF GSM TO 10
SoftwareSerial ss(4, 3); // The serial connection to the GPS device
bool crash;
bool fire;

void setup() 
{
  mySerial.begin(9600); // SETTING BAUD RATE OF GSM MODULE
  Serial.begin(9600); // SETTING BAUD RATE OF SERIAL MONITOR 
  ss.begin(9600); //SETTING BAUD RATE OF GSM MODULE
  delay(100);
  pinMode(IRSensor, INPUT);
  pinMode(Flame_Sensor, INPUT);
  pinMode(Red_LED, OUTPUT);
}

void location(){
  gps.encode(ss.read());
  double a = gps.location.lat();
  double b = gps.location.lng();
  Serial.print("Latitude= "); 
  Serial.print(a);
  Serial.println(" Longitude= "); 
  Serial.print(b);
}

void turn_on(){
  digitalWrite(Red_LED, HIGH);  // turn the LED on (HIGH is the voltage level)
}

void turn_off(){
  digitalWrite(Red_LED, LOW);  // turn the LED on (HIGH is the voltage level)
}

void Crash_Alert()
{
  double a = gps.location.lat();
  double b = gps.location.lng();
  mySerial.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode
  delay(1000);  // Delay of 1000 milli seconds or 1 second
  mySerial.println("AT+CMGS=\"+919354737630\"\r"); 
  delay(1000);
  mySerial.println("Accident Detected at the location: ");
  mySerial.print("Latitude= "); 
  mySerial.print(a);
  mySerial.print(" Longitude= "); 
  mySerial.print(b);
  Serial.println("Accident Detected at the location: ");
  Serial.print("Latitude= "); 
  Serial.print(a);
  Serial.print(" Longitude= "); 
  Serial.print(b);
  delay(100);
  mySerial.println((char)26);
  delay(1000);
} 


void Fire_Alert()
{
  double a = gps.location.lat();
  double b = gps.location.lng();
  mySerial.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode
  delay(1000);  // Delay of 1000 milli seconds or 1 second
  mySerial.println("AT+CMGS=\"+919518032832\"\r"); 
  delay(1000);
  mySerial.println("Vehicle is under fire at the location: ");
  mySerial.print("Latitude= "); 
  mySerial.print(a);
  mySerial.print(" Longitude= "); 
  mySerial.print(b);
  Serial.println("Vehicle is under fire at the location: ");
  Serial.print("Latitude= "); 
  Serial.print(a);
  Serial.print(" Longitude= "); 
  Serial.print(b);
  delay(100);
  mySerial.println((char)26);
  delay(1000);
}

void loop() 
{
  crash = digitalRead(IRSensor);
  fire = digitalRead(Flame_Sensor);
  
  if (crash == 0 && fire == 0){
    Serial.println("all okay");
    turn_off();
  }
  else if (crash == 1 && fire == 1){
    Serial.println("total devastation");
    turn_on();
    Fire_Alert();
    Crash_Alert();
  } else {
    if(crash == 1){
      Serial.println("Crash");
      turn_on();
      Crash_Alert();
    } else if(fire == 1) {
      turn_on();
      Serial.println("Fire");
      Fire_Alert();
    }
  }
}
