// === Declaração de pinos ===

const int motorA_EN = 1;  // Frente esquerda
const int motorA_PWM = 2;

const int motorB_EN = 4;  // Frente direita
const int motorB_PWM = 3;

const int motorC_EN = 5;  // Trás esquerda
const int motorC_PWM = 6;

const int motorD_EN = 8;  // Trás direita
const int motorD_PWM = 7;

int N = 110; // valor minimo que o motor funciona

float Ly = 1;  // Frente/Trás
float Lx = 0;  // Esquerda/Direita
float Rt = 0;  // Rotação

// Valores para os 4 motores PWM
int LeftFront = 0; // LEFT FRONT
int RightFront = 0; // RIGHT FRONT
int LeftBack = 0; // LEFT BACK
int RightBack = 0; // RIGHT BACK

void setup() {
  pinMode(motorA_EN, OUTPUT);
  pinMode(motorA_PWM, OUTPUT);
  pinMode(motorB_EN, OUTPUT);
  pinMode(motorB_PWM, OUTPUT);
  pinMode(motorC_EN, OUTPUT);
  pinMode(motorC_PWM, OUTPUT);
  pinMode(motorD_EN, OUTPUT);
  pinMode(motorD_PWM, OUTPUT);

  Serial.begin(9600);
}

void loop() {

  // === Variáveis de movimento ===
  if (Serial.available() > 0) {
    Ly = Serial.parseFloat();
    Serial.print("Ly: ");
    Serial.println(Ly);

    Lx = Serial.parseFloat();
    Serial.print("Lx: ");
    Serial.println(Lx);

    Rt = Serial.parseFloat();
    Serial.print("Rt: ");
    Serial.println(Rt);
  }
  //Normaliza os parâmetros
  move_robot(Ly, Lx, Rt);

}

// === Função de controle PWM com direção ===

void setMotor(int enPin, int pwmPin, int velocidade) {
  
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

void move_robot(float Ly, float Lx, float Rt) {
  float LF = Ly + Lx + Rt;
  float RF = Ly - Lx - Rt;
  float LB = Ly - Lx + Rt;
  float RB = Ly + Lx - Rt;

  float ALF = abs(LF);
  float ARF = abs(RF);
  float ALB = abs(LB);
  float ARB = abs(RB);

  float Multi = 0;
  float maior = max(max(ALF, ARF), max(ALB, ARB));
  Multi = (maior <= 1) ? (255 - N) : (255 - N) / maior;

  LeftFront  = (LF > 0) ? (int)(LF * Multi + N) : (LF < 0) ? (int)(LF * Multi - N) : 0;
  RightFront = (RF > 0) ? (int)(RF * Multi + N) : (RF < 0) ? (int)(RF * Multi - N) : 0;
  LeftBack   = (LB > 0) ? (int)(LB * Multi + N) : (LB < 0) ? (int)(LB * Multi - N) : 0;
  RightBack  = (RB > 0) ? (int)(RB * Multi + N) : (RB < 0) ? (int)(RB * Multi - N) : 0;

    // Envia sinais aos motores
  setMotor(motorA_EN, motorA_PWM, LeftFront);
  setMotor(motorB_EN, motorB_PWM, RightFront);
  setMotor(motorC_EN, motorC_PWM, LeftBack);
  setMotor(motorD_EN, motorD_PWM, RightBack);

}

