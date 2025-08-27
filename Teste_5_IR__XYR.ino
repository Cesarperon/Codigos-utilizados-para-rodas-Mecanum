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

int N = 110; // valor mínimo que o motor funciona

// Variáveis globais de movimento
float Ly = 0;
float Lx = 0;
float Rt = 0;

// PWM para os motores
int LeftFront = 0;
int RightFront = 0;
int LeftBack = 0;
int RightBack = 0;

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

  // Se não detectar linha (valor HIGH no KeyesIR), o robô anda
  if (valorSensor == HIGH) {
    Serial.println("Sem linha detectada!");
    Ly = 1;
    Lx = 0;
    Rt = 0;
  } else {
    Serial.println("Linha detectada!");
    Ly = 0;
    Lx = 0;
    Rt = 0;
  }

  // Calcula os valores PWM com base em Ly, Lx, Rt
  move_robot(Ly, Lx, Rt);

  // Envia sinais aos motores
  setMotor(motorA_EN, motorA_PWM, LeftFront);
  setMotor(motorB_EN, motorB_PWM, RightFront);
  setMotor(motorC_EN, motorC_PWM, LeftBack);
  setMotor(motorD_EN, motorD_PWM, RightBack);

  delay(100);
}

//------------------------------------------------
// Função para mover robô com rodas mecanum

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

}

//------------------------------------------------
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
