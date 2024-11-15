/*
Opening challenge code of the Team Spark, future Engineers’ category.
    Using Pixycam2.1 to detect blue and orange lines, ultrasonic sensor HC-SR04 
  To detect the left and right distance of the robot, sensor VL53L0X to detect frontal direction distance, servomotor mg90s to direct the robot, motor DC and Bluetooth module to the practices.
  (this will not be allowed during the competition according to the rules).
*/

#include <Servo.h> // Library to control servomotors.
#include <Pixy2.h> // Library to manage the Pixy2 camera.
#include <Wire.h> // Library for the I2C communication.
#include <DFRobot_VL53L0X.h> // Library to the VL53L0X distance sensor.
#include <avr/wdt.h> // Library to manage the watchdog timer.
#include <Ultrasonic.h> // Library to manage the ultrasonic sensors.

#define STARTBUTTON 18 // Define the start button pin.

#define IN1 4 // Define IN1 to the motor control pin.
#define IN2 5 // Define IN2 to the motor control pin.

#define LEDREADY 53 // Define the indicated control LED pin.
#define SERVOPIN 9 // Define the servomotor pin.
#define ENCODERPIN 2 // Define the encoder pin.
#define USTRIGHT A13 // Define the signal pin to the right ultrasonic sensor.
#define USERIGHT A12 // Define the echo pin to the right ultrasonic sensor.
#define USTLEFT A14 // Define the signal pin to the left ultrasonic sensor.
#define USELEFT A15 // Define the echo pin to the left ultrasonic sensor.

#define TURNGRADE 30 // Define the turning grade to check the direction adjustment.
#define NEUTRO 0 // Define constant to the neutral state.
#define IZQUIERDA 1 // Define the constant in the left direction.
#define DERECHA 2 // Define the constant in the right direction. 
#define CENTERVALUE 90 // Center Value to the servomotor (Straight).
#define MAXLEFT 156 // Top-value towards the left for the servomotor.
#define MAXRIGHT 5 // Top-value towards the right for the servomotor.
#define MAXSPEED 250 // Top-speed of the motor.
#define TURNSPEED 180 // Speed for turnings.
#define SPEED2 140 // Secondary speed.

#define LMIN 10 // Low limit of the sensors reading. 
#define LMAX 10 // top limit of the sensors reading.
#define ENDLINE 20 // Limit the ending line.

#define NONE 0 // None color detected.
#define BLUE 1 // Blue color detected.
#define ORANGE 2 // Orange color detected.

#define DEBUG true // Define the depuration.
#define DEBUG2 false // Define the second depuration level.
#define SENSORCOUNT 1 // Define the sensor`s number VL53L0X.

Pixy2 pixy; // Create an instance of the Pixy2 camera.
Servo direccion; // Create an instance for the servomotor.
DFRobot_VL53L0X sensors[SENSORCOUNT]; // Create an array of sensors VL53L0X.
Ultrasonic USRight(USTRIGHT, USERIGHT); // Instance for the right ultrasonic sensor.
Ultrasonic USLeft(USTLEFT, USELEFT); // Instance for the left ultrasonic sensor.

int firstPosition[4]; // Array to upload the first position detected.
int lastPosition[4]; // Array to upload the last position detected.
int currentPosition[4]; // Array to upload the current position detected.
int tempPosition[4]; // Array to upload the detected position temporarily.

byte sentido = NEUTRO; // Variable to update the current position.
byte turnCount = 0; // Count the turnings done.

bool state = false; // State of the start button.
unsigned long encodeCounter = 0; // Count for the encoder pulls.
unsigned long readTime = 0; // Upload the time of the last reading of the sensors.
char myChar; // Variable to upload characters received.
const uint8_t xshutPins[SENSORCOUNT] = { 38 }; //Pin to enable or disenable the sensor VL53L0X.
byte firstColorDetected = NONE; // Variable to save the first color
detected 
bool colorDetected = false; // Indicate if a color is detected.

void stop() {
  if (DEBUG) Serial3.println("Stop"); // Print "Stop" in the serial monitor if the depuration mode is active.
  digitalWrite(IN1, LOW); // Turn off the motor in the IN1 direction. 
  digitalWrite(IN2, LOW); // Turn off the motor in the IN2 direction.
}

void forward(byte speed) {
  if (DEBUG2) Serial3.println("Forward " + String(speed)); // Print "Forward" and the speed if DEBUG2 is active.
  analogWrite(IN1, speed); // Assign the speed in IN1 to go forward.
  digitalWrite(IN2, LOW); 
}

