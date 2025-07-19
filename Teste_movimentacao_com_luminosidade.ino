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

const unsigned int SDA_15 = 20;
const unsigned int SCL_15 = 21;

// === Sensor BH1750 ===
BH1750 lightMeter;

// === Variáveis de movimento ===
int LeftFront = 0;
int RightFront = 0;
int LeftBack = 0;
int RightBack = 0;

// === Setup ===
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

// === Loop ===
void loop() {
  float lux = lightMeter.readLightLevel();
  Serial.print("Luminosidade: ");
  Serial.print(lux);
  Serial.println(" lux");

  int velocidade = calcularVelocidadePorLux(lux);

  Serial.print("Velocidade aplicada: ");
  Serial.println(velocidade);

// === Função para definir velocidade conforme lux ===
int calcularVelocidadePorLux(float lux) {
  if (lux > 150) {
    return 255;
  } else if (lux > 100) {
    return 200;
  } else if (lux > 50) {
    return 150;
  } else {
    return 100;
  }
}

  // Movimento simples para frente
  
  LeftFront = velocidade;
  RightFront = velocidade;
  LeftBack = velocidade;
  RightBack = velocidade;

  setMotor(motorA_EN, motorA_PWM, LeftFront);
  setMotor(motorB_EN, motorB_PWM, RightFront);
  setMotor(motorC_EN, motorC_PWM, LeftBack);
  setMotor(motorD_EN, motorD_PWM, RightBack);

  delay(500);
}

// === Função para controlar motores com direção ===
void setMotor(int enPin, int pwmPin, int velocidade) {
  int veloAbs = abs(velocidade);
  if (velocidade > 0) {
    analogWrite(enPin, 0);         // Sentido horário
    analogWrite(pwmPin, veloAbs);
  } else if (velocidade < 0) {
    analogWrite(enPin, veloAbs);   // Sentido anti-horário
    analogWrite(pwmPin, 0);
  } else {
    analogWrite(pwmPin, 0);        // Parado
    analogWrite(enPin, 0);
  }
}
