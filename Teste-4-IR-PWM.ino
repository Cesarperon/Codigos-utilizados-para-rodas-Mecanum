// === Declaração de pinos ===

const int motorA_EN = 2;  // Frente esquerda
const int motorA_PWM = 1;

const int motorB_EN = 3;  // Frente direita
const int motorB_PWM = 4;

const int motorC_EN = 6;  // Trás esquerda
const int motorC_PWM = 5;

const int motorD_EN = 7;  // Trás direita
const int motorD_PWM = 8;

const int sensorPin = 17; // Pino do sensor infravermelho

int velocidadeAtual = 0;  // Armazena o último valor de velocidade

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

  // Sem linha detectada → acelera até 255 e mantém
  if (valorSensor == HIGH) {
    if (velocidadeAtual < 255) {
      for (int x = velocidadeAtual; x <= 255; x++) {
        move_robot(x);
        delay(10);
      }
      velocidadeAtual = 255;
    } else {
      // Já está na velocidade máxima
      move_robot(255);
    }
  } 
  // Linha detectada → freia imediatamente
  else {
    move_robot(0);
    velocidadeAtual = 0;
  }

  delay(100);
}

//------------------------------------------
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

//------------------------------------------
// Aplica mesma velocidade para todos os motores
void move_robot(int velocidade) {
  setMotor(motorA_EN, motorA_PWM, velocidade);
  setMotor(motorB_EN, motorB_PWM, velocidade);
  setMotor(motorC_EN, motorC_PWM, velocidade);
  setMotor(motorD_EN, motorD_PWM, velocidade);
}