void backward(byte speed) {
if (DEBUG) Serial3.println("Backward " + String(speed)); // Print "Backward" and the speed if DEBUG is active.
  digitalWrite(IN1, LOW); // turn off IN1 to go backward.

  analogWrite(IN2, speed);
}

void center() {
  if (DEBUG) Serial3.println("Center"); // Print "Center" if DEBUG is active.
  direction.write(CENTERVALUE); // Place the servomotor in the central value (90 degrees).
}

void turnLeft() {
  if (DEBUG) Serial3.println("Left"); // Print "Left" if DEBUG is active.
  direction.write(MAXLEFT); // Turn the servomotor at the top to the left.
  direction.write(MAXLEFT); // Repeat the writing with precision.
  setMove(500, 1, TURNSPEED); // Perform the movement with 500 ticks in direction1 (forward) to TURNSPEED.
  center(); // Center the servomotor after the turning.
  stop(); // Stop the movement.
}

void turnRight() {
  if (DEBUG) Serial3.println("Right"); // Print "right" if DEBUG is active.
  direction.write(MAXRIGHT); // Turn the servomotor at the top to the right
  direction.write(MAXRIGHT); // Repeat the writing with precision.
  setMove(760, 1, MAXSPEED); // Perform the movement with 760 ticks in direction1 (forward) to MAXSPEED.
  center(); // Center the servomotor after the turning.
  stop(); // Stop the movement.
}

void turnGrade(int grade) {
  if (DEBUG) Serial3.println("turnGrade " + String(CENTERVALUE + grade)); // Print the turn angle calculated.
  direction.write(CENTERVALUE + grade); // Adjust the servomotor angle according to the specify grade.
}

void initSensor() {
  Wire.begin(); // Start the I2C communication.

  for (uint8_t i = 0; i < SENSORCOUNT; i++) {
    pinMode(xshutPins[i], OUTPUT); // Set the sensor`s off pin as an output. 
    digitalWrite(xshutPins[i], LOW); // Turn off the corresponding sensor. 
  }

  for (uint8_t i = 0; i < SENSORCOUNT; i++) {
    pinMode(xshutPins[i], INPUT); // Set the sensor`s off pin as an input to restart the sensor.
    delay(10); // Delay 10 ms to restart the sensor
    sensors[i].begin(0x2A + (i * 2)); // Start the sensor with the specific I2C direction. 
    sensors[i].setMode(sensors[i].eContinuous, sensors[i].eHigh); // Set the continuous mode and the high precision.
    sensors[i].start(); // Start the sensor measurement.
  }
}

void readDistance(unsigned int* array) {
  array[0] = sensors[0].getDistance(); // Upload the distance measured by the frontal sensor.
  array[2] = USRight.read(); // Upload the distance measured by the right ultrasonic sensor.
  array3] = USLeft.read(); // Upload the distance measured by the left ultrasonic sensor.
}

void printDistance(unsigned int* array) {
  char buffer[6]; 
  for (byte i = 0; i < 4; i++) {
    sprintf(buffer, "%04d", array[i]);
    Serial3.print(buffer); 
    Serial3.print("    "); 
  }
  Serial3.println(); 
}

void flash(byte time) { //Function for indicating that the start button can be pressed 
  for (byte i = 0; i < time; i++) {
    pixy.setLamp(1, 0); // turn on the Pixy2 camera LED. 
    delay(200); // Delay 200 ms
    pixy.setLamp(0, 0); // turn off the Pixy2 camera LED
    delay(200); // Delay 200 ms
  }
}

void setMove(int tick, byte direction, byte speed) {
  unsigned long conteoActual = encodeContador + tick; // Calculate the final value of the encoder for the movement.
  if (DEBUG) Serial3.println("setMove " + String(tick)); 
  if (direction == 1) { 
    while (encodeContador < conteoActual) { // While the final encoder value is not reached.
      forward (speed); // Go forward to the specified speed. 
      if (DEBUG2) Serial3.println("Actual: " + String(encodeContador) + " Final: " + String(conteoActual)); 
      wdt_reset(); 
    }
  } else if (direction == 2) { // If the direction is backward
    while (encodeContador < conteoActual) { // While the final encoder value is not reached.
      backware(speed); // Go backward to the specified speed.      
if (DEBUG2) Serial3.println("Actual: " + String(encodeContador) + " Final: " + String(conteoActual)); 
      wdt_reset(); 
    }
  }
}

