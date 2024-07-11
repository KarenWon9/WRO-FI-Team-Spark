#include <Pixy2.h>
#include <Servo.h>

const int IN1 = 11;
const int IN2 = 12;
int velocidad = 240;

const int Trigger = 4;
const int Echo = 3;
Pixy2 pixy;
Servo myServo;

void setup() {
  // Configuración del motor
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);

  Serial.begin(115200);
  pixy.init();
  pixy.setLamp(1, 0);
  myServo.attach(10);
  myServo.write(90);

  pinMode(Trigger, OUTPUT);
  pinMode(Echo, INPUT);
  digitalWrite(Trigger, LOW);
}

long getDistance() {
  long t;
  long d;

  digitalWrite(Trigger, HIGH);
  delayMicroseconds(10);
  digitalWrite(Trigger, LOW);

  t = pulseIn(Echo, HIGH);
  d = t / 59;

  return d;
}

void controlMotor() {
  analogWrite(IN1, 0);
  analogWrite(IN2, velocidad);
}

void loop() {
  long distance = getDistance();
  pixy.ccc.getBlocks();

  if (pixy.ccc.numBlocks) {
    int signature = pixy.ccc.blocks[0].m_signature;
    int x = pixy.ccc.blocks[0].m_x;
    int servoPos = map(x, 0, 319, 50, 130);

    if (distance >= 2 && distance <= 24) {
      if (signature == 1) {
        myServo.write(120); 
        Serial.println("color rojo, moviendo servo 120°");
        delay(1500);
      } else if (signature == 2) {
        myServo.write(60);
        delay(30);
        Serial.println("color verde, moviendo servo 60°");
        delay(1500);
      }
      
    } else {
      if (signature == 1 || signature == 2) {
        myServo.write(servoPos); 
        Serial.print("Following color, Signature: ");
        Serial.print(signature);
        Serial.print(", x: ");
        Serial.print(x);
        Serial.print(", Servo: ");
        Serial.println(servoPos);
      } else {
        myServo.write(90); 
      }
    }
  } else {
    myServo.write(90); 
    Serial.println("ningun color, moviendo servo 90°");
  }

  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  controlMotor();

  delay(0);
}
