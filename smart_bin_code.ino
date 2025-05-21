#include <ServoESP32.h>  // Use the ServoESP32 library for ESP32 boards

// Pins for Ultrasonic Sensor
#define TRIG_PIN 5
#define ECHO_PIN 18

// Servo Motor Pin
#define SERVO_PIN 19

// Button Pins for Waste Type
#define BIO_BUTTON 12
#define NONBIO_BUTTON 14
#define METAL_BUTTON 27

// LED Pins
#define GREEN_LED 21
#define YELLOW_LED 22
#define RED_LED 23

ServoESP32 lidServo;  // Initialize the ServoESP32 object

void setup() {
  Serial.begin(115200);

  // Set pin modes
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  pinMode(BIO_BUTTON, INPUT_PULLUP);
  pinMode(NONBIO_BUTTON, INPUT_PULLUP);
  pinMode(METAL_BUTTON, INPUT_PULLUP);

  pinMode(GREEN_LED, OUTPUT);
  pinMode(YELLOW_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);

  // Attach the servo motor
  lidServo.attach(SERVO_PIN);
  lidServo.write(0);  // Initial position: closed
}

void loop() {
  int distance = getDistance();
  indicateFillLevel(distance);

  // Simulate waste detection
  if (digitalRead(BIO_BUTTON) == LOW) {
    handleWaste("Biodegradable", distance);
  } else if (digitalRead(NONBIO_BUTTON) == LOW) {
    handleWaste("Non-Biodegradable", distance);
  } else if (digitalRead(METAL_BUTTON) == LOW) {
    handleWaste("Metal", distance);
  }

  delay(200);
}

// Function to get distance from HC-SR04
int getDistance() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  long duration = pulseIn(ECHO_PIN, HIGH);
  int distance = duration * 0.034 / 2;
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");
  return distance;
}

// Handles waste insertion based on type
void handleWaste(String type, int distance) {
  Serial.println("Detected: " + type);
  if (distance < 5) {
    Serial.println("Bin is full! Cannot open lid.");
    return;
  }

  openLid();
  delay(2000); // Simulate insertion time
  closeLid();
}

// Opens the bin lid
void openLid() {
  Serial.println("Opening lid...");
  lidServo.write(90);
}

// Closes the bin lid
void closeLid() {
  Serial.println("Closing lid...");
  lidServo.write(0);
}

// Indicate fill level with LEDs
void indicateFillLevel(int dist) {
  digitalWrite(GREEN_LED, LOW);
  digitalWrite(YELLOW_LED, LOW);
  digitalWrite(RED_LED, LOW);

  if (dist > 20) {
    digitalWrite(GREEN_LED, HIGH); // Mostly empty
  } else if (dist > 10 && dist <= 20) {
    digitalWrite(YELLOW_LED, HIGH); // Half-full
  } else {
    digitalWrite(RED_LED, HIGH); // Almost full
  }
}
