#pragma once

#include <Arduino.h>
#include <stdarg.h>

class Logger
{
public:
  enum class LogLevel {
    Debug = 0,
    Info,
    Error
};

  static LogLevel logLevel;

  static void Debug(const char* format, ...) {
    if (logLevel > LogLevel::Debug) {
      return;
    }

    va_list args;
    va_start(args, format);
    print("[DEBUG] ", format, args);
    va_end(args);
  };

  static void Info(const char* format, ...) {
    if (logLevel > LogLevel::Info) {
      return;
    }

    va_list args;
    va_start(args, format);
    print("[INFO] ", format, args);
    va_end(args);
  };

  static void Error(const char* format, ...) {
    if (logLevel > LogLevel::Error) {
      return;
    }

    va_list args;
    va_start(args, format);
    print("[ERROR] ", format, args);
    va_end(args);
  };

private:
  static void print(const char* preface, const char* format, va_list args) {
    const auto bufferSize = vsnprintf(nullptr, 0, format, args);
    std::string buffer(bufferSize, 0);

    vsprintf(&buffer[0], format, args);

    Serial.print(preface);
    Serial.println(buffer.c_str());
  }
};
