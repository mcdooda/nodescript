#include "pin.h"

Pin::Pin(int nodeCall, PinIndex index) :
	m_nodeCall(nodeCall),
	m_index(index)
{
	
}

Pin::Pin() : Pin(-1, INVALID_PIN_INDEX)
{
	
}

Pin::Pin(const Pin& pin) : Pin(pin.m_nodeCall, pin.m_index)
{
	
}

void Pin::operator=(const Pin& pin)
{
	m_nodeCall = pin.m_nodeCall;
	m_index = pin.m_index;
}



