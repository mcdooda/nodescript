#ifndef NDEBUG

#include "pinvalue.h"

#define PINTYPES_CPP

#undef PIN_TYPE

#define PIN_TYPE(Type) char PinType<Type>::id

#include "pintypes.h"

#endif



