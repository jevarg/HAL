#pragma once

#include <memory>
#include <ArduinoBLE.h>

#include "Module.h"

namespace HAL {

class Intercom : public Module
{
public:
  enum class State: uint8_t
  {
    Initializing = 0,
    Idle,
    Opening,

    Error = UINT8_MAX
};

  explicit Intercom(uint8_t id);

  const char* Name() override;
  bool Setup() override;
  bool Start() override;
  bool IsReady() const override;

  void Update() override;

private:
  State m_state = State::Initializing;
  bool m_ready = false;

  BLEByteCharacteristic m_characteristic;

  void _onMessage(uint32_t value);
};

}
