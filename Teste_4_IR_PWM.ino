// === Declaração de pinos ===

const int motorA_EN = 1;  // Frente esquerda
const int motorA_PWM = 2;

const int motorB_EN = 4;  // Frente direita
const int motorB_PWM = 3;

const int motorC_EN = 5;  // Trás esquerda
const int motorC_PWM = 6;

const int motorD_EN = 8;  // Trás direita
const int motorD_PWM = 7;

const int sensorPin = 17; // Pino do sensor infravermelho

void setup() {
  Serial.begin(9600);

  // Motores
  pinMode(motorA_EN, OUTPUT);
  pinMode(motorA_PWM, OUTPUT);
  pinMode(motorB_EN, OUTPUT);
  pinMode(motorB_PWM, OUTPUT);
  pinMode(motorC_EN, OUTPUT);
  pinMode(motorC_PWM, OUTPUT);
  pinMode(motorD_EN, OUTPUT);
  pinMode(motorD_PWM, OUTPUT);

  // Sensor
  pinMode(sensorPin, INPUT);
}

void loop() {
  int valorSensor = digitalRead(sensorPin);

  // Se não detectar linha (valor HIGH no KeyesIR), o robô acelera
  if (valorSensor == HIGH) {
    Serial.println("Sem linha detectada!");
    for (int x = 75; x <= 255; x++) {
      setMotor(motorA_EN, motorA_PWM, x);
      setMotor(motorB_EN, motorB_PWM, x);
      setMotor(motorC_EN, motorC_PWM, x);
      setMotor(motorD_EN, motorD_PWM, x);
      delay(20); 
    }
  }
  // Se linha detectada, desacelera
  else {
    Serial.println("Linha detectada!");
    for (int x = 255; x >= 75; x--) {
      setMotor(motorA_EN, motorA_PWM, x);
      setMotor(motorB_EN, motorB_PWM, x);
      setMotor(motorC_EN, motorC_PWM, x);
      setMotor(motorD_EN, motorD_PWM, x);
      delay(20); 
    }
  }

  delay(100);
}

// Função para aplicar PWM e direção

void setMotor(int enPin, int pwmPin, int velocidade) {
  int veloAbs = abs(velocidade);
  if (velocidade > 0) {
    analogWrite(enPin, 0);          // Sentido horário
    analogWrite(pwmPin, veloAbs);
  } else if (velocidade < 0) {
    analogWrite(enPin, veloAbs);    // Sentido anti-horário
    analogWrite(pwmPin, 0);
  } else {
    analogWrite(enPin, 0);          // Parado
    analogWrite(pwmPin, 0);
  }
}