void moveCM(float distance, byte direction, byte speed) {
  unsigned int tick = int(distance * 1000.0 / 69.0); // Calculate the ticks in centimeters.
  setMove(tick, direction, speed); 
  wdt_reset();
}

void followLine(byte speed) {
  int grade = 0; // Define the variable 'grade' which represents the  angle correction.
  if (DEBUG2) Serial3.println("followLine"); 
  readDistance(currentPosition); // Read the distances and upload 'currentPosition'
  if (currentPosition[0] < 10) { // If the frontal distance is less than 10 millimeters
    stop(); // Stop the robot
    return; 
  }
  if (DEBUG2) printDistance(currentPosition); 
  if (tempPosition[2] < tempPosition[3]) { // If the frontal distance is less to the left
    grade = map(currentPosition[2], tempPosition[2] - LMIN, tempPosition[2] + LMIN, TURNGRADE, -TURNGRADE); // Map the angle correction to the right.   
 if (DEBUG) Serial3.println("1 Actual: " + String(currentPosition[2]) + ". Inicial: " + String(tempPosition[2])); 
  } else { // If the left distance is less or equal to the right 
    grade = map(currentPosition[3], tempPosition[3] - LMIN, tempPosition[3] + LMIN, -TURNGRADE, TURNGRADE); // Map the angle correction to the left.    if (DEBUG) Serial3.println("2 Actual: " + String(currentPosition[3]) + ". Inicial: " + String(tempPosition[3])); 
  }
  if (DEBUG) Serial3.println("Grados : " + String(grade)); 
  if (grade > TURNGRADE) { // Limit the top angle turning.
    grade = TURNGRADE;
  } else if (grade < TURNGRADE * -1) { // Limit the low angle turning.
    grade = -TURNGRADE;
  }
  if (DEBUG) Serial3.println("Grades : " + String(grade)); 
  turnGrade(grade); // Turn the servomotor to the angle calculated.
  turnGrade(grade); // Repeat the action to assure precision.
  moveCM(2, 1, speed); // Move the robot 2 cms forward at the specified speed 
}

void correctLeft() { //Function to correct the left distance.
  turnGrade(60); 
  moveCM(20, 1, MAXSPEED); 
  turnGrade(-66); 
  moveCM(20, 1, MAXSPEED); 
  turnGrade(0); 
  moveCM(20, 1, MAXSPEED); 
  stop(); 
}

void correctRight() { // Function to correct the right distance.
  turnGrade(-60); 
  moveCM(20, 1, MAXSPEED); 
  turnGrade(66); 
  moveCM(20, 1, MAXSPEED); 
  turnGrade(0); 
  moveCM(20, 1, MAXSPEED); 
  stop(); 

}

void goCar() {
  bool endCycle = false; // Define a variable for the block detection.
  Serial3.println("goCar"); 
  readDistance(tempPosition); 
  while (!endCycle) { 
    pixy.ccc.getBlocks(false, CCC_SIG_ALL, 2); // Request the Pixy detection of objects with any signature.
    readDistance(currentPosition); 
     if (pixy.ccc.numBlocks > 0) { // If the Pixy detects a block.
      stop(); // Stop the robot
      center(); // Center the servomotor
      switch (pixy.ccc.blocks[0].m_signature) { 
        case BLUE: // If detect a blue object.
          Serial3.println("BLUE"); 
          if (!colorDetected) { // if any color has not been detected previously.
            colorDetected = true; // Mark color detected
            firstColorDetected = BLUE; // Stablish the color detected as blue.       
 }
          if (colorDetected && firstColorDetected == ORANGE) { // If orange has already been detected.
            Go to orange; 
          }
azul: 
          center(); // Center the servomotor
          readDistance(currentPosition); // upload 'currentPosition'
          readDistance(lastPosition); // upload 'lastPosition'
          while ((currentPosition[3] < 100)) { // While the left distance is less than 100 cm.
            followLine(SPEED2); 
            readDistance(currentPosition); // Upload 'currentPosition'
          }
          turnLeft(); // turn to the left
          moveCM(40, 1, MAXSPEED); // Move the robot 40 cm forward to the top speed. 
          do {
            followLine(MAXSPEED); // Follow the inner walls at the top-speed.
            readDistance(currentPosition); // Update 'currentPosition'
          } while (currentPosition[3] > 100); // Repeat while the left distance is more than 100 cm
          break;

        case ORANGE: // If detect an orange object.
          Serial3.println("ORANGE"); 
          if (!colorDetected) { // If any color has not been detected previously.
            colorDetected = true; // Mark color detected
            firstColorDetected = ORANGE; // Stablish the first color detected as orange        
   }
          if (colorDetected && firstColorDetected == BLUE) { // If blue has already been detected. 
go to blue; // Jump to the blue color section
          }
orange: 
          center(); // Center the servomotor
          readDistance(currentPosition); // Update 'currentPosition'
          readDistance(lastPosition); // Update 'lastPosition'
          while ((currentPosition[2] < 100)) { // while the right distance is more than 100 cm.
            followLine(SPEED2); // Follow the wall to 'SPEED2'.
            readDistance(currentPosition); // Update 'currentPosition'
          }
          turnRight(); // Turn to the right.
          moveCM(40, 1, MAXSPEED); // Move the robot 40 cm forward to the top speed.
          do {
            followLine(MAXSPEED); // Follow the inner Wall to top-speed. 
            readDistance(currentPosition); // Update 'currentPosition'
          } while (currentPosition[2] > 100); // Repeat while the right distance is more than 100 cm
          break;
      }

      stop(); // stop the robot at the end of the block detection.
      endCicle = true; // closing cycle.
      return; 
    } else { // If a block is not detected.
      Serial3.println("None detection block"); 
      printDistance(currentPosition); 
      followLine(MAXSPEED); // Follow the inner wall at the top speed.   
 }
  }
}

