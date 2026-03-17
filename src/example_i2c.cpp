#include <Arduino.h>
#include <TeensyTimerTool.h>
#include <vector>
#include <math.h>
#include "nu_control.hpp"
#include "i2c_encoder.hpp"

TeensyTimerTool::PeriodicTimer timer_(TeensyTimerTool::TCK);

constexpr float CURR_GAIN = 5.f; // Amps / Volt
constexpr int ADC_RES = 10;

InlineCurrentSensor Current_Phase_B{A8, CURR_GAIN, ADC_RES};
InlineCurrentSensor Current_Phase_C{A9, CURR_GAIN, ADC_RES};
InlineCurrentSensorPackage Current_Sensors{{&Current_Phase_C, &Current_Phase_B}};

constexpr float PWM_FREQ = 20000.f;
constexpr int PWM_RES = 10;
constexpr float DRIVER_VOLTAGE = 24.f;

BrushlessDriver GateDriver{{2, 3, 4}, 1, PWM_FREQ, PWM_RES, DRIVER_VOLTAGE};

// AS5600: device address 0x36, angle high byte 0x0E, low byte 0x0F
I2CEncoder Encoder{19, 18, 0x36, 0x0E, 0x0F};

BrushlessController controller_{wrist_motor, GateDriver, Current_Sensors, Encoder};



void setup()
{
  while (!Serial) {}

  TeensyTimerTool::attachErrFunc(timer_errors);
  analogReadAveraging(1);

  Serial.println("Hell yeah!");

  controller_.set_feedback_state(false);
  controller_.set_back_emf_comp_state(false);


  if (!controller_.init_components()) {
    Serial.println("Motor controller component failed to init");
    exit(0);
  }
  Serial.println("Aligning");

  if (!controller_.align_sensors()) {
    Serial.println("Motor controller component failed to align");
    exit(0);
  }

  Serial.println("Preparing to run");
  delay(1000);

  controller_.set_control_mode(ControllerMode::TORQUE);
  controller_.set_target(0.05f);

  controller_.start_control(500);

}

void loop() {}
