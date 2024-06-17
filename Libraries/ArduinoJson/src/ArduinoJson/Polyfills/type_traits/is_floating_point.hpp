// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2024, Benoit BLANCHON
// MIT License

#pragma once

#include "C:\Users\Daniel Rossi\Documents\Arduino\libraries\ArduinoJson\src\ArduinoJson\Polyfills\type_traits\integral_constant.hpp"
#include "C:\Users\Daniel Rossi\Documents\Arduino\libraries\ArduinoJson\src\ArduinoJson\Polyfills\type_traits\is_same.hpp"
#include "C:\Users\Daniel Rossi\Documents\Arduino\libraries\ArduinoJson\src\ArduinoJson\Polyfills\type_traits\remove_cv.hpp"

ARDUINOJSON_BEGIN_PRIVATE_NAMESPACE

template <class T>
struct is_floating_point
    : integral_constant<
          bool,  //
          is_same<float, typename remove_cv<T>::type>::value ||
              is_same<double, typename remove_cv<T>::type>::value> {};

ARDUINOJSON_END_PRIVATE_NAMESPACE
