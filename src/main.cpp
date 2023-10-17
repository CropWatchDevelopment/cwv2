#include "./../lib/ModbusRTU/ModbusRTU.h"

// data array for modbus network sharing
uint16_t au16data[16];
uint8_t u8state;

/**
 *  Modbus object declaration
 *  u8id : node id = 0 for master, = 1..247 for slave
 *  port : serial port
 *  u8txenpin : 0 for RS-232 and USB-FTDI
 *               or any pin number > 1 for RS-485
 */
Modbus master(0, Serial1, 3); // this is master and RS-232 or USB-FTDI

/**
 * This is an structe which contains a query to an slave device
 */
modbus_t telegram;

unsigned long u32wait;

void setup()
{
  Serial1.begin(9600);
  Serial2.begin(9600);
  pinMode(2, INPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  digitalWrite(4, HIGH);
  master.start();
  master.setTimeOut(2000); // if there is no answer in 2000 ms, roll over
  u32wait = millis() + 1000;
  u8state = 0;
}

void loop()
{
  switch (u8state)
  {
  case 0:
    if (millis() > u32wait)
      u8state++; // wait state
    break;
  case 1:
    telegram.u8id = 1;        // slave address
    telegram.u8fct = 3;       // function code (this one is registers read)
    telegram.u16RegAdd = 0;   // start address in slave
    telegram.u16CoilsNo = 7;  // number of elements (coils or registers) to read
    telegram.au16reg = au16data; // pointer to a memory array in the Arduino

    master.query(telegram); // send query (only once)
    u8state++;
    break;
  case 2:
    master.poll(); // check incoming messages
    if (master.getState() == COM_IDLE)
    {
      u8state = 0;
      u32wait = millis() + 100;

      if (master.capture_Success)
      {
        for(int i = 0; i < 8; i++)
        {
          Serial2.print(au16data[i]);
          Serial2.print(" ");
        }
        Serial2.println(" - SUCCESS!!!!!");
        delay(5000);
      }
    }
    break;
  }
}

// #include <ModbusMaster.h>

// #define MAX485_DE      3
// #define MAX485_RE_NEG  2

// ModbusMaster node;

// void preTransmission()
// {
//   digitalWrite(MAX485_RE_NEG, 1);
//   digitalWrite(MAX485_DE, 1);
// }

// void postTransmission()
// {
//   digitalWrite(MAX485_RE_NEG, 0);
//   digitalWrite(MAX485_DE, 0);
// }

// void setup()
// {
//   pinMode(MAX485_RE_NEG, OUTPUT);
//   pinMode(MAX485_DE, OUTPUT);
//   pinMode(4, OUTPUT);
//   // Init in receive mode
//   digitalWrite(MAX485_RE_NEG, 0);
//   digitalWrite(MAX485_DE, 0);
//   digitalWrite(4, HIGH);

//   // Modbus communication runs at 115200 baud
//   Serial1.begin(9600);
//   Serial2.begin(9600);

//   // Modbus slave ID 1
//   node.begin(2, Serial1);
//   // Callbacks allow us to configure the RS485 transceiver correctly
//   node.preTransmission(preTransmission);
//   node.postTransmission(postTransmission);
// }

// bool state = true;

// void loop()
// {
//   static uint32_t i;
//   uint8_t j, result;
//   uint16_t data[6];
//   i++;
//   // slave: read (6) 16-bit registers starting at register 2 to RX buffer
//   result = node.readHoldingRegisters(0, 1);
//   // do something with data if read is successful
//   if (result == node.ku8MBSuccess)
//   {
//     for (j = 0; j < 1; j++)
//     {
//       data[j] = node.getResponseBuffer(j);
//       Serial2.print(data[j]);
//     }
//     Serial2.println();
//   }
//   if (result == node.ku8MBResponseTimedOut)
//   {
//     Serial2.println("Timeout");
//   }
//   i = 0;
// }

// // #include <Arduino.h>
// // #include "LoRaWAN.h"
// // #include "STM32L0.h"
// // #include "pins.h"
// // #include "ModbusRTU.h"
// // #define DEBUG 1 // 1 = debug mode; 0 = production mode

// // // put function declarations here:
// // void PowerDownForSleep();
// // void DoModbus();
// // // put variable declarations here:
// // Modbus modbusMaster(0, Serial1);
// // uint8_t buffer[] = {1, 1, 3, 1, 0, 0}; // MODEL,VERSION,SENSOR-TYPE,BATTERY-LEVEL,ERROR-CODE,DATA...
// // const char *appEui = "94f5c41f62ef6db7";
// // const char *appKey = "739b041cbe94f66ae65fb8d1e7c7f493";
// // char lora_deveui[18];
// // uint32_t sleep_time = 600000;

// // void setup()
// // {
// //   // Setup Pins
// //   Serial1.begin(4800);
// //   pinMode(RS485_POWER_ENABLE, OUTPUT);
// //   pinMode(RS485_nRE, OUTPUT);
// //   pinMode(RS485_DE, OUTPUT);
// //   pinMode(BATTERY_LEVEL_PIN, INPUT);

// //   digitalWrite(RS485_POWER_ENABLE, LOW);

// //   // Do initial Read and display 1 time only using Serial Port
// //   // DoModbus();
// //   // Kill Serial port and USB in preperation for real-life operation

// //   // Setup LoRaWAN settings
// //   LoRaWAN.begin(AS923);
// //   LoRaWAN.setADR(true);
// //   LoRaWAN.getDevEui(lora_deveui, sizeof(lora_deveui));

// //   LoRaWAN.joinOTAA(appEui, appKey, (const char *)lora_deveui);
// // }

// // void loop()
// // {
// //   // Wake from sleep

// //   // Read RS485 Settings

// //   // Read from RS485 Device
// //   modbusMaster.ReadModbusData(1, 3, 0, 8, buffer);

// //   // if (STM32L0.getVBUS())
// //   // {
// //   //   LoRaWAN.setBatteryLevel(LORAWAN_BATTERY_LEVEL_EXTERNAL);
// //   // }
// //   // else
// //   // {
// //   //   LoRaWAN.setBatteryLevel(LORAWAN_BATTERY_LEVEL_UNKNOWN);
// //   // }

// //   // // Attempt to send confirmed LoRaWAN UP-LINK

// //   // if (LoRaWAN.getMaxPayloadSize() < sizeof(buffer))
// //   // {

// //   // }
// //   //   LoRaWAN.sendPacket(buffer, 30, true);
// //   //   while (LoRaWAN.busy())
// //   //   {
// //   //   }
// //   //   if (LoRaWAN.confirmed())
// //   //   {
// //   //     if (DEBUG)
// //   //       Serial.println("ACK");
// //   //     // on Success Read Updates for device
// //   //     //////--If recieved updates, save them to settings
// //   //   }
// //   //   else
// //   //   {
// //   //     if (DEBUG)
// //   //       Serial.println("NACK");
// //   //     LoRaWAN.joinOTAA(appEui, appKey, (const char *)lora_deveui);
// //   //   }

// //   // go to sleep
// //   // PowerDownForSleep();
// //   // STM32L0.stop(sleep_time);
// // }

// // void PowerDownForSleep()
// // {
// //   Serial.end();
// //   USBDevice.detach();
// //   digitalWrite(RS485_POWER_ENABLE, LOW);
// //   pinMode(RS485_nRE, LOW);
// //   pinMode(RS485_DE, LOW);
// // }

// // void DoModbus()
// // {

// //   if (DEBUG)
// //   {
// //     Serial.begin(115200);
// //     Serial.print("POWER: ");
// //     Serial.println(buffer[BATTERY_LEVEL]);
// //     Serial.print("RS485: ");
// //     for (int i = 0; i < 8; i++)
// //     {
// //       Serial.print(buffer[i]);
// //     }
// //     Serial.println();
// //   }
// // }