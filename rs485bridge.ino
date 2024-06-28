/**
 * @file rs485bridge.ino
 * @author Cristiano Brudna (cristiano.brudna@gmail.com)
 * @date 2024-06-25
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include <SoftwareSerial.h>

#define BAUD_RATE                       4800

#define SERIAL_BAUD_RATE                BAUD_RATE

#define RS485_BAUD_RATE                 BAUD_RATE
#define RS485_DI_TX_PIN                 2
#define RS485_DE_PIN                    3
#define RS485_RE_PIN                    4
#define RS485_RO_RX_PIN                 5

SoftwareSerial rs485Serial(RS485_RO_RX_PIN, RS485_DI_TX_PIN);

void setRs485ReceiveMode()
{
    digitalWrite(RS485_DE_PIN, LOW);
    digitalWrite(RS485_RE_PIN, LOW);
}

void setRs485TransmitMode()
{
    digitalWrite(RS485_DE_PIN, HIGH);
    digitalWrite(RS485_RE_PIN, HIGH);
}

void setup()
{
    // Initialize Serial for USB communication
    Serial.begin(SERIAL_BAUD_RATE);
    while(!Serial)
    {
        delay(10);
    }

    // Initialize the control pins
    pinMode(RS485_DE_PIN, OUTPUT);
    pinMode(RS485_RE_PIN, OUTPUT);
    
    setRs485ReceiveMode();

    // Initialize SoftwareSerial for RS485 communication
    rs485Serial.begin(RS485_BAUD_RATE);
}

void loop()
{
    // Check if data is available on the RS485 bus
    if (rs485Serial.available())
    {
        // Read data from the RS485 bus and send it to the Serial (USB)
        while (rs485Serial.available())
        {
            char c = rs485Serial.read();
            Serial.write(c);
        }
    }

    // Check if data is available on the Serial (USB)
    if (Serial.available())
    {
        setRs485TransmitMode();

        // Read data from the Serial (USB) and send it to the RS485 bus
        while (Serial.available())
        {
            char c = Serial.read();
            rs485Serial.write(c);
        }

        setRs485ReceiveMode();
    }
}
