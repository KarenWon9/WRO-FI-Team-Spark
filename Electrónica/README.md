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

# Diagrama de cableado
![Diagrama de cableado](https://github.com/KarenWon9/WRO-FI-Team-Spark/blob/main/Electr%C3%B3nica/Diagrama%20de%20cableado.jpg)


# Elección de componentes
## Pixy2
 Es una cámara que puede detectar 7 colores al mismo tiempo, cuenta con un procesador y permite exportar únicamente la información que se necesita a través de distintos puertos serial UART, SPI, I2C, digital out o analógicos. Además de una velocidad de obturación de 60 fps por lo que el movimiento constante no será problema a la hora de detectar colores, cuenta con una apertura de 60º horizontal y 40º vertical. También cuenta con leds de luz blanca graduable para mejor percepción de los colores.

## Controlador (Arduino Mega 2560)
Se decidió usar un Arduino Mega ya que cuenta con 54 pines de los cuales 16 son analógicos y 12 de señal pwm y es compatible con I2C, lo que permite la conexión de gran cantidad de componentes de ser necesario al igual que una capacidad de procesamiento para codigos grandes y complejos.

## Sensor ultrasonido(HC-S04)
![Sensor ultrasonido](https://github.com/KarenWon9/WRO-FI-Team-Spark/blob/main/Electr%C3%B3nica/Medidas%20de%20sensor%20ultrasonido%20HC-SR04.jpg)

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

![Sensor ultrasonido](https://github.com/KarenWon9/WRO-FI-Team-Spark/blob/main/Electr%C3%B3nica/Medidas%20de%20motor%20GA37-520.jpg)

| | | | |
| --- | --- | --- | --- |
| **Tensión nominal** | 12V | **Potencia nominal** | 8,3W |
| **Corriente de partida** | 1,5A | **Reducción** | 18.8:1 |
| **Velocidad sin carga** | 320rpm | **Velocidad nominal** | 224rpm |
| **Corriente sin carga** | ≥120mA | **Corriente nominal** | ≥400mA |
| **Par de bloqueo** | 3.1Kg.cm | **Par nominal** | 0.67Kg.cm |

Con el kit adquirido obtuvimos varias piezas que pudimos aprovechar como los motores, sus bases y las ruedas. Cada uno de los morores a 12V eran capaces de ir a  una velocidad nominal de 250rpm y un torque 3,5Kg a 12V. Por lo tanto usando un solo motor a 9V seria suficienta para darle la velocidad, control y seguimiento necesario, gracias a su encode que nos permite tener una mayor exactitud en el recorrido que el robot debe hacer en cancha.

## Batería
Al adquirir un kit de robótica obtuvimos una batería de 9V, con la que pudimos alimentar todos los componentes del robot. 