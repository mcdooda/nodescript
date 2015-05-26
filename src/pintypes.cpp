#include "pinvalue.h"

#define PINTYPES_CPP

#undef PIN_TYPE

#define PIN_TYPE(Type) PIN_TYPE_DECLARE_ID(Type)

#include "pintypes.h"



