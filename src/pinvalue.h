#ifndef PINVALUE_H
#define PINVALUE_H

#include <type_traits>

union PinValue
{
	bool boolValue;
	int intValue;
	long longValue;
	float floatValue;
	double doubleValue;
	void* pointerValue;
};

template <class T>
class PinType
{
	public:
		enum { Valid = false };
};

#ifndef NDEBUG
#define PIN_TYPE(Type) 				\
	template <>						\
	class PinType<Type>				\
	{								\
		public:						\
			static char id;			\
			enum { Valid = true };	\
	};								\

#define PIN_TYPE_VALID(Type) PinType<Type>::Valid
#define PIN_TYPE_ID(Type) &PinType<Type>::id
#define PIN_TYPE_IMPULSE_ID nullptr

typedef char* PinTypeId;

#include "pintypes.h"
#endif

// read
template <class T>
void readPinValue(const PinValue& pinValue, T& value)
{
	static_assert(std::is_pointer<T>::value, "Incompatible pin type!");
	#ifndef NDEBUG
	static_assert(PIN_TYPE_VALID(T), "This pin type is not registered!");
	#endif
	value = static_cast<T>(pinValue.pointerValue);
}

template <> void readPinValue(const PinValue& pinValue, bool& value);
template <> void readPinValue(const PinValue& pinValue, int& value);
template <> void readPinValue(const PinValue& pinValue, long& value);
template <> void readPinValue(const PinValue& pinValue, float& value);
template <> void readPinValue(const PinValue& pinValue, double& value);

// write
template <class T>
void writePinValue(PinValue& pinValue, T value)
{
	static_assert(std::is_pointer<T>::value, "Incompatible pin type!");
	pinValue.pointerValue = value;
}

template <> void writePinValue(PinValue& pinValue, bool value);
template <> void writePinValue(PinValue& pinValue, int value);
template <> void writePinValue(PinValue& pinValue, long value);
template <> void writePinValue(PinValue& pinValue, float value);
template <> void writePinValue(PinValue& pinValue, double value);

#endif // PINVALUE_H



