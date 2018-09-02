/*
Curso Mestrado em Redes de Computadores - Disciplina Software Embarcado (Prof. Francisco Sant'Anna)

Tarefa 01
  - Piscar o LED a cada 1 segundo
  - Botão 1: Acelerar o pisca-pisca a cada pressionamento (somente ao pressionar e não ao soltar)
  - Botão 2: Desacelerar a cada pressionamento (somente ao pressionar e não ao soltar)
  - Botão 1+2 (em menos de 500ms): Parar
*/

const int but2 = 2; //define identificador do pino 2
const int but4 = 4; //define identificador do pino 4
const int ledPin = LED_BUILTIN; //define identificador do led onboard
int ledState = 0; // variável para guardar o estado do led
int stopBlink = 0; //variável para registrar que o led ficará parado
unsigned long previousMillis = 0; //variável para guardar tempo inicial
unsigned long button2Millis = 0; //variável para guardar tempo do botão 2 pressionado
unsigned long button4Millis = 0; //variável para guardar tempo do botão 4 pressionado
int interval = 1000; //variável para guardar intervalo do blink
int button2State = 0; //variável para guardar estado do botão 2
int button4State = 0;//variável para guardar estado do botão 4

void setup() {
  pinMode(ledPin, OUTPUT); //seta led onboard como output
  pinMode(but2, INPUT); //seta botão do pino 2 como input
  pinMode(but4, INPUT); //seta botão do pino 4 como input
  //Serial.begin(9600);
}

void loop() {
  unsigned long currentMillis = millis(); //variável para guardar tempo atual
  int button2State = digitalRead(but2); //lê pino 2 para saber se botão foi pressionado
  int button4State = digitalRead(but4); //lê pino 4 para saber se botão foi pressionado
  
  if (stopBlink == 0 && currentMillis - previousMillis >= interval){ //teste do intervalo de tempo para definir se pisca ou não pisca

    previousMillis = currentMillis; //troca tempo inicial pelo atual

    //Pisca o led
    if (ledState == LOW) {
     ledState = HIGH;
    } else {
    ledState = LOW;
    }
    
  digitalWrite(ledPin, ledState); //guarda estado do led

  //acelera pisca mudando intervalo e guarda o tempo do pressionamento deste botão
    if (button2State == HIGH){
      button2Millis = millis();
      if (interval >= 200){
        interval = interval - 150;
      }
    }

//desacelera pisca mudando intervalo e guarda o tempo do pressionamento deste botão
    if (button4State == HIGH){
      button4Millis = millis();
      if (button2State == LOW && interval <= 900){
        interval = interval + 100 ;
    }

//verifica se os dois botões foram pressionados juntos e para o blink se o intervalo for menor de 500ms
    if (button2State == HIGH && button4State == HIGH){
      if (button2Millis - button4Millis < 500 || button4Millis - button2Millis < 500){
        interval = 0; //para o blink
        stopBlink = 1; //define que vai ficar sem piscar pra sempre
        digitalWrite(ledPin, HIGH);
      }
    }
  }
  //Serial.println(interval);
}
}
