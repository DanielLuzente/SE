/*
Curso Mestrado em Redes de Computadores - Disciplina Software Embarcado (Prof. Francisco Sant'Anna)
Tarefa 05
  - Aplicação usando dois sensores e dois atuadores + timer
*/

#include <DHT.h>; //biblioteca do sensor DHT

#define LED_PIN 4 //define led no pino 4

#define DHTPIN 2     //define sensor DHT no pino 2
#define DHTTYPE DHT22   // DHT 22  (AM2302)
DHT dht(DHTPIN, DHTTYPE); //// Initialize DHT sensor for normal 16mhz Arduino

int speakerPin = 8; //define speaker no pino 8
int flexSensorPin = A0; //define flexSensor no pino analógico A0
float hum;  //Stores humidity value
float temp; //Stores temperature value

unsigned long previousMillis = 0;
const long interval = 2000;

void setup()
{
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
  dht.begin();
  Serial.begin(9600); 

  // Inicializa Timer1 
  noInterrupts();           // desliga interrupções
  TCCR1A = 0;
  TCCR1B = 0;
  TCCR1B |= (1 << CS10)|(1 << CS12);    // prescaler 1024
  TCNT1 = 0xC2F7;
  TIMSK1 |= (1 << TOIE1);
  interrupts();             // liga as interrupções
}

ISR(TIMER1_OVF_vect)          // rotina de interrupção de timer compare
{
  TCNT1 = 0xC2F7;
  digitalWrite(LED_PIN, digitalRead(LED_PIN) ^ 1);   // toggle LED pin
}
void loop()
{
  //lê estado do sensor flex e armazena na variável 
  int flexSensorReading = analogRead(flexSensorPin); 
  
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
  
  //Mostra valores medidos pelo sensor de umidade, temperatura e do flex sensor
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= interval){
      previousMillis = currentMillis;
      Serial.print("Humidity: ");
      Serial.print(hum);
      Serial.print(" %, Temp: ");
      Serial.print(temp);
      Serial.println(" Celsius");
      Serial.println(flexSensorReading);
    }
}
