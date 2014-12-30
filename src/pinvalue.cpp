#include "pinvalue.h"

template <>
void readPinValue(const PinValue& pinValue, bool& value)
{
	value = pinValue.boolValue;
}

template <>
void readPinValue(const PinValue& pinValue, int& value)
{
	value = pinValue.intValue;
}

template <>
void readPinValue(const PinValue& pinValue, long& value)
{
	value = pinValue.longValue;
}

template <>
void readPinValue(const PinValue& pinValue, float& value)
{
	value = pinValue.floatValue;
}

template <>
void readPinValue(const PinValue& pinValue, double& value)
{
	value = pinValue.doubleValue;
}

template <>
void writePinValue(PinValue& pinValue, bool value)
{
	pinValue.boolValue = value;
}

template <>
void writePinValue(PinValue& pinValue, int value)
{
	pinValue.intValue = value;
}

template <>
void writePinValue(PinValue& pinValue, long value)
{
	pinValue.longValue = value;
}

template <>
void writePinValue(PinValue& pinValue, float value)
{
	pinValue.floatValue = value;
}

template <>
void writePinValue(PinValue& pinValue, double value)
{
	pinValue.doubleValue = value;
}



