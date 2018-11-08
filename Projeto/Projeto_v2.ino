/*
Curso Mestrado em Redes de Computadores - Disciplina Software Embarcado (Prof. Francisco Sant'Anna)
Projeto
  - Aplicação usando dois sensores e dois atuadores com comunicação RF
*/

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
RF24 radio(7, 8); // CE, CSN

const uint64_t flexpipe = 0xE8E8F0F0E1LL;
const uint64_t temppipe = 0xE14BC8F482LL;

#include <DHT.h>; //biblioteca do sensor DHT


#define DHTPIN 2     //define sensor DHT no pino 2
#define DHTTYPE DHT22   // DHT 22  (AM2302)
DHT dht(DHTPIN, DHTTYPE); //// Initialize DHT sensor for normal 16mhz Arduino

int speakerPin = 8; //define speaker no pino 8
int flexSensorPin = A0; //define flexSensor no pino analógico A0

float tempReading; //Stores temperature value
int flexSensorReading;

unsigned long previousMillis = 0;
const long interval = 2000;

void setup()
{
  radio.begin();
  dht.begin();
  Serial.begin(9600); 
}
  
void loop()
{
  //lê estado do sensor flex e armazena na variável 
  flexSensorReading = analogRead(flexSensorPin); 
  
  //Lê dados do sensor e armazena na variável de temperatura
  tempReading= dht.readTemperature();

  //Emite som na frequência de 1000 Hertz (ciclos por segundo) por 500ms
  //enquanto humidade estiver acima de 95% e temperatura acima de 30 graus
  while(tempReading >= 30){
    tempReading= dht.readTemperature();
    tone(speakerPin, 10, 50); 
  }
  
  //Mostra valores medidos pelo sensor de umidade, temperatura e do flex sensor
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= interval){
      previousMillis = currentMillis;
      Serial.print("Temp: ");
      Serial.print(tempReading);
      Serial.println(" Celsius");
      Serial.println(flexSensorReading);
    }
        
    radio.openWritingPipe(flexpipe);
    radio.stopListening();
    radio.write(&flexSensorReading, sizeof(flexSensorReading));
    
    radio.openWritingPipe(temppipe);
    radio.stopListening();
    radio.write(&tempReading, sizeof(tempReading));
}
