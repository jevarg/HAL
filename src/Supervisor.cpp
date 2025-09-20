#include "Supervisor.h"
#include "Logger.h"

namespace HAL {

void Supervisor::Run() {
  _init();

  for (const auto &it : m_modules) {
    it->Start();
  }
}

void Supervisor::Update() {
  _updateConnection();
  _updateLed();
  if (m_state != State::Connected) {
    return;
  }

  while (m_central.connected()) {
    _updateModules();
    delay(33); // 30 FPS
  }
}

void Supervisor::_init() {
  // set LED's pin to output mode
  pinMode(LEDR, OUTPUT);
  pinMode(LEDG, OUTPUT);
  pinMode(LEDB, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);

  _setState(State::Initializing);
  _updateLed();

  // begin initialization
  if (!BLE.begin()) {
    Logger::Error("Failed to start BLE!");
    while (1)
      ;
  }

  BLE.setLocalName("ImBlue");

  for (const auto &it : m_modules) {
    BLEService &service = it->Service();
    BLE.setAdvertisedService(service);
    BLE.addService(service);
  }

  // start advertising
  BLE.advertise();
  Logger::Info("Started advertising.");
  Logger::Info("MAC: " + BLE.address());

  _setState(State::WaitingForConnection);
  _updateLed();

  Logger::Info("Initialization complete!");
}

void Supervisor::_setState(State state) {
  if (m_state == state) {
    return;
  }

  Logger::Debug(String("State update: ") + (uint8_t)m_state + " " +
                (uint8_t)state);
  m_state = state;
}

void Supervisor::_updateConnection() {
  // listen for BLE peripherals to connect:
  const BLEDevice central = BLE.central();
  if (!central) {
    _setState(State::WaitingForConnection);
    return;
  }

  m_central = central;
  _setState(State::Connected);

  Logger::Info(String("Connected to central: ") + m_central.address());
}

void Supervisor::_updateLed() {
  switch (m_state) {
  case State::Initializing:
    digitalWrite(LEDR, LOW);
    digitalWrite(LEDG, LOW);
    digitalWrite(LEDB, HIGH);
    break;

  case State::WaitingForConnection:
    digitalWrite(LEDR, HIGH);
    digitalWrite(LEDG, HIGH);
    digitalWrite(LEDB, LOW);
    break;

  case State::Connected:
    digitalWrite(LEDR, HIGH);
    digitalWrite(LEDG, LOW);
    digitalWrite(LEDB, HIGH);
    break;

  default:
    digitalWrite(LEDR, LOW);
    digitalWrite(LEDG, HIGH);
    digitalWrite(LEDB, HIGH);
    break;
  }
}

void Supervisor::_updateModules() {
  for (const auto &it : m_modules) {
    it->Update();
  }
}

} // namespace HAL
