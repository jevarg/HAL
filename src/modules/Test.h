#pragma once

#include <ArduinoBLE.h>
#include "../Module.h"

namespace HAL {

class Test : public Module
{
public:
  explicit Test(uint8_t id);

  const char* Name() override;
  bool Setup() override;
  bool Start() override;
  bool IsReady() const override;

  void Update() override;

private:
  BLEByteCharacteristic m_characteristic;
};

}
