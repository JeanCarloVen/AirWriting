#include <Adafruit_BNO055.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <utility/imumaths.h>

#define BNO055_SAMPLERATE_DELAY_MS (600)
Adafruit_BNO055 bno = Adafruit_BNO055(55, 0x28, &Wire);

int verticalCount = 0;
int horizontalCount = 0;

bool displayCalStatus(void) {

  bool response = false;
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
  Serial.println("");

  if (system == 3 && gyro == 3 && accel == 3 && mag == 3) {
    // Sensor calibrado correctamente
    response = true;
  }

  return response;
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

void loop(void) {

  // Calibración
  if (!displayCalStatus()) {
    delay(BNO055_SAMPLERATE_DELAY_MS);
    return;
  }

  //This function retrieves a 3-axis linear acceleration vector (m/s²) from the BNO055 sensor by subtracting the force of gravity
  imu::Vector<3> accel = bno.getVector(Adafruit_BNO055::VECTOR_LINEARACCEL);

  float az = accel.z();  // Aceleración en eje Z (m/s²)
  float ax = accel.x();
  float ay = accel.y();

  Serial.print("Accel X: ");
  Serial.println(accel.x());
  Serial.print("Accel y: ");
  Serial.println(accel.y());
  Serial.print("Accel z: ");
  Serial.println(accel.z());
  Serial.println("");


  if (az > 0.5) {
    Serial.println("Movimiento hacia arriba");
    Serial.println("");
    verticalCount++;
  } else if (az < -0.5) {
    Serial.println("Movimiento hacia abajo");
    Serial.println("");
    verticalCount++;
  } else {
    Serial.println("Sin movimiento vertical");
    Serial.println("");
  }

  Serial.println("verticalCount: ");
  Serial.println(verticalCount);

  if (verticalCount >= 8) {
    Serial.println("Se han hecho 8 lineas verticales");
    // Comenzar con la determinacion del plano
    // a) Hacer un circulo
    // b) Hacer un triangulo
    // c) Hacer una +
    // d) Hacer un infinito
    if (ax < -1) {
      Serial.println("Movimiento hacia la izquierda");
      horizontalCount++;
    } else if (ax > 1) {
      Serial.println("Movimiento hacia la derecha ");
      horizontalCount++;
    } else {
      Serial.println("Sin movimiento horizontal");
    }

    Serial.println("horizontalCount: ");
    Serial.println(horizontalCount);

    if(horizontalCount > 8){
      Serial.println("Se han hecho 8 lineas horizontales");
      //Debe guardar los puntos de estas 16 lineas que ayudaran a construir el plano
    }

  }

  /* Wait the specified delay before requesting nex data */
  delay(BNO055_SAMPLERATE_DELAY_MS);
}