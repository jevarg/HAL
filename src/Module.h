#pragma once

#include <ArduinoBLE.h>
#include <vector>

namespace HAL {

class Module {
public:
#pragma pack(push, 1)
  struct Message {
    uint8_t moduleId;
    uint32_t value;
  };
#pragma pack(pop)

  explicit Module(uint8_t id);
  virtual ~Module() = default;

  virtual const char *Name() = 0;
  virtual bool Setup();
  virtual bool Start() = 0;
  virtual bool IsReady() const = 0;
  virtual BLEService& Service();

  virtual void Update() = 0;

protected:
  std::unique_ptr<BLEService> m_service;
};

} // namespace HAL
