#include "Servo.h" 
#include <Pixy2.h> 
#include <Ultrasonic.h> 

// Definición de pines y variables
const int IN1 = 3; // Pin de control del motor
const int IN2 = 4; // Pin de control del motor
const int servoPin = 5; // Pin de control para el servomotor
const int botonPin = 2; // Pin del botón para iniciar el programa
int velocidad = 250; // Variable de velocidad del robot (0-255)
bool ejecutar = false; // Estado del programa, falso para no ejecutar
Servo servoMotor; // Crea un objeto Servo para controlar el servomotor
Pixy2 pixy; // Crea un objeto Pixy para el sensor de visión
Ultrasonic ultrasonicIzquierdo(6, 7); // Sensor ultrasónico izquierdo (trigger, echo)
Ultrasonic ultrasonicDerecho(8, 9); // Sensor ultrasónico derecho (trigger, echo)
unsigned long tiempoInicio; // Almacena el tiempo de inicio del programa
const unsigned long tiempoDetencion = 50000; // Tiempo de detención del programa en milisegundos (50 segundos)
int primerColorDetectado = 0; // Almacena el primer color detectado por Pixy2

void setup() {
  // Configuración inicial de pines y dispositivos
  pinMode(IN1, OUTPUT); // Establece IN1 como salida
  pinMode(IN2, OUTPUT); // Establece IN2 como salida
  pinMode(botonPin, INPUT_PULLUP); // Establece el botón como entrada con resistencia de pull-up
  servoMotor.attach(servoPin); // Asocia el servomotor con su pin
  servoMotor.write(78); // Posición inicial del servomotor
  pixy.init(); // Inicializa el sensor Pixy
  pixy.setLamp(1, 0); // Enciende el LED del Pixy
}

void loop() {
  // Verifica si el botón está presionado y cambia el estado de 'ejecutar'
  if (digitalRead(botonPin) == LOW && !ejecutar) {
    ejecutar = true; // Activa la ejecución del programa
    tiempoInicio = millis(); // Establece el tiempo de inicio
    delay(500); // Retardo para evitar rebotes del botón
  }

  // Ejecuta el código si 'ejecutar' es verdadero y no ha pasado el tiempo de detención
  if (ejecutar && (millis() - tiempoInicio < tiempoDetencion)) {
    // Código principal que se ejecuta después de presionar el botón
    int distanciaIzquierda = ultrasonicIzquierdo.read(); // Lee la distancia del sensor izquierdo
    int distanciaDerecha = ultrasonicDerecho.read(); // Lee la distancia del sensor derecho
    
    // Si hay un objeto cerca del sensor izquierdo, mueve el servomotor
    if (distanciaIzquierda >= 3 && distanciaIzquierda <= 12) {
      servoMotor.write(104); // Gira a la derecha 
      delay(500); // Espera medio segundo
      servoMotor.write(60); // Gira a la izquierda para nivelar
      delay(200); // Espera 200 milisegundos
      servoMotor.write(78); // Regresa a la posición inicial
    } 
    // Si hay un objeto cerca del sensor derecho, mueve el servomotor
    else if (distanciaDerecha >= 3 && distanciaDerecha <= 12) {
      servoMotor.write(50); // Gira a la izquierda 
      delay(500); // Espera medio segundo
      servoMotor.write(120); // Gira a la derecha para nivelar
      delay(200); // Espera 200 milisegundos
      servoMotor.write(78); // Regresa a la posición inicial
    } 
    // Si no hay objetos cerca, verifica si Pixy detecta colores
    else {
      pixy.ccc.getBlocks(); // Solicita los bloques de colores detectados por Pixy
      
      // Si Pixy detecta al menos un bloque de color
      if (pixy.ccc.numBlocks > 0) {
        // Si aún no se ha detectado un color, almacena el primer color detectado
        if (primerColorDetectado == 0) {
          primerColorDetectado = pixy.ccc.blocks[0].m_signature; // Almacena la firma del color detectado
        }
        
        // Si el primer color detectado fue azul
        if (primerColorDetectado == 1) {
          // Si Pixy detecta azul, mueve el servomotor a la izquierda 
          if (pixy.ccc.blocks[0].m_signature == 1) {
            servoMotor.write(50);
            } 
          // Si Pixy detecta naranja, espera y regresa a la posición inicial
          else if (pixy.ccc.blocks[0].m_signature == 2) {
            delay(1000); // Espera un segundo
            servoMotor.write(78); 
          }
        } 
        // Si el primer color detectado fue naranja
        else if (primerColorDetectado == 2) {
          // Si Pixy detecta naranja, mueve el servomotor a la derecha
          if (pixy.ccc.blocks[0].m_signature == 2) {
            servoMotor.write(104); 
          } 
          // Si Pixy detecta azul, espera y regresa a la posición inicial
          else if (pixy.ccc.blocks[0].m_signature == 1) {
            delay(1000); // Espera un segundo
            servoMotor.write(78); 
          }
        }
      }
    }
    
    // Enciende el motor
    analogWrite(IN1, 0);
    analogWrite(IN2, velocidad);
  } 
  // Si se alcanza el tiempo de detención, detiene la ejecución
  else if (ejecutar && (millis() - tiempoInicio >= tiempoDetencion)) {
    ejecutar = false; // Desactiva la ejecución
    analogWrite(IN1, 0); // Detiene el motor
    analogWrite(IN2, 0); // Detiene el motor
  }
}