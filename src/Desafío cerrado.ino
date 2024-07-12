#include <Pixy2.h>
#include <Servo.h> 

Pixy2 pixy; // Crea un objeto Pixy2
Servo myservo; // Crea un objeto Servo

const int motorPin1 = 3; // Define el pin 3 como motorPin1
const int motorPin2 = 4; // Define el pin 4 como motorPin2

void setup() {
  Serial.begin(9600); // Inicia la comunicación serial a 9600 baudios
  pixy.init(); // Inicializa la cámara Pixy2
  myservo.attach(5); // Conecta el servomotor al pin 5
  myservo.write(78); // Coloca el servomotor en la posición inicial (recto)
  pixy.setLamp(1, 1); // Enciende la lámpara integrada de la Pixy2

  pinMode(motorPin1, OUTPUT); // Define el motorPin1 como salida
  pinMode(motorPin2, OUTPUT); // Define el motorPin2 como salida

  digitalWrite(motorPin1, HIGH); // Activa el motorPin1
  digitalWrite(motorPin2, LOW); // Desactiva el motorPin2
}

void loop() {
  uint16_t blocks = pixy.ccc.getBlocks(); // Obtiene el número de bloques detectados por la cámara

  if (blocks) { // Si se detectan bloques
    int index = -1; // Inicializa el índice del bloque de interés
    int maxY = -1; // Inicializa la coordenada Y máxima
    int targetPosition; // Inicializa la posición objetivo del servomotor
    
    for (int i = 0; i < blocks; i++) { // Recorre todos los bloques detectados
      if (pixy.ccc.blocks[i].m_signature == 1 || pixy.ccc.blocks[i].m_signature == 2) { // Si el bloque es rojo (1) o verde (2)
        if (pixy.ccc.blocks[i].m_y > maxY) { // Si la coordenada Y del bloque actual es mayor que maxY
          maxY = pixy.ccc.blocks[i].m_y; // Actualiza maxY
          index = i; // Guarda el índice del bloque con la mayor Y
        }
      }
    }

    if (index != -1) { // Si se encontró un bloque de interés
      int x = pixy.ccc.blocks[index].m_x; // Obtiene la coordenada X del bloque
      int y = pixy.ccc.blocks[index].m_y; // Obtiene la coordenada Y del bloque
      int signature = pixy.ccc.blocks[index].m_signature; // Obtiene la firma del bloque (1 o 2)

      targetPosition = map(x, 0, 319, 0, 180); // Mapea la coordenada X a un rango de 0 a 180 grados
      myservo.write(targetPosition); // Mueve el servomotor a la posición objetivo

      if (y >= 142 && y <= 180) { // Si el bloque está dentro de una cierta área de interés en la pantalla
        if (signature == 1) { // Si el bloque es verde (1)
          myservo.write(50); // Mueve el servomotor hacia la izquierda
          delay(800); // Espera 800 milisegundos
          myservo.write(115); // Mueve el servomotor hacia la derecha
          delay(900); // Espera 900 milisegundos
          myservo.write(60); // Ajusta el servomotor para volver a la posición recta
          delay(400); // Espera 400 milisegundos
          myservo.write(78); // Vuelve el servomotor a la posición inicial (recto)
        } else if (signature == 2) { // Si el bloque es rojo (2)
          myservo.write(110); // Mueve el servomotor hacia la derecha
          delay(1000); // Espera 1000 milisegundos
          myservo.write(60); // Ajusta el servomotor para volver a la posición recta
          delay(500); // Espera 500 milisegundos
          myservo.write(78); // Vuelve el servomotor a la posición inicial (recto)
        }
      }
    }
  }
}