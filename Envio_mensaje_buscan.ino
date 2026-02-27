#include <SPI.h>
#include <mcp2515.h>
#include <OneWire.h>
#include <DallasTemperature.h>

// DEFINIMOS LOS PINES DE LOS SENSORES
#define TRIG_PIN 2
#define ECHO_PIN 7
#define FLOW_PIN 3
#define ONE_WIRE_BUS 6
#define CAN_CS 10

// LLAMAMOS LAS FUNCIONES DE LAS LIBRERÍAS
MCP2515 mcp2515(CAN_CS);
struct can_frame canMsg;

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

// ASIGNAMOS LAS VARIABLES
volatile unsigned int pulsos = 0;
unsigned long tiempoAnterior = 0;
unsigned long ts = 0;

float distancia_cm = 0;
float temperatura = 0;
float caudal_Lmin = 0;

// SENSOR DE CAUDAL
void contarPulsos() {
  pulsos++;
}

void setup() {
  Serial.begin(115200);

  // Ultrasonido
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  // Caudal
  pinMode(FLOW_PIN, INPUT);
  attachInterrupt(digitalPinToInterrupt(FLOW_PIN), contarPulsos, RISING);

  // Temperatura
  sensors.begin();

  // CAN
  mcp2515.reset();
  mcp2515.setBitrate(CAN_125KBPS);
  mcp2515.setNormalMode();

  // Inicializar CAN
  canMsg.can_id  = 0x0F6; // ID del nodo transmisor
  canMsg.can_dlc = 6;     // 6 bytes para los 3 sensores
  for(int i=0; i<8; i++) canMsg.data[i] = 0;
}

void loop() {
  // SENSOR ULTRASÓNICO
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  long duracion = pulseIn(ECHO_PIN, HIGH, 30000);
  distancia_cm = duracion * 0.034 / 2;

  //SENSOR DE TEMPERATIRA RESPUESTA
  sensors.requestTemperatures();
  delay(700);
  temperatura = sensors.getTempCByIndex(0);

  // CONTADOR DE PULSOS
  if (millis() - tiempoAnterior >= 1000) {
    caudal_Lmin = pulsos / 7.5;
    pulsos = 0;
    tiempoAnterior = millis();
  }

  // ENVÍO DEL CAN 
  if (millis() - ts > 2000) {
    ts = millis();

    // Convertir a int16 (x10 para un decimal)
    int16_t nivel_CAN   = distancia_cm * 10;
    int16_t temp_CAN    = temperatura * 10;
    int16_t caudal_CAN  = caudal_Lmin * 10;

    // Rellenar paquete CAN
    canMsg.data[0] = highByte(nivel_CAN);
    canMsg.data[1] = lowByte(nivel_CAN);
    canMsg.data[2] = highByte(temp_CAN);
    canMsg.data[3] = lowByte(temp_CAN);
    canMsg.data[4] = highByte(caudal_CAN);
    canMsg.data[5] = lowByte(caudal_CAN);

    // Enviar mensaje CAN
    if(mcp2515.sendMessage(&canMsg) == MCP2515::ERROR_OK) {
      Serial.print("Nivel: "); Serial.print(distancia_cm);
      Serial.print(" cm | Temp: "); Serial.print(temperatura);
      Serial.print(" C | Caudal: "); Serial.print(caudal_Lmin);
      Serial.println(" L/min enviado por CAN!");
    } else {
      Serial.println("Error al enviar mensaje CAN");
    }
  }
}
