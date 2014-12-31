#include "pin.h"

Pin::Pin(int nodeCall, int index) :
	m_nodeCall(nodeCall),
	m_index(index)
{
	
}

Pin::Pin() : Pin(-1, -1)
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



