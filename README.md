# Introducción 

# Lista de componentes electrónicos
| Piezas  | Cantidad |
| --- | --- |
| Arduino mega | 1 |
| Cámara Pixy2 | 1 |
| Sensor ultrasonido (HC-SR04) | 3 |
| Mini protoboard de 170 puntos | 1 |
| Resistencia de 10k | 1 |
| Puente H (L298) | 1 |
| Botón pull-up | 1 |
| Cable dupont macho-macho | 6 |
| Cable dupont macho-hembra | 14 |
| Motor GA37-520 de 320rpm | 1 |
| Servo motor MG995 | 1 |
| Bateria 9V | 1 |
| Contrapeso de 194g | 2 |
| Sensor de color (TCS3200) | 1 |

# Chasis
El chasis fue diseñado por impresión 3D de manera que permitiese el movimiento de las ruedas del sistema Ackerman y el soporte de todas las partes electrónicas y mecánicas. Además se dividió en dos niveles.

## Superior
En este se colocó el controlador (Arduino Mega 2560), junto como el puente h (L298n), los sensores ultrasonidos (HC-S04) y la cámara pixy2.

## Inferior
Todas las partes mecánicas y de alimentación se colocaron en este nivel, eso incluye el sistema Ackerman y de empuje con su respectivo actuador y ademas de la bateria y el sensor de color.

# Movimiento
El robot cuenta con  dos partes de mecanismos independientes, el tren delantero y el tren trasero:

## Tren delantero
Se decidió que el robot tuviese tracción delantera, además de un mecanismo Ackerman para permitir el cruce del robot, para su movimiento buscamos actuadores que fueran precisos como los motores paso a paso o servomotores. Sin embargo se optó por usar un servomotor MG995, que tiene un torque de 9kg.cm a 5V y velocidad adecuada de 0.2seg/60°, que es suficiente para controlar adecuadamente el cruce y además sobreponerse a la fuerza de fricción generada, tanto por las ruedas como por el peso del robot sobre ellas. Se eligió este sistema porque permite una gran maniobrabilidad, facilita el estacionamiento y es fácil de controlar.

## Tren trasero
El movimiento de las ruedas traseras se transmite del actuador (motor de conducción) a las ruedas por un engranaje de 36mm de diámetro a otro idéntico colocado en un eje hexagonal que permite una transmisión de movimiento efectiva sin necesidad de agregar componentes extras como sería con ejes circulares. El actuador utilizado tiene un torque de 3,5Kg.cm y una velocidad nominal de 250rpm a 12V, sin embargo se decidió usar con una tensión eléctrica de 9V, lo que daría como resultado 187rpm, es decir una velocidad 1,37m/s.

# Componentes
## Batería
Al adquirir un kit de robótica obtuvimos una batería de 9V, con la que pudimos alimentar todos los componentes del robot, incluso en controlador ya que al ser un Arduino Mega permite un voltaje de alimentación de 7-12V, por lo que únicamente se ramificaron los cables de alimentación provenientes de la batería hacia el Arduino Mega y el puente

## Controlador (Arduino Mega 2560)
Se decidió usar único controlador,  el Arduino Mega ya que cuenta con 54 pines de los cuales 16 son analógicos y 12 de señal pwm y es compatible con I2C, lo que permite la conexión de gran cantidad de componentes de ser necesario al igual que una capacidad de procesamiento para codigos grandes y complejos.

