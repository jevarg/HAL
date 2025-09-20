#include "Intercom.h"
#include "Logger.h"

#include <Arduino.h>

constexpr uint32_t WriteDelayMs = 200;
constexpr uint32_t DoorMessageValue = 0x01;
constexpr uint8_t DoorPin = D9;

namespace HAL {

Intercom::Intercom(const uint8_t id)
    : Module(id), m_characteristic("0001", BLEWrite) {}

const char *Intercom::Name() { return "Intercom"; }

bool Intercom::Setup() {
  pinMode(DoorPin, OUTPUT);
  digitalWrite(DoorPin, HIGH);

  m_service->addCharacteristic(m_characteristic);
  return true;
}

bool Intercom::Start() {
  m_ready = true;
  m_state = State::Idle;

  return true;
}

bool Intercom::IsReady() const { return m_ready; }

void Intercom::Update() {
  if (!m_characteristic.written()) {
    return;
  }

  _onMessage(m_characteristic.value());
}

void Intercom::_onMessage(const uint32_t value) {
  Logger::Debug(String("[Intercom] Received message: ") + String(value, HEX));

  if (value != DoorMessageValue) {
    Logger::Error("[Intercom] Unknown message");
    return;
  }

  if (m_state != State::Idle) {
    Logger::Error("[Intercom] Received message but state was not Idle");
    return;
  }

  Logger::Info("[Intercom] Opened door");
  m_state = State::Opening;

  digitalWrite(DoorPin, LOW);
  delay(WriteDelayMs);
  digitalWrite(DoorPin, HIGH);

  m_state = State::Idle;
}

} // namespace HAL
