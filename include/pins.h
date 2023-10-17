#pragma once

// RS485 MODBUS
#define RS485_POWER_ENABLE 4
#define RS485_RX           0
#define RS485_TX           1
#define RS485_nRE          2 // RECIEVER ENABLE (ACTIVE LOW)
#define RS485_DE           3 // DRIVER ENABLE (ACTIVE HIGH)

// POWER
#define BATTERY_LEVEL_PIN     A4
#define DEBUG_LED_POWER   D13

// Data Structure Indexes
//MODEL,VERSION,SENSOR-TYPE,BATTERY-LEVEL,ERROR-CODE,DATA...
#define MODEL         0
#define VERSION       1
#define SENSOR_TYPE   2
#define BATTERY_LEVEL 3
#define ERROR_CODE    4
#define DATA          5