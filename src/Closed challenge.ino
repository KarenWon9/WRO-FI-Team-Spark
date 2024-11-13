// Inclusion of libraries

#include <Pixy2.h>

#include <Servo.h>

#include "Ultrasonic.h"

#include <avr/wdt.h>

#include <VL53L0X.h>

 

// Pins and constants definition

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

 

// objects` creation

Pixy2 pixy;                 

Servo direction;          

VL53L0X sensor;           

Ultrasonic USRight(USTRIGHT, USERIGHT); 

Ultrasonic USLeft(USTLEFT, USELEFT);  

 

// Control Variables.

bool state = false;          

bool end = false;      

unsigned long readTime = millis(); 

unsigned long encodeContador = 0; 

byte secondColor = 0;        

byte count = 0;              

int grade = 0;              

int currentGrade = CENTERVALUE; 

 

// Secuency-position Variables.

int firstPosition[4];

int lastPosition[4];

int currentPosition[4];

int tempPosition[4];

 

// Variables to the detected color.

byte firstColorDetected = NONE;  // 0 = none, 1 = blue, 2 = orange

bool colorDetected = false;      

 

// Function to wink the pixy LED

void flash(byte time) {

  for (byte i = 0; i < time; i++) {

    pixy.setLamp(1, 0);  // Turn-on the LED

    delay(200);

    pixy.setLamp(0, 0);  // Turn-off the LED.

    delay(200);

  }

}

 

//Function to detect the robot.

void stop() {

  if (DEBUG) Serial3.println("Stop");

  digitalWrite(IN1, LOW);   

  digitalWrite(IN2, LOW);

}

 

// Function to move forward.

void forward(byte speed) {

  if (DEBUG2) Serial3.println("Forward " + String(speed));

  analogWrite(IN1, speed);  

  digitalWrite(IN2, LOW);

}

 

// Function to move backward.

void backward(byte speed) {

  if (DEBUG2) Serial3.println("Backward " + String(speed));

  digitalWrite(IN1, LOW);  

  analogWrite(IN2, speed);

}

 

// Function to move the robot with the codification and direction

void setMove(int tick, byte direction, byte speed) {

  unsigned long conteoActual = encodeContador + tick;

  if (DEBUG2) Serial3.println("setMove " + String(tick));

  

  if (direction == 1) {  // Move forward

    while (encodeCounter < currentCount) {

      forware(speed);

      if (DEBUG2) Serial3.println("Actual: " + String(encodeCounter) + " Final: " + String(currentCount));

      wdt_reset();  // Restart the watchdog timer

    }

  } else if (direction == 2) {  // Mover backward

    while (encodeContador < currentCount) {

      backwaredspeed);

      if (DEBUG2) Serial3.println("Actual: " + String(encodeContador) + " Final: " + String(conteoActual));

      wdt_reset();  // Restart the watchdog timer

    }

  }

}

 

// Function to move the robot to a specific distance in centimeters.

void moveCM(float distance, byte direction, byte speed) {

  unsigned int tick = int(distance * 1000.0 / 70.5); 

  setMove(tick, direction, speed);  // Call a setMove to move the robot

  wdt_reset();  // Restart the watchdog timer

}

 

// Function to read the distances of the ultrasonic sensors and the distance laser sensor.

void readDistance(unsigned int* array) {

  array[0] = sensor.readRangeContinuousMillimeters();  // Red the laser sensor distance.

  array[1] = USRight.read();  // Red the right ultrasonic sensor distance.

  array[2] = USLeft.read();   // Red the left ultrasonic sensor distance

}

 

// Function to print the distances read in the serial monitor.

void printDistance(unsigned int* array) {

  char buffer[6];

  for (byte i = 0; i < 4; i++) {

    sprintf(buffer, "%04d", array[i]);

    Serial3.print(buffer);

    Serial3.print("    ");

  }

  Serial3.println();

}

 

// Function to center the robot direction.

void center() {

  if (DEBUG) Serial3.println("Center");

  direccion.write(CENTERVALUE);  // Stablish the servo in the central position.

}

 

// Function to turn the robot in a specific grade.

void turnGrade(int grade) {

  if (DEBUG) Serial3.println("turnGrade " + String(CENTERVALUE + grade));

  currentGrade = CENTERVALUE + grade;  // Adjust the direction.

  direccion.write(currentGrade);  // Move the servo to the new direction.

}

 

// Function to let the robot move one step slower, adjusting the direction if it is necessary.

void nextStep(byte direction) {

  int dif = 0;

  wdt_reset();  // Restart the watchdog timer

  readDistance(currentPosition);  // read the current distances.

  printDistance(currentPosition);  // print the distances.

  grade = 0;

  if(currentPosition[1] > 80 || currentPosition[2] > 80) return;  // If there are near obstacles, don`t move.

  // Adjust the turning grades according to the different positions.

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

    grade = map(dif, 0, 100, 0, 100);  // Map the defference to a grade value.

 

    Serial3.println("Difference 1: " + String(dif) + ". Grades: " + String(grade));

 

    if (tempPosition[2] > currentPosition[2]) {

      grade *= -1;

    }

  }

  turnGrade(grade);  // Adjust the robot`s direction.

  setMove(5, 1, MAXSPEED);  // Move the robot forward.

}

 

// Function to turn the robot to the left.

void turnLeft() {

  if (DEBUG) Serial3.println("Left");

  stop();  // Stop the robot.

  readDistance(currentPosition);  // Read current distances.

  center();  // Center the direction.

  while (currentPosition[2] < 140) {

    nextStep(1);  // Move till it detects an appropriate distance.

  }

  stop();

  direction.write(MAXLEFT);  // Adjust the direction to the left.   setMove(700, 1, MAXSPEED);  // Move the robot forward

  center();  // Center again the position.

  stop();

}

 

// Function to turn the robot to the right.

void turnRight() {

  if (DEBUG) Serial3.println("Right");

  stop();  // Stop the robot.

  readDistance(currentPosition);  // read the current distances.

  center();  // Center the direction.

  while (currentPosition[1] < 140) {

    nextStep(3);  // Move till it detects an appropriate distance.

  }

  stop();

  direccion.write(MAXRIGHT);  // Adjust the direction to the right.

  setMove(700, 1, MAXSPEED);  // Move the robot forward

  center();  // Center againt the direction.

  stop();

}

 

// Principal Performance Function of the robot.

void doceVueltas() {

  while (!finaliza) {

    int closestIndex = -1;

    pixy.ccc.getBlocks();  // Obtain the blocks objects detected by the Pixy

    readDistance(currentPosition);  // Read the current distances.

 

    if (pixy.ccc.getBlocks() > 0) {  // If detect at least one object.

      // Detect the object`s color and apply the actions according to the color.

       switch (pixy.ccc.blocks[0].m_signature) {

        case BLUE:  // If detect a blue object

          stop();

          turnLeft();  // Turn to the Left

          break;

        case ORANGE:  // If detect an orange object

          stop();

          turnRight();  // Turn to the right.

          break;

        default:

          setMove(5, 1, MAXSPEED);  // Continue forward if any relevant color is detected.

      }

    } else {

      nextStep(1);  // Continue moving if any object is detected.

    }

  }

}
