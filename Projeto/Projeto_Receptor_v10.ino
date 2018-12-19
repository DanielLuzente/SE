
#include "pitches.h"         //Arquivo de tons para o speaker
#include <SPI.h>             //Biblioteca para comunicação serial
#include <nRF24L01.h>        //Biblioteca para rádiofrequência
#include <RF24.h>            //Biblioteca para rádiofrequência
RF24 radio(7, 8);            // CE, CSN

//Pinos dos leds
#define LED_SETOR1_1 5
#define LED_SETOR1_2 4
#define LED_SETOR2_1 3
#define LED_SETOR2_2 6
//Pinos dos leds que simulam Ar Condicionado
#define LED_AC_1 9
#define LED_AC_2 10

int relay = 19;              //Pino do Relé
int doorOpen = 0;            //Variável para guardar estado da porta do Datacenter
int lightFull = 0;           //Variável para guardar estado de todas as luzes acesas
int speakerPin = 2;          //Define speaker no pino 8

//Dataset para receber os valores dos sensores capturados no outro Arduino
typedef struct{
  float tempReading;
  int flexSensorReading;
  int havePeople;
  int haveNotPeople;
  int valorSensorPIR;
}
A_t;

A_t dataset;

//Intervalos para uso do Millis em delay de tempo
unsigned long previousMillis = 0;
unsigned long previousMillisPIR = 0;
const long interval1 = 1000;
const long interval2 = 5000;

const uint64_t pipe = 0xF0F0F0F0F0E1;    //Canal único para transmissão via rádio

void setup() {
  Serial.begin(9600);
  pinMode(relay, OUTPUT);
  pinMode(LED_SETOR1_1, OUTPUT);
  pinMode(LED_SETOR1_2, OUTPUT);
  pinMode(LED_SETOR2_1, OUTPUT);
  pinMode(LED_SETOR2_2, OUTPUT);
  pinMode(LED_AC_1, OUTPUT);
  pinMode(LED_AC_2, OUTPUT);
  digitalWrite(LED_SETOR1_1, LOW);
  digitalWrite(LED_SETOR1_2, LOW);
  digitalWrite(LED_SETOR2_1, LOW);
  digitalWrite(LED_SETOR2_2, LOW);
  digitalWrite(LED_AC_1, LOW);
  digitalWrite(LED_AC_2, LOW);
  
  radio.begin();                       //Inicializa funções de rádio
  radio.setDataRate( RF24_250KBPS );   //Limita taxa de transferência em 250KBPS
  radio.openReadingPipe(1, pipe);      //Abre o canal de comunicação para leitura dos dados em buffer
  radio.startListening();              //Inicia a escuta do canal
  
}
void loop() {
  //Realiza leitura dos dados no buffer do RF24 e grava no dataset  
  if (radio.available()) {
    radio.read(&dataset, sizeof(dataset));
  }
  
  //Mostra valores medidos para debugging
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval1) {
    previousMillis = currentMillis;
    Serial.print("flexSensorReading: ");
    Serial.println(dataset.flexSensorReading); 
    Serial.print("tempReading: "); 
    Serial.println(dataset.tempReading); 
    Serial.print("havePeople: "); 
    Serial.println(dataset.havePeople);
    Serial.print("haveNotPeople: "); 
    Serial.println(dataset.haveNotPeople);
    Serial.print("valorSensorPIR: ");
    Serial.println(dataset.valorSensorPIR);
  }

  //Controla os dois ACs baseado na temperatura e soa dois níveis de beeps
    if(dataset.tempReading >= 32.60 && dataset.tempReading < 32.70){
      digitalWrite(LED_AC_2, LOW);
      digitalWrite(LED_AC_1, HIGH);
      tone(speakerPin, 4978, 250);
      delay(1500);
      noTone(speakerPin);
    } else if (dataset.tempReading >= 32.80){
      digitalWrite(LED_AC_1, HIGH);
      digitalWrite(LED_AC_2, HIGH);
      tone(speakerPin, 4978, 250);
      delay(500);
      noTone(speakerPin);
    } else {
      digitalWrite(LED_AC_1, LOW);
      digitalWrite(LED_AC_2, LOW);
    }
   
    if(dataset.havePeople == 1){
      doorOpen = 1;
    }

    //Verifica se botão de entrada foi pressionado e se porta foi aberta flexionando o sensor
    if(dataset.flexSensorReading<=350 && doorOpen == 1 && lightFull == 0){
      lightFull = 1;                     //Seta variável para ele não entrar nesse If novamente
      digitalWrite(LED_SETOR1_1, HIGH);  //Liga iluminação total do ambiente
      digitalWrite(LED_SETOR1_2, HIGH);
      digitalWrite(LED_SETOR2_1, HIGH);
      digitalWrite(LED_SETOR2_2, HIGH);
      delay(5000);
      digitalWrite(LED_SETOR1_1, LOW);   //Desliga metade da iluminação do ambiente após 5 segundos
      digitalWrite(LED_SETOR2_2, LOW);
    }

  //Captura movimento no SETOR1 apenas se tiver gente dentro
  if (dataset.valorSensorPIR == 1 && doorOpen == 1 && lightFull == 1){
    previousMillisPIR = millis();
    digitalWrite(LED_SETOR1_1, HIGH);
  }

  //Apaga iluminação setorial se decorrido intervalo sem qualquer movimento
  unsigned long currentMillisPIR = millis();
  if (dataset.valorSensorPIR == 0 && doorOpen == 1 && lightFull == 1 && currentMillisPIR - previousMillisPIR >= interval2){
    digitalWrite(LED_SETOR1_1, LOW);    
  }

  //Apaga toda a iluminação quando botão de saída for pressionado
  if(dataset.haveNotPeople == 1){
      doorOpen = 0;
      lightFull = 0;
      digitalWrite(LED_SETOR1_1, LOW);
      digitalWrite(LED_SETOR1_2, LOW);
      digitalWrite(LED_SETOR2_1, LOW);
      digitalWrite(LED_SETOR2_2, LOW);
    }
}

