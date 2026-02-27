La anterior programación está completamente enfocada a la lectura de ciertos sensores que utiliza la planta de tratamiento de aguas potables y residuales de FESTO, los cuáles están previamente adaptadas para ser leídas por el arduino por sus pines analógicos y posteriormente puedan ser enviados y leídos por el PLC a través de Ethernet.

para establecer la comunicación entre arduino y plc para obtener la señal de los sensores que componen la planta  hemos realizado la programación:

En la programación prodemos observar cómo leemos y enviamos los datos de cada uno de los sensores mediante un código de registro, adicionalmente le hemos dado al arduino una dirección IP y una dirección Fisica MAC para que pueda conectarse a modbus a través del módulo Ethernet ENC28J60 

se cargan códigos de programación correspondientes a la lectura de sensores para la planta de extracción de aceites con el nombre lectura de sensores.ino , la lectura de sensores de la planta de extracción de harina de achira con el nombre lectura de sensores PA. ino , el código para enviar y recibir datos con el módulo mcp2515 BUSCAN y todas las librerias correspondientes 

REFERENCES

utilizamos la librería ModbusEthercard.h la cuál fue obtenida del siguiente Repositorio https://github.com/epsilonrt/modbus-ethercard.git
