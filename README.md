# 1. Lista de componentes electrónicos
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

# 2. Componentes
## Pixy2
 Es una cámara que puede detectar 7 colores al mismo tiempo, cuenta con un procesador y permite exportar únicamente la información que se necesita a través de distintos puertos serial UART, SPI, I2C, digital out o analógicos. Además de una velocidad de obturación de 60 fps por lo que el movimiento constante no será problema a la hora de detectar colores, cuenta con una apertura de 60º horizontal y 40º vertical. También cuenta con leds de luz blanca graduable para mejor percepción de los colores. Para este caso utilizamos los pines I2C del Arduino Mega.
 
 Para poder usar la pixy se agrega la libreria `<Pixy2.h>`, ademas una variable llamada `primerColorDetectado` para almacenar el primer color que detectó. Para inicializar la cámara pixy2 se utiliza `pixy.init()` y en caso de necesitar luz adicional se usa `pixy.setLamp(1, 0)` y esto activará las luces led de la cámara.

```ino
#include <Pixy2.h> 
int primerColorDetectado = 0; // Almacena el primer color detectado por Pixy2

void setup() {
     pixy.init(); // Inicializa el sensor Pixy
     pixy.setLamp(1, 0); // Enciende el LED del Pixy

```
 
 Se crearon varios bloques para dos posibilidades donde primero solicitamos los colores que la camara haya detectado la cámara con `pixy.ccc.blocks[0].m_signature` para almacenarlo en la variable `primerColorDetectado`

```ino
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
```
En caso de que el primer color detectado sea azul el cual asignamos con anterioridad como 1, el servomotor girará 50°. Es decir girará a la izquierda.
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

## Controlador (Arduino Mega 2560)
Se decidió usar un Arduino Mega ya que cuenta con 54 pines de los cuales 16 son analógicos y 12 de señal pwm y es compatible con I2C, lo que permite la conexión de gran cantidad de componentes de ser necesario al igual que una capacidad de procesamiento para codigos grandes y complejos.

## Sensor ultrasonido(HC-S04)
![Sensor ultrasonido](https://github.com/KarenWon9/WRO-FI-Team-Spark/blob/main/other/Medidas%20de%20sensor%20ultrasonido%20HC-SR04.jpg)

|  |  |  |  |
| --- | --- | --- | --- |
| **Voltaje de trabajo DC** | 5 V | **Distancia mínima** | 2cm |
| **Corriente de trabajo** | 15mA | **Ángulo de medición** | 15° |
| **Frecuencia de trabajo** | 40Hz | **Pulso  Trigger** | Input 10μs TTL |
| **Distancia máxima** | 4m | **Pulso Echo** | Output 100-25000 μs TTL |

Los sensores para detectar distancia, funcionan con ondas de ultrasonido, donde está un emisor que transmite la onda y un detector que nos indica cuánta distancia hay según el tiempo que tardó el rebote de dicha onda. Le permite al robot tener una idea de su ubicación en la cancha ya que tiene una capacidad de detección máxima de 4.5m. y la cancha tiene dimensiones de 3x3m.

## MG995
Hemos seleccionado al servomotor MG995 debido a su potente torque en un tamaño semicompacto, siendo esté más efectivo que su predecesor funcionando lo que nos permite tener una dirección mmucho más precisa y con un torque de 9kg.cm a 5V.

## Motor GA37-520

![Sensor ultrasonido](https://github.com/KarenWon9/WRO-FI-Team-Spark/blob/main/other/Medidas%20de%20motor%20GA37-520.jpg)

| | | | |
| --- | --- | --- | --- |
| **Tensión nominal** | 12V | **Potencia nominal** | 8,3W |
| **Corriente de partida** | 1,5A | **Reducción** | 18.8:1 |
| **Velocidad sin carga** | 320rpm | **Velocidad nominal** | 224rpm |
| **Corriente sin carga** | ≥120mA | **Corriente nominal** | ≥400mA |
| **Par de bloqueo** | 3.1Kg.cm | **Par nominal** | 0.67Kg.cm |

Con el kit adquirido obtuvimos varias piezas que pudimos aprovechar como los motores, sus bases y las ruedas. Cada uno de los morores a 12V eran capaces de ir a  una velocidad nominal de 250rpm y un torque 3,5Kg a 12V. Por lo tanto usando un solo motor a 9V seria suficienta para darle la velocidad, control y seguimiento necesario, gracias a su encode que nos permite tener una mayor exactitud en el recorrido que el robot debe hacer en cancha.

## Batería
Al adquirir un kit de robótica obtuvimos una batería de 9V, con la que pudimos alimentar todos los componentes del robot, incluso en controlador ya que al ser un Arduino Mega permite un voltaje de alimentacíon de 7-12V, por lo que unicamente se ramificaron los cables de alimetacion provenientes de la bateria hacia el Arduino Mega y el puente H. 