// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2024, Benoit BLANCHON
// MIT License

#pragma once

#if __cplusplus < 201103L && (!defined(_MSC_VER) || _MSC_VER < 1910)
#  error ArduinoJson requires C++11 or newer. Configure your compiler for C++11 or downgrade ArduinoJson to 6.20.
#endif

#include "C:\Users\Daniel Rossi\Documents\Arduino\libraries\ArduinoJson\src\ArduinoJson\Configuration.hpp"

// Include Arduino.h before stdlib.h to avoid conflict with atexit()
// https://github.com/bblanchon/ArduinoJson/pull/1693#issuecomment-1001060240
#if ARDUINOJSON_ENABLE_ARDUINO_STRING || ARDUINOJSON_ENABLE_ARDUINO_STREAM || \
    ARDUINOJSON_ENABLE_ARDUINO_PRINT ||                                       \
    (ARDUINOJSON_ENABLE_PROGMEM && defined(ARDUINO))
#  include <Arduino.h>
#endif

#if !ARDUINOJSON_DEBUG
#  ifdef __clang__
#    pragma clang system_header
#  elif defined __GNUC__
#    pragma GCC system_header
#  endif
#endif

#include "C:\Users\Daniel Rossi\Documents\Arduino\libraries\ArduinoJson\src\ArduinoJson\Array\JsonArray.hpp"
#include "C:\Users\Daniel Rossi\Documents\Arduino\libraries\ArduinoJson\src\ArduinoJson\Object\JsonObject.hpp"
#include "C:\Users\Daniel Rossi\Documents\Arduino\libraries\ArduinoJson\src\ArduinoJson\Variant\JsonVariantConst.hpp"

#include "C:\Users\Daniel Rossi\Documents\Arduino\libraries\ArduinoJson\src\ArduinoJson\Document\JsonDocument.hpp"

#include "C:\Users\Daniel Rossi\Documents\Arduino\libraries\ArduinoJson\src\ArduinoJson\Array\ArrayImpl.hpp"
#include "C:\Users\Daniel Rossi\Documents\Arduino\libraries\ArduinoJson\src\ArduinoJson\Array\ElementProxy.hpp"
#include "C:\Users\Daniel Rossi\Documents\Arduino\libraries\ArduinoJson\src\ArduinoJson\Array\Utilities.hpp"
#include "C:\Users\Daniel Rossi\Documents\Arduino\libraries\ArduinoJson\src\ArduinoJson\Collection\CollectionImpl.hpp"
#include "C:\Users\Daniel Rossi\Documents\Arduino\libraries\ArduinoJson\src\ArduinoJson\Memory\VariantPoolImpl.hpp"
#include "C:\Users\Daniel Rossi\Documents\Arduino\libraries\ArduinoJson\src\ArduinoJson\Object\MemberProxy.hpp"
#include "C:\Users\Daniel Rossi\Documents\Arduino\libraries\ArduinoJson\src\ArduinoJson\Object\ObjectImpl.hpp"
#include "C:\Users\Daniel Rossi\Documents\Arduino\libraries\ArduinoJson\src\ArduinoJson\Variant\ConverterImpl.hpp"
#include "C:\Users\Daniel Rossi\Documents\Arduino\libraries\ArduinoJson\src\ArduinoJson\Variant\JsonVariantCopier.hpp"
#include "C:\Users\Daniel Rossi\Documents\Arduino\libraries\ArduinoJson\src\ArduinoJson\Variant\VariantCompare.hpp"
#include "C:\Users\Daniel Rossi\Documents\Arduino\libraries\ArduinoJson\src\ArduinoJson\Variant\VariantRefBaseImpl.hpp"

#include "C:\Users\Daniel Rossi\Documents\Arduino\libraries\ArduinoJson\src\ArduinoJson\Json\JsonDeserializer.hpp"
#include "C:\Users\Daniel Rossi\Documents\Arduino\libraries\ArduinoJson\src\ArduinoJson\Json\JsonSerializer.hpp"
#include "C:\Users\Daniel Rossi\Documents\Arduino\libraries\ArduinoJson\src\ArduinoJson\Json\PrettyJsonSerializer.hpp"
#include "C:\Users\Daniel Rossi\Documents\Arduino\libraries\ArduinoJson\src\ArduinoJson\MsgPack\MsgPackDeserializer.hpp"
#include "C:\Users\Daniel Rossi\Documents\Arduino\libraries\ArduinoJson\src\ArduinoJson\MsgPack\MsgPackSerializer.hpp"

#include "C:\Users\Daniel Rossi\Documents\Arduino\libraries\ArduinoJson\src\ArduinoJson\compatibility.hpp"
