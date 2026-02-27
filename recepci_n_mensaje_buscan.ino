#include <SPI.h>
#include <mcp2515.h>
#include <LiquidCrystal_I2C.h>

struct can_frame canMsg;
MCP2515 mcp2515(10);
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Variables de sensores
float nivel_f = 0;
float temp_f = 0;
float caudal_f = 0;

// Temporizador para alternar
unsigned long lastUpdate = 0;
int sensorIndex = 0; // 0 = nivel, 1 = temp, 2 = caudal

void setup() {
  lcd.init();
  lcd.backlight();
  lcd.clear();
  Serial.begin(115200);

  // Inicializar CAN
  mcp2515.reset();
  mcp2515.setBitrate(CAN_125KBPS);
  mcp2515.setNormalMode();

  // Mensaje inicial LCD
  lcd.setCursor(0, 0);
  lcd.print("Esperando CAN...");
}

void loop() {
  // ------------- Recepción CAN -------------
  if (mcp2515.readMessage(&canMsg) == MCP2515::ERROR_OK) {
    // Reconstruir valores de sensores
    int16_t nivel   = (canMsg.data[0] << 8) | canMsg.data[1];
    int16_t temp    = (canMsg.data[2] << 8) | canMsg.data[3];
    int16_t caudal  = (canMsg.data[4] << 8) | canMsg.data[5];

    // Convertir a float con 1 decimal
    nivel_f  = nivel / 10.0;
    temp_f   = temp / 10.0;
    caudal_f = caudal / 10.0;

    // Mostrar en Serial siempre
    Serial.print("Nivel: "); Serial.print(nivel_f); Serial.print(" cm | ");
    Serial.print("Temp: "); Serial.print(temp_f); Serial.print(" C | ");
    Serial.print("Caudal: "); Serial.print(caudal_f); Serial.println(" L/min");
  }

  // ------------- Alternar LCD cada 2s -------------
  if (millis() - lastUpdate > 2000) {
    lastUpdate = millis();
    lcd.clear();

    switch(sensorIndex) {
      case 0: // Nivel
        lcd.setCursor(0,0);
        lcd.print("Nivel:");
        lcd.setCursor(0,1);
        lcd.print(nivel_f,1);
        lcd.print(" cm");
        sensorIndex = 1;
        break;
      case 1: // Temperatura
        lcd.setCursor(0,0);
        lcd.print("Temperatura:");
        lcd.setCursor(0,1);
        lcd.print(temp_f,1);
        lcd.print(" C");
        sensorIndex = 2;
        break;
      case 2: // Caudal
        lcd.setCursor(0,0);
        lcd.print("Caudal:");
        lcd.setCursor(0,1);
        lcd.print(caudal_f,1);
        lcd.print(" L/min");
        sensorIndex = 0;
        break;
    }
  }
}
