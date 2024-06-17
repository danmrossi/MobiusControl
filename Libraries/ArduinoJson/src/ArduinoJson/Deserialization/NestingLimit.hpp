// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2024, Benoit BLANCHON
// MIT License

#pragma once

#include "C:\Users\Daniel Rossi\Documents\Arduino\libraries\ArduinoJson\src\ArduinoJson\Namespace.hpp"
#include "C:\Users\Daniel Rossi\Documents\Arduino\libraries\ArduinoJson\src\ArduinoJson\Polyfills\assert.hpp"

ARDUINOJSON_BEGIN_PUBLIC_NAMESPACE

namespace DeserializationOption {
class NestingLimit {
 public:
  NestingLimit() : value_(ARDUINOJSON_DEFAULT_NESTING_LIMIT) {}
  explicit NestingLimit(uint8_t n) : value_(n) {}

  NestingLimit decrement() const {
    ARDUINOJSON_ASSERT(value_ > 0);
    return NestingLimit(static_cast<uint8_t>(value_ - 1));
  }

  bool reached() const {
    return value_ == 0;
  }

 private:
  uint8_t value_;
};
}  // namespace DeserializationOption

ARDUINOJSON_END_PUBLIC_NAMESPACE
