/*
Curso Mestrado em Redes de Computadores - Disciplina Software Embarcado (Prof. Francisco Sant'Anna)
Projeto
  - Projeto Final: Automatização e Otimização de Ambiente de Datacenter
*/
#include <avr/interrupt.h>      //Biblioteca para interrupções
#include <SPI.h>                //Biblioteca para comunicação serial
#include <nRF24L01.h>           //Biblioteca para rádiofrequência
#include <RF24.h>               //Biblioteca para rádiofrequência
RF24 radio(7, 8);               // CE, CSN
#include <DHT.h>                //Biblioteca do sensor DHT
#define DHTPIN 4                //Sensor DHT no pino 2
#define DHTTYPE DHT22           // DHT 22  (AM2302)
DHT dht(DHTPIN, DHTTYPE);       // Initialize DHT sensor for normal 16mhz Arduino

const uint64_t pipe = 0xF0F0F0F0F0E1;  //Canal único para transmissão via rádio

int pinSensorPIR = 5;                       //Sensor PIR no pino 5
const int havePeopleBtn = 3;                //Botão entrada no pino 3
const int haveNotPeopleBtn = 2;             //Botão saída no pino 2
int lastButton1State = LOW;                 //Variável para guardar a última leitura do botão entrada
int lastButton2State = LOW;                 //Variável para guardar a última leitura do botão saída
unsigned long lastDebounceTime = 0;         //Variável para debouncing do botão entrada
unsigned long lastDebounceTimeButton2 = 0;  //Variável para debouncing do botão saída
unsigned long debounceDelay = 50;           //Tempo de debouncing para o pressionar do botão
int readingBtn1 = 0;                        //Variável para guardar valor do botão de entrada
int readingBtn2 = 0;                        //Variável para guardar valor do botão de saída
int speakerPin = 8;                         //Define speaker no pino 8
int flexSensorPin = A0;                     //Define flexSensor no pino analógico A0

//Dataset para armazenar dados dos sensores que serão transmitidos via RF
typedef struct{
  float tempReading; //Stores temperature value
  int flexSensorReading;
  int havePeople;
  int haveNotPeople;
  int valorSensorPIR;
}
A_t;

A_t dataset;

//Intervalos para uso do Millis em delay de tempo
unsigned long previousMillis = 0;
const long interval1 = 1000;
const long interval2 = 1500;
const long interval3 = 10000;

void setup()
{
  radio.begin();                      //Inicializa funções de rádio
  radio.setDataRate( RF24_250KBPS );  //Limita taxa de transferência em 250KBPS
  
  dht.begin();                        //Inicializa funções do sensor DHT

  pinMode(pinSensorPIR,INPUT);        //Sensor de presença
  pinMode(havePeopleBtn, INPUT);      //Botão de entrada com interrupção
  EICRA = (1 << ISC11);               //INT1 modo RISING
  EIMSK = (1 << INT1);                //Habilita external interrupt INT1
  pinMode(haveNotPeopleBtn, INPUT);   //Botão de saida sem interrupção
  
  radio.openWritingPipe(pipe);        //Abre canal de rádio para escrita
  Serial.begin(9600); 
}
  
void loop()
{
  int readingBtn2 = digitalRead(haveNotPeopleBtn);     //Leitura do botão de saida feita a cada loop
  
  if (readingBtn1 != lastButton1State) {
    // reset the debouncing timer
    lastDebounceTime = millis();
  }
  if (readingBtn2 != lastButton2State) {
    // reset the debouncing timer
    lastDebounceTimeButton2 = millis();
  }
  
  if (readingBtn1 == 1) {
    dataset.havePeople = readingBtn1;         //Grava no dataset se botão de entrada foi pressionado por interrupção
  }

//Respeita debounce antes de trocar variável do botão de saída
  if ((millis() - lastDebounceTimeButton2) > debounceDelay) {
    if (readingBtn2 != dataset.haveNotPeople) {
      dataset.haveNotPeople = readingBtn2;
      readingBtn1 =0;                         //Troca valor da variável do botão de entrada
      dataset.havePeople = readingBtn1;       //Troca valor da variável no dataset também
    }
  }
  
  //lê estado do sensor flex e armazena na variável 
  dataset.flexSensorReading = analogRead(flexSensorPin); 
  
  //Lê dados do sensor e armazena na variável de temperatura
  dataset.tempReading= dht.readTemperature();

  //Lê e grava estado do PIR
  dataset.valorSensorPIR = digitalRead(pinSensorPIR);

  //Mostra valores medidos para debugging
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval1){
    previousMillis = currentMillis;
    Serial.print("Temp: ");
    Serial.print(dataset.tempReading);
    Serial.print("Flex: ");
    Serial.println(dataset.flexSensorReading);
    Serial.print("Btn1: ");
    Serial.println(readingBtn1);
    Serial.print("Btn2: ");
    Serial.println(readingBtn2);
    Serial.print("Valor do Sensor PIR: ");  
    Serial.println(dataset.valorSensorPIR);
  }

    //Envia os dados via RF
    radio.stopListening();
    radio.write(&dataset, sizeof(dataset));  
    radio.startListening();

    //Guarda último estado dos botões entrada e saída
    lastButton1State = readingBtn1;
    lastButton2State = readingBtn2;
}

//Interrupção do botão de entrada
ISR(INT1_vect)
{
  if ((millis() - lastDebounceTime) > debounceDelay) {
    readingBtn1 = 1;
  }
}
