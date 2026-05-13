#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Arduino.h>

Adafruit_MPU6050 mpu;

int buzzer = 8;          // Buzzer connected to pin 8
int eyeSensor = 7;       // Eye blink sensor output pin connected to pin 7

// Movement thresholds
float upperThreshold = 1.5;  // Trigger threshold
float lowerThreshold = 0.3;  // Normal movement lower limit

void setup() {
  Serial.begin(115200);

  pinMode(buzzer, OUTPUT);
  digitalWrite(buzzer, LOW);

  pinMode(eyeSensor, INPUT);  // Set eye sensor pin as input

  // Initialize MPU6050
  if (!mpu.begin()) {
    Serial.println("MPU6050 not found! Check wiring.");
    while (1) delay(10);
  }

  Serial.println("MPU6050 connected successfully!");

  mpu.setAccelerometerRange(MPU6050_RANGE_2_G);
  mpu.setGyroRange(MPU6050_RANGE_250_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_5_HZ);
  delay(100);
}

void loop() {
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  float AccX = a.acceleration.x / 9.81;
  float AccY = a.acceleration.y / 9.81;
  float AccZ = a.acceleration.z / 9.81;

  // Read eye blink sensor value
  int eyeState = digitalRead(eyeSensor);

  // Print values
  Serial.print("AccX: "); Serial.print(AccX);
  Serial.print("  AccY: "); Serial.print(AccY);
  Serial.print("  AccZ: "); Serial.print(AccZ);
  Serial.print("  | Eye Sensor: "); Serial.println(eyeState);

  // Detect abnormal movement
  bool abnormalMovement = (abs(AccX) > upperThreshold || abs(AccY) > upperThreshold || abs(AccZ) < lowerThreshold);

  // Detect unwanted eye blink (for example, sensor gives LOW when eyes closed)
  bool eyeClosed = (eyeState == LOW); // Adjust depending on your sensor output

  // Combine both conditions
  if (abnormalMovement || eyeClosed) {
    Serial.println("⚠️ ALERT! Unusual Movement or Eye Blink Detected!");
    digitalWrite(buzzer, HIGH);
  } else {
    Serial.println("✅ Normal Driving Detected.");
    digitalWrite(buzzer, LOW);
  }

  delay(200); // Small delay for stability
} 
