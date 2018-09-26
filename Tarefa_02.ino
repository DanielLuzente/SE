/*
Curso Mestrado em Redes de Computadores - Disciplina Software Embarcado (Prof. Francisco Sant'Anna)

Tarefa 02
  - Aplicação usando dois sensores e dois atuadores
*/

#include <DHT.h>; //biblioteca do sensor DHT

#define LED_PIN 4 //define led no pino 4

#define DHTPIN 2     //define sensor DHT no pino 2
#define DHTTYPE DHT22   // DHT 22  (AM2302)
DHT dht(DHTPIN, DHTTYPE); //// Initialize DHT sensor for normal 16mhz Arduino

int speakerPin = 8; //define speaker no pino 8
int flexSensorPin = A0; //define flexSensor no pino analógico A0
int chk;
float hum;  //Stores humidity value
float temp; //Stores temperature value

void setup()
{
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
  dht.begin();
  Serial.begin(9600); 
}

void loop()
{
  //lê estado do sensor flex e armazena na variável 
  int flexSensorReading = analogRead(flexSensorPin); 
  Serial.println(flexSensorReading);

  //O sensor varia de 10 a 1 mais ou menos, ao flexionar até o valor chegar a 4 acende o led
  if(flexSensorReading<=4){
    digitalWrite(LED_PIN, HIGH);
  }else{
    digitalWrite(LED_PIN, LOW);
  }
  
  //Lê dados do sensor e armazena nas variáveis de humidade e temperatura
  hum = dht.readHumidity();
  temp= dht.readTemperature();

  //Emite som na frequência de 1000 Hertz (ciclos por segundo) por 500ms
  //enquanto humidade estiver acima de 95% e temperatura acima de 30 graus
  while(hum >= 80 || temp >= 30){
    hum = dht.readHumidity();
    temp= dht.readTemperature();
    tone(speakerPin, 10, 50); 
  }
  
  //Mostra valores medidos pelo sensor de umidade e temperatura
    Serial.print("Humidity: ");
    Serial.print(hum);
    Serial.print(" %, Temp: ");
    Serial.print(temp);
    Serial.println(" Celsius");
    delay(2000); 
}

