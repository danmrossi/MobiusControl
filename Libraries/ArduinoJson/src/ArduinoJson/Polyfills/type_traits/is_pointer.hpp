// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2024, Benoit BLANCHON
// MIT License

#pragma once

#include "C:\Users\Daniel Rossi\Documents\Arduino\libraries\ArduinoJson\src\ArduinoJson\Polyfills\type_traits\integral_constant.hpp"

ARDUINOJSON_BEGIN_PRIVATE_NAMESPACE

template <typename T>
struct is_pointer : false_type {};

template <typename T>
struct is_pointer<T*> : true_type {};

ARDUINOJSON_END_PRIVATE_NAMESPACE
