# Movimiento
El robot cuenta con  dos partes de mecanismos independientes, el tren delantero y el tren trasero:

## Tren delantero
Se decidió que el robot tuviese tracción delantera, además de un mecanismo Ackerman para permitir el cruce del robot, para su movimiento buscamos actuadores que fueran precisos como los motores paso a paso o servomotores. Sin embargo se optó por usar un servomotor mg90s, que tiene un torque de 2.2kg.cm a 5V y velocidad adecuada de 0.1seg/60°, que es suficiente para controlar adecuadamente el cruce y además sobreponerse a la fuerza de fricción generada, tanto por las ruedas como por el peso del robot sobre ellas. Se eligió este sistema porque permite una gran maniobrabilidad, facilita el estacionamiento y es fácil de controlar.

## Tren trasero
El movimiento de las ruedas traseras se transmite del actuador (motor de conducción) a las ruedas por un engranaje de 3,6mm de diámetro a otro idéntico colocado en un eje hexagonal que permite una transmisión de movimiento efectiva sin necesidad de agregar componentes extras como sería con ejes circulares. El actuador utilizado tiene un torque de 3,5Kg.cm y una velocidad nominal de 250rpm a 12V, sin embargo se decidió usar con una tensión eléctrica de 9V, lo que daria como resultado 187rpm, es decir una velocidad 1,37m/s.
