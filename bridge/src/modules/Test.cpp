#include "Test.h"
#include "Logger.h"

#include <Arduino.h>

namespace HAL {

Test::Test(const uint8_t id)
    : Module(id), m_characteristic("0001", BLEWrite) {}

const char *Test::Name() { return "Test"; }

bool Test::Setup() {
  m_service->addCharacteristic(m_characteristic);
  return true;
}

bool Test::Start() { return true; }

bool Test::IsReady() const { return true; }

void Test::Update() {
  if (!m_characteristic.written()) {
    return;
  }

  Logger::Info("[Test] Received message: %s", String(m_characteristic.value(), HEX).c_str());

  digitalWrite(LED_BUILTIN, HIGH);
  delay(500);
  digitalWrite(LED_BUILTIN, LOW);
}

} // namespace HAL
