#include <Wire.h>
#include <BH1750.h>

// === Pinos do motor ===
const int motorA_EN = 1;  // Frente esquerda
const int motorA_PWM = 2;

const int motorB_EN = 4;  // Frente direita
const int motorB_PWM = 3;

const int motorC_EN = 5;  // Trás esquerda
const int motorC_PWM = 6;

const int motorD_EN = 8;  // Trás direita
const int motorD_PWM = 7;

int velocidadeMinima = 70;

// === Sensor BH1750 ===
const unsigned int SDA_15 = 20;
const unsigned int SCL_15 = 21;
BH1750 lightMeter;

// === Variáveis ===
int velocidade = 0;

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
  lightMeter.begin(); 
  
}

void loop() {
  velocidade = velocidadeporlux();

  setMotor(motorA_EN, motorA_PWM, velocidade);
  setMotor(motorB_EN, motorB_PWM, velocidade);
  setMotor(motorC_EN, motorC_PWM, velocidade);
  setMotor(motorD_EN, motorD_PWM, velocidade);

  delay(500);
}

// === Velocidade proporcional à luz ===
int velocidadeporlux() {
  float lux = lightMeter.readLightLevel();

  // Limita para o máximo de 255 lux
  lux = constrain(lux, 0, 255);

  // Converte diretamente para o intervalo PWM 0–255
  velocidade = constrain((lux / 255.0) * (255 - velocidadeMinima) + velocidadeMinima, 0, 255);

  return velocidade;
}

// === Função para controlar motores ===
void setMotor(int enPin, int pwmPin, int velocidade) {
  analogWrite(enPin, 0);         // Sentido horário (ajuste conforme necessidade)
  analogWrite(pwmPin, velocidade);
}
