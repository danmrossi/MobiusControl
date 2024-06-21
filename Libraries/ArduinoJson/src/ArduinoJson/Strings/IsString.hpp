// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2024, Benoit BLANCHON
// MIT License

#pragma once

#include "C:\Users\Daniel Rossi\Documents\Arduino\libraries\ArduinoJson\src\ArduinoJson\Polyfills\type_traits.hpp"
#include "C:\Users\Daniel Rossi\Documents\Arduino\libraries\ArduinoJson\src\ArduinoJson\Strings\StringAdapter.hpp"

ARDUINOJSON_BEGIN_PRIVATE_NAMESPACE

template <typename T, typename Enable = void>
struct IsString : false_type {};

template <typename T>
struct IsString<
    T, typename make_void<typename StringAdapter<T>::AdaptedString>::type>
    : true_type {};

ARDUINOJSON_END_PRIVATE_NAMESPACE