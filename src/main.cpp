#include <Arduino.h>
#include <Supervisor.h>

#include "modules/Test.h"
#include "modules/Fan.h"
#include "modules/Intercom.h"

constexpr uint32_t FPS = 30;
constexpr uint32_t DelayTimer = 1000 / FPS;

HAL::Supervisor supervisor;

void setup() {
  Logger::logLevel = Logger::LogLevel::Debug;

  Serial.begin(9600);
  while (!Serial) {}
  delay(1000);

  Logger::Info("Let's get started!");
  supervisor.RegisterModule<HAL::Intercom>();
  supervisor.RegisterModule<HAL::Fan>();
  supervisor.RegisterModule<HAL::Test>();
  supervisor.Run();

  Logger::Info("Setup complete!");
}

void loop() {
  supervisor.Update();
  delay(DelayTimer);
}