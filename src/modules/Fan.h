#pragma once

#include "Module.h"
#include "cc1101.h"

namespace HAL {

class Fan : public Module {
public:
  explicit Fan(uint8_t id);

  const char *Name() override;
  bool Setup() override;
  bool Start() override;
  bool IsReady() const override;

  void Update() override;

private:
  enum class Query : uint32_t {
    FanToggle = 0x888888e8,
    FanDirection = 0x888e8888,
    Fan1 = 0x8888eee8,
    Fan2 = 0x8888e888,
    Fan3 = 0x8888e8e8,
    Fan4 = 0x888e88e8,
    Fan5 = 0x888ee888,
    Fan6 = 0x888e8ee8,
    LightToggle = 0x88888ee8,
    LightIntensity = 0x888eeee8,
  };

  BLETypedCharacteristic<Query> m_characteristic;
  CC1101::Radio m_cc1101;
};

} // namespace HAL
