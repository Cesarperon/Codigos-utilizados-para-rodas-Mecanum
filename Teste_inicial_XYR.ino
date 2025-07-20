//#include <pico/stdlib.h>

// === Declaração de pinos ===
// === Frente do robô é a pilha ===

const int motorA_EN = 1;  // Frente esquerda
const int motorA_PWM = 2;

const int motorB_EN = 4;  // Frente direita
const int motorB_PWM = 3;

const int motorC_EN = 5;  // Trás esquerda
const int motorC_PWM = 6;

const int motorD_EN = 8;  // Trás direita
const int motorD_PWM = 7;

int N = 75; // valor minimo que o motor funciona

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

// === Variáveis de movimento ===


  int Lx = 0;  // Esquerda/Direita
  int Ly = 0;  // Frente/Trás
  int Rt = 1;  // Rotação
  
// === Possível utilização depois ===

// int Lx = Serial.read(); 
// int Ly = Serial.read(); 
// int Rt = Serial.read();

  float LF = Ly + Lx + Rt; // LEFT FRONT
  float RF = Ly - Lx - Rt; // RIGHT FRONT
  float LB = Ly - Lx + Rt; // LEFT BACK
  float RB = Ly + Lx - Rt; // RIGHT BACK

  
  int LeftFront = LF*255; // LEFT FRONT
  int RightFront = RF*255; // RIGHT FRONT
  int LeftBack = LB*255; // LEFT BACK
  int RightBack = RB*255; // RIGHT BACK


  Serial.println(LeftFront);
  Serial.println(RightFront);
  Serial.println(LeftBack);
  Serial.println(RightBack);

  Serial.println(" ");

  
  // Envia sinais aos motores
  setMotor(motorA_EN, motorA_PWM, LeftFront);
  setMotor(motorB_EN, motorB_PWM, RightFront);
  setMotor(motorC_EN, motorC_PWM, LeftBack);
  setMotor(motorD_EN, motorD_PWM, RightBack);

}

// === Função de controle PWM com direção ===
void setMotor(int enPin, int pwmPin, int velocidade) {
  velocidade = constrain(velocidade, -255, 255);
  int veloAbs = abs(velocidade);
  if (velocidade > 0) {
analogWrite(enPin, 0);              // Sentido horário
      analogWrite(pwmPin, veloAbs);
  } else if (velocidade < 0) {
      analogWrite(enPin, veloAbs);
      analogWrite(pwmPin, 0);               // Sentido anti-horário
  } else {
      analogWrite(pwmPin, 0);                 // Parado
      analogWrite(enPin, 0); 
  }
}