void IRS() {
  state = !state; // State changes between true and false.
  while (!digitalRead(STARTBUTTON)) {} // holdup till the button is pushed.
}

void encoder() {
  encodeContador++; // Increase the encoder counter.
}

void setup() {
  wdt_disable(); 
  pinMode(STARTBUTTON, INPUT_PULLUP); 
  pinMode(ENCODERPIN, INPUT_PULLUP); 
  pinMode(LEDREADY, OUTPUT); 
  digitalWrite(LEDREADY, LOW); 
  stop(); // Call the 'stop' function.
  direccion.attach(SERVOPIN); 
  center(); // Center the servomotor 
  Serial.begin(115200); 
  Serial3.begin(9600); 
  initSensor(); 
  attachInterrupt(digitalPinToInterrupt(ENCODERPIN), encoder, RISING); // Set an interruption for the encoder
  attachInterrupt(digitalPinToInterrupt(STARTBUTTON), IRS, FALLING); // Set an interruption for the start button.
  pixy.init(); 
  wdt_enable(WDTO_2S); 
  flash(3); // Call the 'flash' function to let wink the PixyCam LED three times.
  pixy.setLamp(0, 0); // turn off the PixyCam LED.
  state = false; // Stablish the robot`s initial state as false. 
  Serial3.println(millis()); 
  while (!state) { 
    readDistance(firstPosition); // Read the distance of the sensors and upload 'firstPosition'
    readDistance(tempPosition); // Read the distance of the sensors and upload 'tempPosition'
    readDistance(currentPosition); // Read the distance of the sensors and upload 'currentPosition'
    printDistance(currentPosition); 
    wdt_reset(); 
  }
  if (DEBUG) Serial3.println("Iniciated"); 
  digitalWrite(LEDREADY, LOW); // Assure the prepering LED it`s off. 
  center(); // Center the servomotor
  doceVueltas(); // Call the 'doceVueltas' function to make 12 laps (three laps done)
  moveCM(20, 1, MAXSPEED); // Call the 'moveCM' function to move the robot 20 cm forward at the top-speed.
  center(); // Center the servomotor again
  stop(); // Stop the robot
}

int count = 0; // Corner counter.

void doceVueltas() {
  for (count = 0; count < 12; count++) { 
    wdt_reset(); 
    goCar(); // Call the 'goCar' function to move the robot forward.
    followLine(MAXSPEED); // Call the 'followLine' function to follow the inner wall at the top-speed
    followLine(MAXSPEED); 
    followLine(MAXSPEED); 
    center(); // Center the servomotor (adjust the steering of the robot)
    if (DEBUG) Serial3.println("Lap " + String(count + 1) + "successfully done"); 
    if (DEBUG) Serial3.println();
    if (DEBUG) Serial3.println(); 
  }
  stop(); // Stop the robot after 12 complete laps.
}

void loop() {
}


    run = false;
    analogWrite(IN1, 0); // Stop the motor
    analogWrite(IN2, 0); // Stop the motor
  }
}
