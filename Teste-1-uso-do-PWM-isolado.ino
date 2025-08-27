// === Declaração de pinos ===
const int motorA_EN = 1;  // frente esquerda
const int motorA_PWM = 2;

const int motorB_EN = 4;  // frente direita
const int motorB_PWM = 3;

const int motorC_EN = 5;  // trás esquerda
const int motorC_PWM = 6;

const int motorD_EN = 8;  // trás direita
const int motorD_PWM = 7;

// === Velocidade base ===
//int x = 0;

void setup() {
  pinMode(motorA_EN, OUTPUT);
  pinMode(motorA_PWM, OUTPUT);
  pinMode(motorB_EN, OUTPUT);
  pinMode(motorB_PWM, OUTPUT);
  pinMode(motorC_EN, OUTPUT);
  pinMode(motorC_PWM, OUTPUT);
  pinMode(motorD_EN, OUTPUT);
  pinMode(motorD_PWM, OUTPUT);
}

void loop() {
  
  // Acelera de 0 a 255
  for (int x = 75; x <= 255; x++) {
    setMotor(motorA_EN, motorA_PWM, x);
    setMotor(motorB_EN, motorB_PWM, x);
    setMotor(motorC_EN, motorC_PWM, x);
    setMotor(motorD_EN, motorD_PWM, x);
    delay(20);
  }

  // Desacelera de 255 a 0
  for (int x = 255; x >= 75; x--) {
    setMotor(motorA_EN, motorA_PWM, x);
    setMotor(motorB_EN, motorB_PWM, x);
    setMotor(motorC_EN, motorC_PWM, x);
    setMotor(motorD_EN, motorD_PWM, x);
    delay(20);
  }
 
  // Desacelera no sentido contrário (reverso)
  for (int x = -75; x >= -250; x--) {
    setMotor(motorA_EN, motorA_PWM, x);
    setMotor(motorB_EN, motorB_PWM, x);
    setMotor(motorC_EN, motorC_PWM, x);
    setMotor(motorD_EN, motorD_PWM, x);
    delay(20);
  }

  // Acelera de -255 até 0
  for (int x = -250; x <= -75; x++) {
    setMotor(motorA_EN, motorA_PWM, x);
    setMotor(motorB_EN, motorB_PWM, x);
    setMotor(motorC_EN, motorC_PWM, x);
    setMotor(motorD_EN, motorD_PWM, x);
    delay(20);
  }  
 
  
 delay(2000);
}

// === Função de controle PWM com sentido ===
void setMotor(int enPin, int pwmPin, int velocidade) {
  if (velocidade > 0) {
    digitalWrite(enPin, LOW);              // Sentido horário
    analogWrite(pwmPin, velocidade);
  } else if (velocidade < 0) {
    digitalWrite(enPin, LOW);               // Sentido anti-horário
    analogWrite(pwmPin, abs(velocidade));
  } else {
    analogWrite(pwmPin, 0);                 // Parado
  }
}

