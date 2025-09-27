#include "Module.h"

namespace HAL {

Module::Module(const uint8_t id) {
  std::array<char, 5> uuid{};
  sprintf(uuid.data(), "%04u", id);

  m_service = std::make_unique<BLEService>(uuid.data());
}

bool Module::Setup() {
  return true;
}

BLEService& Module::Service() { return *m_service; }

} // namespace HAL