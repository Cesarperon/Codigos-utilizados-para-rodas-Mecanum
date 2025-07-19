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

float Ly = 0;  // Frente/Trás
float Lx = 0;  // Esquerda/Direita
float Rt = 0;  // Rotação
  
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

  
  float LF = Ly + Lx + Rt; // LEFT FRONT
  float RF = Ly - Lx - Rt; // RIGHT FRONT
  float LB = Ly - Lx + Rt; // LEFT BACK
  float RB = Ly + Lx - Rt; // RIGHT BACK



  // Velocidades absolutas
  float ALF = abs(LF);
  float ARF = abs(RF);
  float ALB = abs(LB);
  float ARB = abs(RB);

  // Speed upscaling to 255 (absolute max)
  // float Vel = N + X*(255 - N);
  
  float Multi = 0;  // Declare the Multi variable

  // Variaveis se houver snal > 0

  if (ALF >= ARF && ALF >= ALB && ALF >= ARB) {
    if (ALF <= 1)
      Multi = (255 - N);
    else 
      Multi = (255 - N) / ALF;
      
  }
  else if (ARF >= ALF && ARF >= ALB && ARF >= ARB) {
    if (ARF <= 1)
      Multi = (255 - N) ;
    else 
      Multi = (255 - N) / ARF;

  }
  else if (ALB >= ALF && ALB >= ARF && ALB >= ARB) {
    if (ALB <= 1)
      Multi = (255 - N) ;
    else 
      Multi = (255 - N) / ALB;

  }
  else if (ARB >= ALF && ARB >= ARF && ARB >= ALB) {
    if (ARB <= 1)
      Multi = (255 - N) ;
    else 
      Multi = (255 - N) / ARB;
 
  }

  int LeftFront = 0; // LEFT FRONT
  int RightFront = 0; // RIGHT FRONT
  int LeftBack = 0; // LEFT BACK
  int RightBack = 0; // RIGHT BACK

// LeftFront
    if (LF>0){
       LeftFront = (int)((LF * Multi) + N);  // LEFT FRONT
  } else if (LF<0){
       LeftFront = (int)((LF * Multi) - N); 
  }
    else{
       LeftFront = 0;
    }
    
// RightFront
    if (RF>0){
       RightFront = (int)((RF * Multi) + N);  // RIGHT FRONT
  } else if (RF<0){
       RightFront = (int)((RF * Multi) - N); 
  }
    else if (RF = 0){
       RightFront = 0;
  }

// LeftBack
    if (LB>0){
       LeftBack = (int)((LB * Multi) + N);  // LEFT BACK
  } else if (LB<0){
       LeftBack = (int)((LB * Multi) - N); 
  }
    else if (LB = 0){
       LeftBack = 0;
  }
  
// RightBack
  if (RB>0){
       RightBack = (int)((RB * Multi) + N);  // RIGHT BACK
  } else if (RB<0){
       RightBack = (int)((RB * Multi) - N); 
  }
    else if (RB = 0){
       RightBack = 0;
  }
  
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
