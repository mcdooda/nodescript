#include "pin.h"

Pin::Pin(NodeCall nodeCall, PinIndex index) :
	m_nodeCall(nodeCall),
	m_index(index)
{
	
}

Pin::Pin() : Pin(INVALID_NODE_CALL, INVALID_PIN_INDEX)
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



