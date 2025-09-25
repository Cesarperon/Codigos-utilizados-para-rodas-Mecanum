#include <Wire.h>
#include <BH1750.h>

const int motorA_EN = 1;  // Frente esquerda
const int motorA_PWM = 2;

const int motorB_EN = 4;  // Frente direita
const int motorB_PWM = 3;

const int motorC_EN = 5;  // Trás esquerda
const int motorC_PWM = 6;

const int motorD_EN = 8;  // Trás direita
const int motorD_PWM = 7;

const unsigned int SDA_15 = 20;
const unsigned int SCL_15 = 21;

BH1750 lightMeter;

int velocidadeMinima = 70;    

// === Direções de movimento ===
float Ly = 1;  // Frente/Trás
float Lx = 0;  // Esquerda/Direita
float Rt = 0;  // Rotação

void setup() {
  Serial.begin(9600);

  pinMode(motorA_EN, OUTPUT);
  pinMode(motorA_PWM, OUTPUT);
  pinMode(motorB_EN, OUTPUT);
  pinMode(motorB_PWM, OUTPUT);
  pinMode(motorC_EN, OUTPUT);
  pinMode(motorC_PWM, OUTPUT);
  pinMode(motorD_EN, OUTPUT);
  pinMode(motorD_PWM, OUTPUT);

  Wire.setSDA(SDA_15);
  Wire.setSCL(SCL_15);
  Wire.begin();

  // Inicializa o sensor
  if (!lightMeter.begin()) {
    Serial.println("Erro ao inicializar o BH1750!");
    while (1);
  }
}

void loop() {
  float lux = lightMeter.readLightLevel();
  
  int pwmBase = velocidadePorLux(lux);

  // Calcula velocidades individuais com base em X, Y e R
  float LF = Ly + Lx + Rt;  // Frente esquerda
  float RF = Ly - Lx - Rt;  // Frente direita
  float LB = Ly - Lx + Rt;  // Trás esquerda
  float RB = Ly + Lx - Rt;  // Trás direita

  // Normaliza para manter dentro de -1 a 1
  float maxVal = max(max(abs(LF), abs(RF)), max(abs(LB), abs(RB)));
  if (maxVal > 1.0) {
    LF /= maxVal;
    RF /= maxVal;
    LB /= maxVal;
    RB /= maxVal;
  }

  // Aplica PWM proporcional à luz
  setMotor(motorA_EN, motorA_PWM, LF * pwmBase);
  setMotor(motorB_EN, motorB_PWM, RF * pwmBase);
  setMotor(motorC_EN, motorC_PWM, LB * pwmBase);
  setMotor(motorD_EN, motorD_PWM, RB * pwmBase);

  delay(100);
}

int velocidadePorLux(float lux) {
  lux = constrain(lux, 0, 255); 
  int pwm = (lux / 255.0) * (255 - velocidadeMinima) + velocidadeMinima;
  return constrain(pwm, 0, 255);
}

void setMotor(int enPin, int pwmPin, float velocidade) {
  int pwm = abs(velocidade);
  if (velocidade > 0) {
    analogWrite(enPin, 0);       // Sentido horário
    analogWrite(pwmPin, pwm);
  } else if (velocidade < 0) {
    analogWrite(enPin, pwm);     // Sentido anti-horário
    analogWrite(pwmPin, 0);
  } else {
    analogWrite(enPin, 0);       // Parado
    analogWrite(pwmPin, 0);
  }
}
