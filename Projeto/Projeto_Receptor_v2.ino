#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
RF24 radio(7, 8); // CE, CSN

#define LED_PIN 4 //define led no pino 4

float tempReading; //Stores temperature value
int flexSensorReading;

unsigned long previousMillis = 0;
const long interval = 2000;

const uint64_t flexpipe = 0xE8E8F0F0E1LL;
const uint64_t temppipe = 0xE14BC8F482LL;

void setup() {
  Serial.begin(9600);
  
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
  
  radio.begin();
  radio.startListening();
   
}
void loop() {
  if (radio.available()) {
    
    radio.openReadingPipe(0, flexpipe);
    radio.read(&flexSensorReading, sizeof(flexSensorReading));
    
    radio.openReadingPipe(1, temppipe);
    radio.read(&tempReading, sizeof(tempReading));
    
    if(flexSensorReading<=4){
      digitalWrite(LED_PIN, HIGH);
    }else{
      digitalWrite(LED_PIN, LOW);
    }
    Serial.println(flexSensorReading);
    Serial.println(tempReading);
  }
}
