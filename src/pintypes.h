#if !defined(PINVALUE_H) && !defined(PINTYPES_CPP)
#error "This file must be included carefully!"
#endif

#include <string>

// native pins
PIN_TYPE(bool);
PIN_TYPE(int);
PIN_TYPE(long);
PIN_TYPE(float);
PIN_TYPE(double);

// pointer pins
PIN_TYPE(std::string*);



