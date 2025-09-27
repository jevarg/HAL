#pragma once

#include <memory>
#include <vector>

#include "Logger.h"
#include "Module.h"

namespace HAL {

template <typename T>
using EnableIfModule = std::enable_if_t<std::is_base_of<Module, T>::value>;

class Supervisor
{
public:
  enum class State : uint8_t
  {
    Unknown = 0,
    Initializing,
    WaitingForConnection,
    Connected,
};

  template <typename T, typename = EnableIfModule<T>>
  void RegisterModule()
  {
    const uint8_t nextId = m_modules.size();
    std::unique_ptr<Module> mod = std::make_unique<T>(nextId);
    if (!mod->Setup()) {
      Logger::Error("Unable to register module: %s", mod->Name());
      return;
    }

    Logger::Info("Registered module: %s (%d)", mod->Name(), nextId);
    m_modules.push_back(std::move(mod));
  }

  void Run();
  void Update();

private:
  void _init();
  void _setState(State state);
  void _updateConnection();
  void _updateLed();
  void _updateModules();

  State m_state = State::Unknown;
  BLEDevice m_central;
  std::vector<std::unique_ptr<Module>> m_modules;
};

}
