// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2024, Benoit BLANCHON
// MIT License

#pragma once

#include "C:\Users\Daniel Rossi\Documents\Arduino\libraries\ArduinoJson\src\ArduinoJson\Namespace.hpp"

ARDUINOJSON_BEGIN_PRIVATE_NAMESPACE

struct VariantTag {};

template <typename T>
struct IsVariant : is_base_of<VariantTag, T> {};

ARDUINOJSON_END_PRIVATE_NAMESPACE
