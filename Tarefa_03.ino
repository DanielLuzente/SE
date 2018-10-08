/*
Curso Mestrado em Redes de Computadores - Disciplina Software Embarcado (Prof. Francisco Sant'Anna)

Tarefa 03
  - Sensores e Atuadores (Conv. ou Comp. Analógico)
*/

#include <DHT.h>;                     //biblioteca do sensor DHT

#define LED_PIN 4                     //define led no pino 4

#define DHTPIN 2                      //define sensor DHT no pino 2
#define DHTTYPE DHT22                 // DHT 22  (AM2302)
DHT dht(DHTPIN, DHTTYPE);             // Initialize DHT sensor for normal 16mhz Arduino

int speakerPin = 8;                   //define speaker no pino 8
int flexSensorPin = A0;               //define flexSensor no pino analógico A0
float hum;                            //Stores humidity value
float temp;                           //Stores temperature value

unsigned long previousMillis = 0;
const long interval = 2000;

void setup()
{
  //inicializa pino 4 como saida e coloca estado do led apagado
  DDRD |= (1 << 4);                    //pinMode(LED_PIN, OUTPUT);
  PORTD &= ~(1 << 4);                  //digitalWrite(LED_PIN, LOW);
  
  dht.begin();
  Serial.begin(9600); 

  //Parâmetros do flexsensor em porta serial
  ADMUX   = 0;                          // usa o canal 0
  ADMUX  |= (1 << REFS0);               // usa Vcc como referencia
  ADCSRA |= (1 << ADEN);                // habilita o ADC
}

void loop()
{
  //Parâmetros do flexsensor em porta serial
  //lê estado do sensor flex e armazena na variável 
  //int flexSensorReading = analogRead(flexSensorPin); 
  ADCSRA |= (1 << ADSC);                // inicia a conversao
  while(ADCSRA & (1 << ADSC));          // espera a conversao
  int v = ADCL;                         // tem que ser lido antes do ADCH
  v = (ADCH << 8) + v;                  // valor de 2 bytes

  //O sensor varia de 10 a 1 mais ou menos, ao flexionar até o valor chegar a 4 acende o led
  if(v<=4){                             //if(flexSensorReading<=4){
    PORTD |= 1 << 4;                    //digitalWrite(LED_PIN, HIGH);
  }else{
    PORTD &= ~(1 << 4);                 //digitalWrite(LED_PIN, LOW);
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
      Serial.println(v);                  //Serial.println(flexSensorReading);
    }
}

