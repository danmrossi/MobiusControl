// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2024, Benoit BLANCHON
// MIT License

#pragma once

#include "C:\Users\Daniel Rossi\Documents\Arduino\libraries\ArduinoJson\src\ArduinoJson\Polyfills\type_traits\integral_constant.hpp"
#include "C:\Users\Daniel Rossi\Documents\Arduino\libraries\ArduinoJson\src\ArduinoJson\Polyfills\type_traits\is_same.hpp"
#include "C:\Users\Daniel Rossi\Documents\Arduino\libraries\ArduinoJson\src\ArduinoJson\Polyfills\type_traits\remove_cv.hpp"

ARDUINOJSON_BEGIN_PRIVATE_NAMESPACE

// clang-format off
template <typename T>
struct is_signed : integral_constant<bool,
    is_same<typename remove_cv<T>::type, char>::value ||
    is_same<typename remove_cv<T>::type, signed char>::value ||
    is_same<typename remove_cv<T>::type, signed short>::value ||
    is_same<typename remove_cv<T>::type, signed int>::value ||
    is_same<typename remove_cv<T>::type, signed long>::value ||
    is_same<typename remove_cv<T>::type, signed long long>::value ||
    is_same<typename remove_cv<T>::type, float>::value ||
    is_same<typename remove_cv<T>::type, double>::value> {};
// clang-format on

ARDUINOJSON_END_PRIVATE_NAMESPACE
