#pragma once

#include "Module.h"
#include "cc1101.h"

#include <unordered_map>

namespace HAL {

class Fan : public Module {
public:
  enum class Command : uint8_t {
    FanToggle = 0,
    FanDirection,
    FanSpeed1,
    FanSpeed2,
    FanSpeed3,
    FanSpeed4,
    FanSpeed5,
    FanSpeed6,
    LightToggle,
    LightIntensity,
  };

  explicit Fan(uint8_t id);

  const char *Name() override;
  bool Setup() override;
  bool Start() override;
  bool IsReady() const override;

  void Update() override;

private:
  BLETypedCharacteristic<Command> m_characteristic;
  CC1101::Radio m_cc1101;
};

} // namespace HAL
