#include "pinimpulse.h"

PinImpulse::PinImpulse(NodeRuntime* nodeRuntime, PinIndex inputPinIndex) :
	m_nodeRuntime(nodeRuntime),
	m_inputPinIndex(inputPinIndex)
{
	
}

PinImpulse::PinImpulse() : PinImpulse(nullptr, INVALID_PIN_INDEX)
{
	
}

PinImpulse::PinImpulse(const PinImpulse& pinImpulse) : PinImpulse(pinImpulse.m_nodeRuntime, pinImpulse.m_inputPinIndex)
{
	
}

void PinImpulse::operator=(const PinImpulse& pinImpulse)
{
	m_nodeRuntime = pinImpulse.m_nodeRuntime;
	m_inputPinIndex = pinImpulse.m_inputPinIndex;
}



