#include "Servo.h"
#include <Pixy2.h>

const int IN1 = 11;
const int IN2 = 12;
const int servoPin = 10;
int velocidad = 250;
Servo servoMotor;
Pixy2 pixy;
unsigned long tiempoInicio;
const unsigned long tiempoDetencion = 50000;

void setup() {
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  servoMotor.attach(servoPin);
  pixy.init();
  pixy.setLamp(1,0);
  tiempoInicio = millis();
}

void loop() {
  if (millis() - tiempoInicio >= tiempoDetencion) {
    analogWrite(IN1, 0);
    analogWrite(IN2, 0);
    return;
  }
  analogWrite(IN1, 0);
  analogWrite(IN2, velocidad);
  pixy.ccc.getBlocks();
  if (pixy.ccc.blocks[0].m_signature == 1) {
    servoMotor.write(60);
  }
  if (pixy.ccc.blocks[0].m_signature == 2) {
    delay(800);
    servoMotor.write(90);
  }
}
