#include <PS2X_lib.h>

PS2X ps2x;

// Pinos dos motores
int IN1 = 2;   // Motor direito: frente
int IN2 = 3;   // Motor direito: ré
int velocidadeMotorDir = 4;  // PWM motor direito

int IN3 = 5;   // Motor esquerdo: frente
int IN4 = 7;   // Motor esquerdo: ré
int velocidadeMotorEsq = 8;  // PWM motor esquerdo

void setup() {
  ps2x.config_gamepad(13, 11, 10, 12, true, true);
  
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(velocidadeMotorDir, OUTPUT);
  pinMode(velocidadeMotorEsq, OUTPUT);
}

void loop() {
  ps2x.read_gamepad();

  // Controle de velocidade (eixo Y esquerdo)
  int ly = ps2x.Analog(PSS_LY);
  int velocidade = 0;

  // Mapeamento da velocidade (0-255)
  if (ly < 127) velocidade = map(ly, 0, 127, 255, 0);    // Frente
  else if (ly > 127) velocidade = map(ly, 128, 255, 0, 255); // Ré
  else velocidade = 0;

  // Controle de direção (eixo X direito)
  int rx = ps2x.Analog(PSS_RX);
  int velocidadeDir = velocidade;
  int velocidadeEsq = velocidade;


  if (rx > 140) { // Virar DIREITA: reduz motor ESQUERDO
    velocidadeEsq *= 0.5;
  } 
  else if (rx < 110) { // Virar ESQUERDA: reduz motor DIREITO
    velocidadeDir *= 0.5;
  }

  // Controle dos motores (frente/ré/parar)
  if (abs(ly - 127) > 10) { // Margem menor para evitar oscilação
    // Direção dos motores (IN3 e IN4 invertidos)
    digitalWrite(IN1, (ly < 127) ? HIGH : LOW);   // Motor direito frente
    digitalWrite(IN2, (ly < 127) ? LOW : HIGH);   // Motor direito ré
    digitalWrite(IN3, (ly < 127) ? LOW : HIGH);   // Motor esquerdo frente
    digitalWrite(IN4, (ly < 127) ? HIGH : LOW);   // Motor esquerdo ré

    // Aplica velocidades ajustadas
    analogWrite(velocidadeMotorDir, velocidadeDir);
    analogWrite(velocidadeMotorEsq, velocidadeEsq);
  } 
  else { // Parar motores
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, LOW);
    analogWrite(velocidadeMotorDir, 0);
    analogWrite(velocidadeMotorEsq, 0);
  }

  delay(10);
}
