#include <SPI.h>
#include <EtherCard.h>        // Librería para ENC28J60
#include <ModbusEthercard.h>  // Librería Modbus TCP

// ---------------- PINES ----------------
// Sensores capacitivos
int sensores[] = {4, 5, 6, 7, 8};  
int leds[]     = {9, 10,11, 12, 13};  
const int NUM_SENSORES = 5;

// Sensores de pulsos
#define FLOW_PIN 2           // Sensor de caudal
#define MOTOR_SENSOR_PIN 3   // Sensor de herradura para velocidad del motor

// ---------------- VARIABLES ----------------
volatile unsigned int pulsos_caudal = 0;
volatile unsigned int pulsos_motor = 0;
unsigned long tiempoAnterior = 0;
unsigned long ts_serial = 0;

float caudal_Lmin = 0;
float velocidad_motor_RPM = 0;

// REGISTROS MODBUS
const int SENSOR_IREG1 = 10;  // Sensor capacitivo 1
const int SENSOR_IREG2 = 11;  // Sensor capacitivo 2
const int SENSOR_IREG3 = 12;  // Sensor capacitivo 3
const int SENSOR_IREG4 = 13;  // Sensor capacitivo 4
const int SENSOR_IREG5 = 14;  // Sensor capacitivo 5
const int SENSOR_IREG6 = 15;  // Sensor de caudal
const int SENSOR_IREG7 = 16;  // Sensor de velocidad del motor

// ---------------- MODBUS ----------------
ModbusEthercard mb;

//INICIO MODBUS
void contarPulsosCaudal() {
  pulsos_caudal++;
}

void contarPulsosMotor() {
  pulsos_motor++;
}

void setup() {
  // SENSORES CAPACITIVOS
  for (int i = 0; i < NUM_SENSORES; i++) {
    pinMode(sensores[i], INPUT_PULLUP);  // Pull-up interno
    pinMode(leds[i], OUTPUT);
  }

  //SENSORES 
  pinMode(FLOW_PIN, INPUT);
  attachInterrupt(digitalPinToInterrupt(FLOW_PIN), contarPulsosCaudal, RISING);

  pinMode(MOTOR_SENSOR_PIN, INPUT);
  attachInterrupt(digitalPinToInterrupt(MOTOR_SENSOR_PIN), contarPulsosMotor, RISING);

  tiempoAnterior = millis();
  Serial.begin(115200);

  //ID MODBUS
  byte mymac[] = {0xDE,0xAD,0xBE,0xEF,0xFE,0xED};
  byte myip[]  = {192,168,1,100};  // IP del Arduino

 


  // Agregar registros Modbus 
  mb.addIreg(SENSOR_IREG1);
  mb.addIreg(SENSOR_IREG2);
  mb.addIreg(SENSOR_IREG3);
  mb.addIreg(SENSOR_IREG4);
  mb.addIreg(SENSOR_IREG5);
  mb.addIreg(SENSOR_IREG6);
  mb.addIreg(SENSOR_IREG7);
}

void loop() {
  
  mb.task();

  // LECTURA SENSORES CAPACITIVOS
  for (int i = 0; i < NUM_SENSORES; i++) {
    int estado = digitalRead(sensores[i]);
    digitalWrite(leds[i], estado == LOW ? HIGH : LOW);  // LOW = tocado

    // Guardar valor en registro Modbus
    switch(i) {
      case 0: mb.Ireg(SENSOR_IREG1, estado == LOW ? 1 : 0); break;
      case 1: mb.Ireg(SENSOR_IREG2, estado == LOW ? 1 : 0); break;
      case 2: mb.Ireg(SENSOR_IREG3, estado == LOW ? 1 : 0); break;
      case 3: mb.Ireg(SENSOR_IREG4, estado == LOW ? 1 : 0); break;
      case 4: mb.Ireg(SENSOR_IREG5, estado == LOW ? 1 : 0); break;
    }
  }

  // SENSORES DE CAUDAL Y VELOCIDAD
  if (millis() - tiempoAnterior >= 1000) {
    // Calcular caudal
    caudal_Lmin = pulsos_caudal / 7.5;
    pulsos_caudal = 0;

    // Calcular velocidad motor (RPM)
    velocidad_motor_RPM = pulsos_motor * 60;
    pulsos_motor = 0;

    tiempoAnterior = millis();

    // Guardar en registros Modbus (multiplicando por 10 para decimales)
    mb.Ireg(SENSOR_IREG6, caudal_Lmin * 10);
    mb.Ireg(SENSOR_IREG7, velocidad_motor_RPM * 10);
  }

  //SERIAL
  if (millis() > ts_serial + 2000) {
    ts_serial = millis();

    Serial.print("Capacitivos: ");
    for (int i = 0; i < NUM_SENSORES; i++) {
      Serial.print(digitalRead(sensores[i]) == LOW ? "1" : "0");
      Serial.print(" ");
    }
    Serial.print("| Caudal: "); Serial.print(caudal_Lmin); Serial.print(" L/min");
    Serial.print(" | Vel Motor: "); Serial.print(velocidad_motor_RPM); Serial.println(" RPM");
  }
}
  
