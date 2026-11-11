#include <Adafruit_BNO055.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <utility/imumaths.h>

#define BNO055_SAMPLERATE_DELAY_MS (500)
Adafruit_BNO055 bno = Adafruit_BNO055(55, 0x28, &Wire);

void displayCalStatus(void) {
  /* Get the four calibration values (0..3) */
  /* Any sensor data reporting 0 should be ignored, */
  /* 3 means 'fully calibrated" */
  uint8_t system, gyro, accel, mag;
  system = gyro = accel = mag = 0;
  bno.getCalibration(&system, &gyro, &accel, &mag);

  /* The data should be ignored until the system calibration is > 0 */
  Serial.print("\t");
  if (!system) {
    Serial.print("! ");
  }

  /* Display the individual values */
  Serial.print("Sys:");
  Serial.print(system, DEC);
  Serial.print(" G:");
  Serial.print(gyro, DEC);
  Serial.print(" A:");
  Serial.print(accel, DEC);
  Serial.print(" M:");
  Serial.print(mag, DEC);
}


void setup() {
  Serial.begin(9600);

  while (!Serial) delay(10);  // wait for serial port to open!

  Serial.println("Orientation Sensor Test");
  Serial.println("");

  /* Initialise the sensor */
  if (!bno.begin()) {
    /* There was a problem detecting the BNO055 ... check your connections */
    Serial.print("Ooops, no BNO055 detected ... Check your wiring or I2C ADDR!");
    while (1)
      ;
  }

  delay(1000);

  bno.setExtCrystalUse(true);
}

void loop() {
  // Possible vector values can be:
  // - VECTOR_ACCELEROMETER - m/s^2
  // - VECTOR_MAGNETOMETER - uT
  // - VECTOR_GYROSCOPE - rad/s
  // - VECTOR_EULER - degrees
  // - VECTOR_LINEARACCEL - m/s^2
  // - VECTOR_GRAVITY - m/s^2

  // imu::Vector<3> euler = bno.getVector(Adafruit_BNO055::VECTOR_EULER);
  // /* Display the floating point data */
  // Serial.print("Euler X: ");
  // Serial.print(euler.x());
  // Serial.print(", ");
  // Serial.print("Euler Y: ");
  // Serial.print(euler.y());
  // Serial.print(", ");
  // Serial.print("Euler Z: ");
  // Serial.print(euler.z());
  // Serial.println("");

  imu::Vector<3> accel = bno.getVector(Adafruit_BNO055::VECTOR_ACCELEROMETER);
  /* Display the floating point data */
  Serial.print("Acc X: ");
  Serial.print(accel.x());
  Serial.print(", ");
  Serial.print("Acc Y: ");
  Serial.print(accel.y());
  Serial.print(", ");
  Serial.print("Acc Z: ");
  Serial.print(accel.z());
  Serial.println("");

  // imu::Vector<3> linAccel = bno.getVector(Adafruit_BNO055::VECTOR_LINEARACCEL);
  // /* Display the floating point data */
  // Serial.print("Lineal Acc X: ");
  // Serial.print(linAccel.x());
  // Serial.print(", ");
  // Serial.print("Lineal Acc: ");
  // Serial.print(linAccel.y());
  // Serial.print(", ");
  // Serial.print("Lineal Acc: ");
  // Serial.print(linAccel.z());
  // Serial.println("");

  displayCalStatus();

  /* New line for the next sample */
  Serial.println("");

  /* Wait the specified delay before requesting nex data */
  delay(BNO055_SAMPLERATE_DELAY_MS);
}
