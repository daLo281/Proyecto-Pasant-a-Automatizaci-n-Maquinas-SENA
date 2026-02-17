para establecer la comunicación entre arduino y plc para obtener la señal de los sensores que componen la planta  hemos realizado la programación:

En la programación prodemos observar cómo leemos y enviamos los datos de cada uno de los sensores mediante un código de registro, adicionalmente le hemos dado al arduino una dirección IP y una dirección Fisica MAC para que pueda conectarse a modbus a través del módulo Ethernet ENC28J60 


REFERENCES

utilizamos la librería ModbusEthercard.h la cuál fue obtenida del siguiente Repositorio https://github.com/epsilonrt/modbus-ethercard.git