## Sensor de color (TCS3200)
![Sensor de color (TCS3200)](https://github.com/KarenWon9/WRO-FI-Team-Spark/blob/main/other/apoyo/Tabla%20TCS3200.jpg)
![Motor GA37-520](https://github.com/KarenWon9/WRO-FI-Team-Spark/blob/main/other/apoyo/TCS3200.jpg)

Decidimos usar este sensor ya que cuenta con una fuente de luz, además proporciona información sobre el color dividiéndolo en tres componentes principales: rojo, verde y azul, el que después convierte en frecuencia con alta precision gracias a que tiene una matriz de 8x8 fotodiodos. Además su voltaje de alomentación va de 2,7 V a 5,5 V, por lo que se puede conectar directamente al Arduino Mega.

## Motor GA37-520

![Motor GA37-520](https://github.com/KarenWon9/WRO-FI-Team-Spark/blob/main/other/apoyo/Medidas%20de%20motor%20GA37-520.jpg)

| | | | |
| --- | --- | --- | --- |
| **Tensión nominal** | 12V | **Potencia nominal** | 8,3W |
| **Corriente de partida** | 1,5A | **Reducción** | 18.8:1 |
| **Velocidad sin carga** | 320rpm | **Velocidad nominal** | 224rpm |
| **Corriente sin carga** | ≥120mA | **Corriente nominal** | ≥400mA |
| **Par de bloqueo** | 3.1Kg.cm | **Par nominal** | 0.67Kg.cm |

Con el kit adquirido obtuvimos varias piezas que pudimos aprovechar como los motores, sus bases y las ruedas. Cada uno de los motores a 12V eran capaces de ir a  una velocidad nominal de 250rpm y un torque 3,5Kg a 12V. Por lo tanto usando un solo motor a 9V sería suficiente para darle la velocidad, control y seguimiento necesario, gracias a su encode que nos permite tener una mayor exactitud en el recorrido que el robot debe hacer en cancha.

### Código
Primeramente definimos las variables que van al puente h, que a su vez se conecta al motor que llamamos `IN1` e `IN2`, igualmente definimos 250 como la potencia del motor en una escala de 0 a 255 en la variable `velocidad`.

```ino
const int IN1 = 3; // Pin de control del motor
const int IN2 = 4; // Pin de control del motor
int velocidad = 250; // Variable de velocidad del robot (0-255)
```
La velocidad se mantendrá constante hasta que el programa lo detenga.}

```ino
void setup() {
    // Enciende el motor
    analogWrite(IN1, 0);
    analogWrite(IN2, velocidad);

     // Si se alcanza el tiempo de detención, detiene la ejecución
  else if (ejecutar && (millis() - tiempoInicio >= tiempoDetencion)) {
    ejecutar = false; // Desactiva la ejecución
    analogWrite(IN1, 0); // Detiene el motor
    analogWrite(IN2, 0); // Detiene el motor
  }
}
```
## Servo motor (MG995)
![Servo motor (MG995)](https://github.com/KarenWon9/WRO-FI-Team-Spark/blob/main/other/apoyo/Medidas%20servo%20motor%20MG995.png)
Hemos seleccionado al servomotor MG995 debido a su potente torque en un tamaño muy compacto, siendo esté más efectivo que su predecesor funcionando lo que nos permite tener una dirección mucho más precisa y con un torque de 9kg.cm a 5V.

### Código
Para facilitar su uso, utilizamos la librería `"Servo.h"`
y creamos una variable llamada `servoPin` donde colocamos el pin que está ocupando, en este caso es el 5.

```ino
#include "Servo.h" 
const int servoPin = 5; // Pin de control para el servomotor
Servo servoMotor; // Crea un objeto Servo para controlar el servomotor
```
Después asociamos el valor del pin con su pin mediante `servoMotor.attach(servoPin)` y le decimos que se mueva a 78° con el comando `servoMotor.write()` , así siempre comenzará en la posición de inicio.

```ino
void setup() {
  servoMotor.attach(servoPin); // Asocia el servomotor con su pin
  servoMotor.write(78); // Posición inicial del servomotor
}
```

Durante todo el código se usa este servomotor para realizar los cruces que sean requeridos durante los desafíos.

## Pixy2
 Esta cámara puede detectar 7 colores al mismo tiempo, además cuenta con un procesador y permite exportar únicamente la información que se necesita a través de distintos puertos serial UART, SPI, I2C, digital out o analógicos. Una velocidad de obturación de 60 fps por lo que el movimiento constante no será problema a la hora de detectar colores, cuenta con una apertura de 60º horizontal y 40º vertical. También cuenta con leds de luz blanca graduable para mejor percepción de los colores. 

### Código
 Con la cámara conectada a los pines I2C del Arduino Mega agregamos la librería `<Pixy2.h>`, también una variable llamada `primerColorDetectado` para almacenar el primer color que se detecte. Para inicializar la cámara pixy2 se utiliza `pixy.init()` y en caso de necesitar luz adicional se usa para el reconocimiento de los colores usamos `pixy.setLamp(1, 0)` y esto activará las luces led de la cámara.

```ino
#include <Pixy2.h> 
Pixy2 pixy; // Crea un objeto Pixy para el sensor de visión
int primerColorDetectado = 0; // Almacena el primer color detectado por Pixy2

void setup() {
     pixy.init(); // Inicializa el sensor Pixy
     pixy.setLamp(1, 0); // Enciende el LED del Pixy

```
 
 Con los colores ya enseñados a la Pixy2, creamos módulos para dos posibilidades donde primero solicitamos los colores que la cámara haya detectado con `pixy.ccc.blocks[0].m_signature` para almacenarlo en la variable `primerColorDetectado`

```ino
void loop() {
   // Si no hay objetos cerca, verifica si Pixy detecta colores
 else {
      pixy.ccc.getBlocks(); // Solicita los bloques de colores detectados por Pixy
      
      // Si Pixy detecta al menos un bloque de color
      if (pixy.ccc.numBlocks > 0) {
        // Si aún no se ha detectado un color, almacena el primer color detectado
        if (primerColorDetectado == 0) {
          primerColorDetectado = pixy.ccc.blocks[0].m_signature; // Almacena la firma del color detectado
        }
      }   
    }
  }
```
En caso de que el primer color detectado sea azul el cual asignamos con anterioridad como 1 y no se encuentren obstáculos cerca, el servomotor girará 50°. Es decir girará a la izquierda.

```ino
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
```
En caso de que el primer color detectado sea naranja asignado como 2 en la cámara , el servomotor girará 104°. Es decir girará a la derecha.
```ino
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

```

## Sensor ultrasonido(HC-S04)
![Sensor ultrasonido](https://github.com/KarenWon9/WRO-FI-Team-Spark/blob/main/other/apoyo/Medidas%20de%20sensor%20ultrasonido%20HC-SR04.jpg)

|  |  |  |  |
| --- | --- | --- | --- |
| **Voltaje de trabajo DC** | 5 V | **Distancia mínima** | 2cm |
| **Corriente de trabajo** | 15mA | **Ángulo de medición** | 15° |
| **Frecuencia de trabajo** | 40Hz | **Pulso  Trigger** | Input 10μs TTL |
| **Distancia máxima** | 4m | **Pulso Echo** | Output 100-25000 μs TTL |

Los sensores ultrasonido nos permite calcular distancias, funcionan con ondas de ultrasonido, donde está un emisor que transmite la onda y un receptor que detecta la onda enviada y así podemos calcular cuánta distancia hay según el tiempo que tardó el rebote de dicha onda. Gracias a estos sensores le permite al robot tener una idea de su ubicación en la cancha ya que tiene una capacidad de detección máxima de 4.5m. y la cancha tiene dimensiones de 3x3m, además también nos sirve para saber si está la posibilidad de chocar y así esquivar el obstáculo.

### Código
Para facilitar la conversión de la frecuencia de recepción de las ondas a cm utilizamos la librería `<Ultrasonic.h>` y posteriormente definimos los pines de cada sensor con `Ultrasonic` y el nombre que le demos al sensor seguido de `(Trig, Echo)`.

```ino
#include <Ultrasonic.h> 
Ultrasonic ultrasonicIzquierdo(6, 7); // Sensor ultrasónico izquierdo (trigger, echo)
Ultrasonic ultrasonicDerecho(8, 9); // Sensor ultrasónico derecho (trigger, echo)
```
Inmediatamente después de iniciar el programa leemos la distancia dada por los sensores y las guardamos en las variables `distanciaIzquierda` y `distanciaDerecha` respectivamente.

```ino
void loop() {
  if (ejecutar && (millis() - tiempoInicio < tiempoDetencion)) {
    // Código principal que se ejecuta después de presionar el botón
    int distanciaIzquierda = ultrasonicIzquierdo.read(); // Lee la distancia del sensor izquierdo
    int distanciaDerecha = ultrasonicDerecho.read(); // Lee la distancia del sensor derecho
  }
}  
```

En caso de que el sensor izquierdo detecte un obstáculo entre 3cm y 12cm el servomotor se moverá a 104°, después de medio segundo hará una compensación a 60° para volver a su posición original de 78°

```ino
 // Si hay un objeto cerca del sensor izquierdo, mueve el servomotor
    if (distanciaIzquierda >= 3 && distanciaIzquierda <= 12) {
      servoMotor.write(104); // Gira a la derecha 
      delay(500); // Espera medio segundo
      servoMotor.write(60); // Gira a la izquierda para nivelar
      delay(200); // Espera 200 milisegundos
      servoMotor.write(78); // Regresa a la posición inicial
```

En caso de que ocurra los mismo pero del lado del sensor derecho entonces, el servomotor se moverá a 50°, después de medio segundo hará una compensación a 120° para volver a su posición original de 78°

```ino
 // Si hay un objeto cerca del sensor derecho, mueve el servomotor
    else if (distanciaDerecha >= 3 && distanciaDerecha <= 12) {
      servoMotor.write(50); // Gira a la izquierda 
      delay(500); // Espera medio segundo
      servoMotor.write(120); // Gira a la derecha para nivelar
      delay(200); // Espera 200 milisegundos
      servoMotor.write(78); // Regresa a la posición inicial
    } 
```