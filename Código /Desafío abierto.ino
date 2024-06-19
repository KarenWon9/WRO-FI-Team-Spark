 #include "Servo.h"
#include <Pixy2.h>

const int IN1 = 11;
const int IN2 = 12;
const int servoPin = 10;
const int interruptorPin = 13;
int velocidad = 255;
Servo servoMotor;
Pixy2 pixy;
unsigned long tiempoInicio;
bool motorActivado = false;
const unsigned long tiempoDetencion = 47000;
int primerColorDetectado = 0;

void setup() {
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(interruptorPin, INPUT_PULLUP);
  servoMotor.attach(servoPin);
  pixy.init();
  pixy.setLamp(1, 0);
}

void loop() {
  if (digitalRead(interruptorPin) == LOW && !motorActivado) {
    motorActivado = true;
    tiempoInicio = millis();
  }

  if (motorActivado) {
    if (millis() - tiempoInicio < tiempoDetencion) {
      pixy.ccc.getBlocks();
      if (pixy.ccc.numBlocks > 0) {
        if (primerColorDetectado == 0) {
          primerColorDetectado = pixy.ccc.blocks[0].m_signature;
        }
        if (primerColorDetectado == 1) {
          if (pixy.ccc.blocks[0].m_signature == 1) {
            servoMotor.write(60); 
          } else if (pixy.ccc.blocks[0].m_signature == 2) {
            delay(1000);
            servoMotor.write(90); 
          }
        } else if (primerColorDetectado == 2) {
          if (pixy.ccc.blocks[0].m_signature == 2) {
            servoMotor.write(120);
          } else if (pixy.ccc.blocks[0].m_signature == 1) {
            delay(1000);
            servoMotor.write(90); 
          }
        }
      }
      analogWrite(IN1, velocidad);
      analogWrite(IN2, 0);
    } else {
      analogWrite(IN1, 0);
      analogWrite(IN2, 0);
      motorActivado = false;
    }
  } else {
    analogWrite(IN1, 0);
    analogWrite(IN2, 0);
  }
}