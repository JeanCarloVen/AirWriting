#include <Adafruit_BNO055.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <utility/imumaths.h>

/* Set the delay between fresh samples */
#define BNO055_SAMPLERATE_DELAY_MS (100)

// Check I2C device address and correct line below (by default address is 0x29 or 0x28)
//                                   id, address
Adafruit_BNO055 bno = Adafruit_BNO055(55, 0x28, &Wire);

/**************************************************************************/
/*
    Display sensor calibration status
*/
/**************************************************************************/
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

void getData(void) {
  /* Get a new sensor event */
  sensors_event_t event;
  bno.getEvent(&event);

  /* Board layout:
         +----------+
         |         *| RST   PITCH  ROLL  HEADING
     ADR |*        *| SCL
     INT |*        *| SDA     ^            /->
     PS1 |*        *| GND     |            |
     PS0 |*        *| 3VO     Y    Z-->    \-X
         |         *| VIN
         +----------+
  */

  /* The WebSerial 3D Model Viewer expects data as heading, pitch, roll */
  // Serial.print(F("Orientation: "));
  // Serial.print(360 - (float)event.orientation.x);
  // Serial.print(F(", "));
  // Serial.print((float)event.orientation.y);
  // Serial.print(F(", "));
  // Serial.print((float)event.orientation.z);
  // Serial.println(F(""));

  /* The WebSerial 3D Model Viewer also expects data as roll, pitch, heading */
  // imu::Quaternion quat = bno.getQuat();

  // Serial.print(F("Quaternion: "));
  // Serial.print((float)quat.w(), 4);
  // Serial.print(F(", "));
  // Serial.print((float)quat.x(), 4);
  // Serial.print(F(", "));
  // Serial.print((float)quat.y(), 4);
  // Serial.print(F(", "));
  // Serial.print((float)quat.z(), 4);
  // Serial.println(F(""));

  /* Also send calibration data for each sensor. */
  uint8_t sys, gyro, accel, mag = 0;
  bno.getCalibration(&sys, &gyro, &accel, &mag);
  Serial.print(F("Calibration: "));
  Serial.print(sys, DEC);
  Serial.print(F(", "));
  Serial.print(gyro, DEC);
  Serial.print(F(", "));
  Serial.print(accel, DEC);
  Serial.print(F(", "));
  Serial.print(mag, DEC);
  Serial.println(F(""));

  delay(BNO055_SAMPLERATE_DELAY_MS);
}

void plane(void){
  sensors_event_t event;
  bno.getEvent(&event);

  // Realizar 3 verticales arriba y abajo
  Serial.print("\tZ: ");
  Serial.print(event.orientation.z, 4);

  // el movimiento empieza en 0

}

void canvas(void) {
  // Una vez calibrado se debe establecer el plano
  // Establecer el plano
  // 1) Probar haciendo circulos con tres parece suficiente
  // 2) Al hacer el movimiento del circulo se tomarán 3 puntos en intervalos de tiempo de 100 microsegundos
  // 3) Los puntos los tomaremos en coordenadas cartesianas x,y,z
  // 4) Todas las letras se escribiran en el plano virtual (canvas)
  // 5) 1. Concepto matemáticoPara encontrar la ecuación de este plano,
  //  necesitas:Dos vectores directores: Resta las coordenadas de un punto con los otros dos para formar los vectores \(\vec{AB} = B - A\) y \(\vec{AC} = C - A\).
  // Vector normal: Calcula el producto vectorial de ambos vectores \(\vec{n} = \vec{AB} \times \vec{AC}\), el cual será perpendicular al plano.
  // Ecuación del plano: Utiliza uno de los puntos (por ejemplo, A) y el vector normal para establecer que cualquier punto (x, y, z)
  // en el plano satisface la ecuación:\(\vec{n} \cdot ((x, y, z) - A) = 0\)

  // Cada vez que que el apuntador se encuentre sobre el plano debe de indicar que se encuentra sobre el plano
  // Se guarda en memoria el trazado

  // if(onCanvas){
  // Serial.Print(F("Esta sobre el plano"))
  //}else{
  //  Serial.Print(F("NO esta sobre el plano"))
  //}
  sensors_event_t event;
  bno.getEvent(&event);

  // Heading / Yaw: event.orientation.x (ranges from 0° to 360°)
  // Roll: event.orientation.y (ranges from -90° to +90° or -180° to +180° depending on implementation)
  // Pitch: event.orientation.z (ranges from -90° to +90° or -180° to +180°)

  /* Display the floating point data */
  Serial.print("X: ");
  Serial.print(event.orientation.x, 4);
  Serial.print("\tY: ");
  Serial.print(event.orientation.y, 4);
  Serial.print("\tZ: ");
  Serial.print(event.orientation.z, 4);




}


/**************************************************************************/
/*
    Arduino setup function (automatically called at startup)
*/
/**************************************************************************/
void setup(void) {
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

/**************************************************************************/
/*
    Arduino loop function, called once 'setup' is complete (your own code
    should go here)
*/
/**************************************************************************/
void loop(void) {
  /* Get a new sensor event */
  sensors_event_t event;
  bno.getEvent(&event);

  /* Display the floating point data */
  Serial.print("X: ");
  Serial.print(event.orientation.x, 4);
  Serial.print("\tY: ");
  Serial.print(event.orientation.y, 4);
  Serial.print("\tZ: ");
  Serial.print(event.orientation.z, 4);

  /* Optional: Display calibration status */
  displayCalStatus();

  /* New line for the next sample */
  Serial.println("");

  /* Wait the specified delay before requesting nex data */
  delay(BNO055_SAMPLERATE_DELAY_MS);
}
