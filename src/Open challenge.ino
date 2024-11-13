#include <Pixy2.h>
#include <Servo.h>
#include "Ultrasonic.h"
#include <avr/wdt.h>
#include <VL53L0X.h>


#define SERVOPIN 9
#define ENCODERPIN 2

#define USTRIGHT 46
#define USERIGHT 47
#define USTLEFT 26
#define USELEFT 27

#define IN1 4
#define IN2 5

#define STARTBUTTON 18

#define NONE 0
#define BLUE 1
#define ORANGE 2
#define RED 3
#define GREEN 4
#define MAGENTA 5

#define CENTERVALUE 80
#define MAXLEFT 130
#define MAXRIGHT 35
#define MAXSPEED 250
#define TURNSPEED 200
#define SPEED2 140

#define DEBUG true
#define DEBUG2 false

#define YVALUE 65
#define XVALUERED 100
#define XVALUEGREEN 225
#define ENDLINE 20

#define GRADETURN 10

Pixy2 pixy;
Servo direccion;
VL53L0X sensor;

Ultrasonic USRight(USTRIGHT, USERIGHT);
Ultrasonic USLeft(USTLEFT, USELEFT);


bool state = false;
bool finaliza = false;
unsigned long readTime = millis();
unsigned long encodeContador = 0;
byte secondColor = 0;
byte count = 0;
int grade = 0;
int currentGrade = CENTERVALUE;

int firstPosition[4];
int lastPosition[4];
int currentPosition[4];
int tempPosition[4];

byte firstColorDetected = NONE;  // 0 = ninguno, 1 = azul, 2 = naranja
bool colorDetected = false;

void flash(byte time) {
  for (byte i = 0; i < time; i++) {
    pixy.setLamp(1, 0);
    delay(200);
    pixy.setLamp(0, 0);
    delay(200);
  }
}

void stop() {
  if (DEBUG) Serial3.println("Stop");
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
}

void forward(byte speed) {
  if (DEBUG2) Serial3.println("forward " + String(speed));
  analogWrite(IN1, speed);
  digitalWrite(IN2, LOW);
}

void backward(byte speed) {
  if (DEBUG2) Serial3.println("backward " + String(speed));
  digitalWrite(IN1, LOW);
  analogWrite(IN2, speed);
}



void setMove(int tick, byte direction, byte speed) {
  unsigned long conteoActual = encodeContador + tick;
  if (DEBUG2) Serial3.println("setMove " + String(tick));
  if (direction == 1) {
    while (encodeContador < conteoActual) {
      forward(speed);
      if (DEBUG2) Serial3.println("Actual: " + String(encodeContador) + " Final: " + String(conteoActual));
      wdt_reset();
    }
  } else if (direction == 2) {
    while (encodeContador < conteoActual) {
      backward(speed);
      if (DEBUG2) Serial3.println("Actual: " + String(encodeContador) + " Final: " + String(conteoActual));
      wdt_reset();
    }
  }
  // stop();
}

void moveCM(float distance, byte direction, byte speed) {
  unsigned int tick = int(distance * 1000.0 / 70.5);
  setMove(tick, direction, speed);
  wdt_reset();
}

void readDistance(unsigned int* arreglo) {
  arreglo[0] = sensor.readRangeContinuousMillimeters();
  // delay(10);
  arreglo[1] = USRight.read();
  arreglo[2] = USLeft.read();
}

void printDistance(unsigned int* arreglo) {
  char buffer[6];
  for (byte i = 0; i < 4; i++) {
    sprintf(buffer, "%04d", arreglo[i]);
    Serial3.print(buffer);
    Serial3.print("    ");
  }
  Serial3.println();
}

void goAhead() {
}
void moveRight(int xPos) {
}

void moveLeft(int xPos) {
}

void center() {
  if (DEBUG) Serial3.println("Center");
  direccion.write(CENTERVALUE);
}

void turnGrade(int grade) {
  if (DEBUG) Serial3.println("turnGrade " + String(CENTERVALUE + grade));
  currentGrade = CENTERVALUE + grade;
  direccion.write(currentGrade);
}

void nextStep(byte direction) {
  int dif = 0;
  wdt_reset();
  readDistance(currentPosition);
  printDistance(currentPosition);
  grade = 0;
  if(currentPosition[1] > 80 || currentPosition[2] > 80) return;
  if (tempPosition[1] >= tempPosition[2]) {
    if(direction == 0){
      dif = abs(tempPosition[1] - currentPosition[1]);
    }else {
      dif = abs(tempPosition[2] - currentPosition[2]);
    }
    switch (direction) {
      case 0:
      case 1:
        if (dif > 100) return;
        break;
      case 2:
        if (currentPosition[2] < 80) return;
        break;
      case 3:
        if (currentPosition[1] < 80) return;
        break;
    }
    grade = map(dif, 0, 100, 0, 100);
    Serial3.println("Diferencia 1: " + String(dif) + ". Grados: " + String(grade));

    if (tempPosition[2] > currentPosition[2]) {
      grade *= -1;
    } else if (tempPosition[2] < currentPosition[2]) {
      
    }
    Serial3.println("Diferencia 2: " + String(dif) + ". Grados: " + String(grade));
  } else {
    dif = abs(tempPosition[1] - currentPosition[1]);
        switch (direction) {
      case 1:
        if (dif > 100) return;
        break;
      case 2:
        if (currentPosition[2] < 80) return;
        break;
      case 3:
        if (currentPosition[1] < 80) return;
        break;
    }
    grade = map(dif, 0, 100, 0, 80);
    grade *= -1;
    if (tempPosition[1] > currentPosition[1]) {
      grade *= -1;
    } else if (tempPosition[2] < currentPosition[2]) {

    }
    Serial3.println("Diferencia 3: " + String(dif) + ". Grados: " + String(grade));
  }
  turnGrade(grade);
  setMove(5, 1, MAXSPEED);
}

