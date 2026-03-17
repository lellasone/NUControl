#include <Arduino.h>
#include "i2c_encoder.hpp"

#include "transformations.hpp"

// I2C pin assignments (adjust for your board)
constexpr int SCL_PIN = 19;
constexpr int SDA_PIN = 18;

// AS5600 defaults: device address 0x36, angle high byte 0x0C, low byte 0x0D
constexpr uint8_t DEVICE_ADDR     = 0x36;
constexpr uint8_t ANGLE_HIGH_ADDR = 0x0E;
constexpr uint8_t ANGLE_LOW_ADDR  = 0x0F;

I2CEncoder encoder{SCL_PIN, SDA_PIN, DEVICE_ADDR, ANGLE_HIGH_ADDR, ANGLE_LOW_ADDR};

void setup()
{
  while (!Serial) {}
  Serial.println("I2C Encoder Test");
}

void loop()
{
  uint16_t raw   = encoder.read_raw();
  float    angle = encoder.read();

  Serial.print("raw: ");
  Serial.print(raw);
  Serial.print("\tangle (rad): ");
  Serial.println(angle, 6);

  delay(100);
}
