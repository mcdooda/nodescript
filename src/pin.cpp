#include "pin.h"

Pin::Pin() :
	m_nodeCall(-1),
	m_index(-1)
{
	
}

Pin::Pin(int nodeCall, int index) :
	m_nodeCall(nodeCall),
	m_index(index)
{
	
}

Pin::Pin(const Pin& pin) : Pin(pin.m_nodeCall, pin.m_index)
{
	
}

void Pin::copy(const Pin& pin)
{
	m_nodeCall = pin.m_nodeCall;
	m_index = pin.m_index;
}



