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
int velocidadeMaxima = 255;   // PWM máximo

// === Direções de movimento ===
float X = 1.0;  // -1 (esquerda) a +1 (direita)
float Y = 0.0;  // -1 (trás) a +1 (frente)
float R = 0.0;  // -1 (girar esquerda) a +1 (girar direita)

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

  if (lightMeter.begin()) {
    Serial.println("BH1750 iniciado com sucesso!");
  } else {
    Serial.println("Erro ao iniciar BH1750!");
    while (1); // trava o código
  }
}

void loop() {
  float lux = lightMeter.readLightLevel();

  // Converte lux para PWM proporcional, com mínimo
  int pwmBase = luxToPWM(lux);

  // Calcula velocidades individuais com base em X, Y e R
  float vA = Y + X + R;
  float vB = Y - X - R;
  float vC = Y - X + R;
  float vD = Y + X - R;

  // Normaliza para manter dentro de -1 a 1
  float maxVal = max(max(abs(vA), abs(vB)), max(abs(vC), abs(vD)));
  if (maxVal > 1.0) {
    vA /= maxVal;
    vB /= maxVal;
    vC /= maxVal;
    vD /= maxVal;
  }

  // Aplica PWM proporcional à luz
  setMotor(motorA_EN, motorA_PWM, vA * pwmBase);
  setMotor(motorB_EN, motorB_PWM, vB * pwmBase);
  setMotor(motorC_EN, motorC_PWM, vC * pwmBase);
  setMotor(motorD_EN, motorD_PWM, vD * pwmBase);

  // Monitor serial
  Serial.print("Lux: ");
  Serial.print(lux);
  Serial.print(" | PWM base: ");
  Serial.println(pwmBase);

  delay(200);
}

int luxToPWM(float lux) {
  lux = constrain(lux, 0, 255);  // Trunca valor
  int pwm = (lux / 255.0) * (velocidadeMaxima - velocidadeMinima) + velocidadeMinima;
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
