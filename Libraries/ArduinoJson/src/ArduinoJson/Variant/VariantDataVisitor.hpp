// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2024, Benoit BLANCHON
// MIT License

#pragma once

#include "C:\Users\Daniel Rossi\Documents\Arduino\libraries\ArduinoJson\src\ArduinoJson\Array\ArrayData.hpp"
#include "C:\Users\Daniel Rossi\Documents\Arduino\libraries\ArduinoJson\src\ArduinoJson\Numbers\JsonFloat.hpp"
#include "C:\Users\Daniel Rossi\Documents\Arduino\libraries\ArduinoJson\src\ArduinoJson\Numbers\JsonInteger.hpp"
#include "C:\Users\Daniel Rossi\Documents\Arduino\libraries\ArduinoJson\src\ArduinoJson\Object\ObjectData.hpp"

ARDUINOJSON_BEGIN_PRIVATE_NAMESPACE

template <typename TResult>
struct VariantDataVisitor {
  typedef TResult result_type;

  template <typename T>
  TResult visit(const T&) {
    return TResult();
  }
};

ARDUINOJSON_END_PRIVATE_NAMESPACE