void turnLeft() {
  if (DEBUG) Serial3.println("Izquierda");
  stop();
  readDistance(currentPosition);
  readDistance(tempPosition);
  center();
  while (currentPosition[2] < 140) {
    nextStep(1);
  }
  stop();
  direccion.write(MAXLEFT);
  direccion.write(MAXLEFT);
  setMove(700, 1, MAXSPEED);
  center();
  stop();
  readDistance(tempPosition);
  moveCM(20, 1, MAXSPEED);
  while (currentPosition[2] > 80) {
    nextStep(2);
  }
  stop();
  count++;
  if (count >= 12) {
    finaliza = true;
  }
}


void turnRight() {
  if (DEBUG) Serial3.println("Derecha");
  stop();
  readDistance(currentPosition);
  center();
  while (currentPosition[1] < 140) {
    nextStep(0);
  }
  center();
  stop();
   moveCM(20, 2, TURNSPEED-60);
  // stop();
  direccion.write(MAXRIGHT);
  direccion.write(MAXRIGHT);
  readDistance(currentPosition);
  setMove(910, 1, TURNSPEED);
  center();
  stop();
  readDistance(tempPosition);
  moveCM(20, 1, MAXSPEED);
  while (currentPosition[1] > 80) {
    nextStep(3);
  }
  stop();
  count++;
  if (count >= 12) {
    finaliza = true;
  }
}

void printValues(byte index) {
  switch (pixy.ccc.blocks[index].m_signature) {
    case 1:
      Serial3.print("Azul ");
      break;
    case 2:
      Serial3.print("Naranja ");
      break;
    case 3:
      Serial3.print("Rojo ");
      break;
    case 4:
      Serial3.print("Verde ");
      break;
    case 5:
      Serial3.print("Magenta ");
      break;
    default:
      Serial3.print("Ninguno ");
  }
  Serial3.print("  X: " + String(pixy.ccc.blocks[index].m_x));
  Serial3.println("  Y: " + String(pixy.ccc.blocks[index].m_y));
}

void IRS() {
  state = !state;
  while (!digitalRead(STARTBUTTON)) {}
}

void encoder() {
  encodeContador++;
}

void setup() {
  pinMode(STARTBUTTON, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(STARTBUTTON), IRS, FALLING);
  attachInterrupt(digitalPinToInterrupt(ENCODERPIN), encoder, RISING);
  Serial.begin(115200);
  Serial3.begin(9600);
  pixy.init();
  direccion.attach(9);
  direccion.write(90);
  Wire.begin();
  sensor.setTimeout(500);
  if (!sensor.init()) {
    while (1) {
      flash(6);
      delay(1000);
    }
  }
  sensor.startContinuous();
  stop();

  center();
  // wdt_enable(WDTO_2S);
  pixy.setLamp(1, 0);
  flash(3);
  state = false;
  while (!state) {
    readDistance(firstPosition);
    readDistance(tempPosition);
    readDistance(currentPosition);
    wdt_reset();
  }
  Serial3.println("Iniciado...");
  pixy.setLamp(1, 0);
  readDistance(tempPosition);
  doceVueltas();
  // stop();
  moveCM(60, 1, MAXSPEED);
  stop();
}


void doceVueltas() {
  while (!finaliza) {
    int closestIndex = -1;
    int maxY = -1;
    wdt_reset();
    pixy.ccc.getBlocks();
    readDistance(currentPosition);
    if (currentPosition[0] < 150) {
      stop();
      return;
    }
    if (pixy.ccc.getBlocks() > 0) {
      printValues(0);
      secondColor = 0;
      if (pixy.ccc.getBlocks() > 1) secondColor = pixy.ccc.blocks[1].m_signature;
      switch (pixy.ccc.blocks[0].m_signature) {
        case BLUE:
          stop();
          if (colorDetected) {
            if (firstColorDetected == ORANGE) {
              setMove(5, 1, MAXSPEED);
              Serial3.println("Color principal Naranja");
              break;
            }
          } else {
            firstColorDetected = BLUE;
            colorDetected = true;
            Serial3.println("Primer color detectado Azul");
          }
          readDistance(tempPosition);
          turnLeft();
          break;
        case ORANGE:
          stop();
          if (colorDetected) {
            if (firstColorDetected == BLUE) {
              setMove(5, 1, MAXSPEED);
              Serial3.println("Color principal Azul");
              break;
            }
          } else {
            firstColorDetected = ORANGE;
            colorDetected = true;
            Serial3.println("Primer color detectado Naranja");
          }
          readDistance(tempPosition);
          turnRight();
          break;
        default:
          setMove(5, 1, MAXSPEED);
      }
    } else {
      nextStep(1);
    }
  }
}

void loop() {
}