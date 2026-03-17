#ifndef I2C_ENCODER_HPP
#define I2C_ENCODER_HPP

#include "encoder.hpp"
#include "transformations.hpp"

#include "Wire.h"

class I2CEncoder : public AbsoluteEncoder
{
public:
  I2CEncoder() = default;
  ~I2CEncoder() = default;
  I2CEncoder(
    const int SCL,
    const int SDA,
    const uint8_t device_address,
    const uint8_t addr_angle_high, 
    const uint8_t addr_angle_low,
    const int bit_order = MSBFIRST,
    const int res = 12
  )
  : SCL_(SCL),
    SDA_(SDA),
    addr_angle_low_(addr_angle_low),
    addr_angle_high_(addr_angle_high),
    addr_device_(device_address),
    max_read_(1 << res)
  {
    Wire.begin();
    Wire.setClock(400000); // 400 kHz I2C clock speed
  }

  /// @brief read the encoder
  /// @returns the angle is radians (0, 2PI)
  float read()
  {
    digitalWrite(14, HIGH);
    auto read = read_raw();
    if(glitch_filter_enable && (read == 0 || read == (uint16_t)(max_read_ - 1)))
    {
      return -1.f;
    }
    digitalWrite(14, LOW);
    Serial.println(read);
    return static_cast<float>(read) / static_cast<float>(max_read_ - 1) * _2_PI_;
  }

  void set_glitch_filter_state(bool state)
  {
    glitch_filter_enable = state;

  }

  uint16_t read_raw()
  {
    Wire.beginTransmission(addr_device_);
    Wire.write(addr_angle_high_);
    Wire.endTransmission(false);
    Wire.requestFrom(addr_device_, (uint8_t)2);
    uint16_t angle = ((uint16_t)Wire.read() << 8) | Wire.read();
    return angle;
  }

private:
  const int SCL_;
  const int SDA_;
  const uint8_t addr_angle_low_;
  const uint8_t addr_angle_high_;
  const uint8_t addr_device_;
  const int max_read_;
  bool glitch_filter_enable = true;
};
#endif
