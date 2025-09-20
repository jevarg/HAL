#pragma once

#include <Arduino.h>

class Logger
{
public:
  enum class LogLevel {
    Debug = 0,
    Info,
    Error
};

  static LogLevel logLevel;

  template<typename T>
  static void Debug(T msg) {
    if (logLevel > LogLevel::Debug) {
      return;
    }

    Serial.print("[DEBUG] ");
    Serial.println(msg);
  };

  template<typename T>
  static void Info(T msg) {
    if (logLevel > LogLevel::Info) {
      return;
    }

    Serial.print("[INFO] ");
    Serial.println(msg);
  };

  template<typename T>
  static void Error(T msg) {
    if (logLevel > LogLevel::Error) {
      return;
    }

    Serial.print("[ERROR] ");
    Serial.println(msg);
  };
};
