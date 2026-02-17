/**
  @file TempSensor.ino
  Modbus-Arduino Example - TempSensor (Modbus TCP using Ethercard ENC28J60)
  Copyright by André Sarmento Barbosa
  http://github.com/andresarmento/modbus-arduino
*/

#include <ModbusEthercard.h> // SE ANEXA LA LIBRERAR MODBUSETHERCARD  PARA LA COMUNICACION MODBUS ENTRE EL PLC Y ARDUINO

//Modbus Registers Offsets (0-9999)
const int SENSOR_IREG1 = 10;//SE DEFINE SENSOR B102 EN REGISTRO 10
const int SENSOR_IREG2 = 20;//SE DEFINE SENSOR CLORO EN REGISTRO 20
const int SENSOR_IREG3 = 30;//SE DEFINE SENSOR B202 EN REGISTRO 30
const int SENSOR_IREG4 = 40;//SE DEFINE SENSOR B220 EN REGISTRO 40
const int SENSOR_IREG5 = 50;//SE DEFINE SENSOR B302 EN REGISTRO 50
const int SENSOR_IREG6 = 60;//SE DEFINE SENSOR B401 EN REGISTRO 60
const int SENSOR_IREG7 = 70;//SE DEFINE SENSOR B402 EN REGISTRO 70
const int SENSOR_IREG8 = 80;//SE DEFINE SENSOR OXIGENO EN REGISTRO 80

//Used Pins
//const int sensorPin = A3;

//VARIABLES DE SENSORES, SE INICIALIZAN TODOS LOS VALORES DE LAS LECTURAS DE SENSORES EN CERO
int Sensor_B102=0;
int Sensor_CLORO=0;
int Sensor_B202=0;
int Sensor_B220=0;
int Sensor_B302=0;
int Sensor_B401=0;
int Sensor_B402=0;
int Sensor_OXIGENO=0;

//ModbusEthercard object
ModbusEthercard mb;

long ts;

void setup() {
    
    // The media access control (ethernet hardware) address for the shield
    byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };// SE DEFINE UNA DIRECCION MAC PARA EL MODULO ENC28J60
    // The IP address for the shield
    byte ip[] = { 192, 168, 1, 4 };// SE DEFINE DIRECCION IP PARA EL MODULO ENC28J60
    // Config Modbus TCP
    mb.config(mac, ip);// SE CREA EL CANAL DE COMUNICACION MODBUS

    // Add SENSOR_IREG register - Use addIreg() for analog Inputs
    mb.addIreg(SENSOR_IREG1);// SE CREA EL REGISTRO 10 PARA ENVIAR INFORMACION POR CANAL MODBUS
    mb.addIreg(SENSOR_IREG2);// SE CREA EL REGISTRO 20 PARA ENVIAR INFORMACION POR CANAL MODBUS
    mb.addIreg(SENSOR_IREG3);// SE CREA EL REGISTRO 30 PARA ENVIAR INFORMACION POR CANAL MODBUS
    mb.addIreg(SENSOR_IREG4);// SE CREA EL REGISTRO 40 PARA ENVIAR INFORMACION POR CANAL MODBUS
    mb.addIreg(SENSOR_IREG5);// SE CREA EL REGISTRO 50 PARA ENVIAR INFORMACION POR CANAL MODBUS
    mb.addIreg(SENSOR_IREG6);// SE CREA EL REGISTRO 60 PARA ENVIAR INFORMACION POR CANAL MODBUS
    mb.addIreg(SENSOR_IREG7);// SE CREA EL REGISTRO 70 PARA ENVIAR INFORMACION POR CANAL MODBUS
    mb.addIreg(SENSOR_IREG8);// SE CREA EL REGISTRO 80 PARA ENVIAR INFORMACION POR CANAL MODBUS

    ts = millis();
}

void loop() {
  
  Sensor_B102=analogRead(A0);//SE LEE SENSOR B102
  Sensor_CLORO=analogRead(A2);//SE LEE SENSOR CLORO
  Sensor_B202=analogRead(A4);//SE LEE SENSOR B202
  Sensor_B220=analogRead(A6);//SE LEE SENSOR B220
  Sensor_B302=analogRead(A8);//SE LEE SENSOR B302
  Sensor_B401=analogRead(A10);//SE LEE SENSOR B401
  Sensor_B402=analogRead(A12);//SE LEE SENSOR B402
  Sensor_OXIGENO=analogRead(A14);//SE LEE SENSOR OXIGENO
  
   //Call once inside loop() - all magic here
   
   mb.task();

   //Read each two seconds
   if (millis() > ts + 2000) {
       ts = millis();
       //Setting raw value (0-1024)
       mb.Ireg(SENSOR_IREG1, Sensor_B102);// SE ENVIA LA INFORMACION DEL SENSOR B102 POR EL REGISTRO 10
       mb.Ireg(SENSOR_IREG2, Sensor_CLORO);// SE ENVIA LA INFORMACION DEL SENSOR CLORO POR EL REGISTRO 20
       mb.Ireg(SENSOR_IREG3, Sensor_B202);// SE ENVIA LA INFORMACION DEL SENSOR B202 POR EL REGISTRO 30
       mb.Ireg(SENSOR_IREG4, Sensor_B220);// SE ENVIA LA INFORMACION DEL SENSOR B220 POR EL REGISTRO 40
       mb.Ireg(SENSOR_IREG5, Sensor_B302);// SE ENVIA LA INFORMACION DEL SENSOR B302 POR EL REGISTRO 50
       mb.Ireg(SENSOR_IREG6, Sensor_B401);// SE ENVIA LA INFORMACION DEL SENSOR B401 POR EL REGISTRO 60
       mb.Ireg(SENSOR_IREG7, Sensor_B402);// SE ENVIA LA INFORMACION DEL SENSOR B402 POR EL REGISTRO 70
       mb.Ireg(SENSOR_IREG8, Sensor_OXIGENO);// SE ENVIA LA INFORMACION DEL SENSOR OXIGENO POR EL REGISTRO 80
   }
}
