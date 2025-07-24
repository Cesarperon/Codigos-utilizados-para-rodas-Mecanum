#include <Wire.h>
#include <BH1750.h>

// === Pinos dos motores ===
const int motorA_EN = 1;  // Frente esquerda
const int motorA_PWM = 2;

const int motorB_EN = 4;  // Frente direita
const int motorB_PWM = 3;

const int motorC_EN = 5;  // Trás esquerda
const int motorC_PWM = 6;

const int motorD_EN = 8;  // Trás direita
const int motorD_PWM = 7;

// === Pinos I2C para o sensor ===
const unsigned int SDA_15 = 20;
const unsigned int SCL_15 = 21;

// === Sensor BH1750 ===
BH1750 lightMeter;

// === Configurações ===
int velocidadeMinima = 70;    // PWM mínimo

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

}

void loop() {
  float lux = lightMeter.readLightLevel();

  // Converte lux para PWM proporcional, com mínimo
  int pwmBase = luxparaPWM(lux);

  // Calcula velocidades individuais com base em X, Y e R
  float LF = Ly + Lx + Rt;
  float RF = Ly - Lx - Rt;
  float LB = Ly - Lx + Rt;
  float RB = Ly + Lx - Rt;

  // Normaliza para manter dentro de -1 a 1
  float maxVal = max(max(abs(LF), abs(RF)), max(abs(LB), abs(RB)));
  if (maxVal > 1.0) {
    LF /= maxVal;
    RF /= maxVal;
    LB /= maxVal;
    RB /= maxVal;
  }

  // Aplica PWM proporcional à luz
  setMotor(motorA_EN, motorA_PWM, vA * pwmBase);
  setMotor(motorB_EN, motorB_PWM, vB * pwmBase);
  setMotor(motorC_EN, motorC_PWM, vC * pwmBase);
  setMotor(motorD_EN, motorD_PWM, vD * pwmBase);
}

int velocidadeporlux(float lux) {
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
  } else
  {
    analogWrite(enPin, 0);       // Parado
    analogWrite(pwmPin, 0);
  }
}
