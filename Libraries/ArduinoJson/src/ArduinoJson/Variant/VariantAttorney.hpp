// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2024, Benoit BLANCHON
// MIT License

#pragma once

#include "C:\Users\Daniel Rossi\Documents\Arduino\libraries\ArduinoJson\src\ArduinoJson\Polyfills\attributes.hpp"
#include "C:\Users\Daniel Rossi\Documents\Arduino\libraries\ArduinoJson\src\ArduinoJson\Polyfills\type_traits.hpp"
#include "C:\Users\Daniel Rossi\Documents\Arduino\libraries\ArduinoJson\src\ArduinoJson\Variant\VariantData.hpp"
#include "C:\Users\Daniel Rossi\Documents\Arduino\libraries\ArduinoJson\src\ArduinoJson\Variant\VariantTo.hpp"
#include "C:\Users\Daniel Rossi\Documents\Arduino\libraries\ArduinoJson\src\ArduinoJson\Variant\JsonVariantConst.hpp"

ARDUINOJSON_BEGIN_PRIVATE_NAMESPACE

// Grants access to the internal variant API
class VariantAttorney {
 public:
  template <typename TClient>
  static auto getResourceManager(TClient& client)
      -> decltype(client.getResourceManager()) {
    return client.getResourceManager();
  }

  template <typename TClient>
  static auto getData(TClient& client) -> decltype(client.getData()) {
    return client.getData();
  }

  template <typename TClient>
  static VariantData* getOrCreateData(TClient& client) {
    return client.getOrCreateData();
  }
};

ARDUINOJSON_END_PRIVATE_NAMESPACE
