#include "node.h"

Node::Node() :
	m_firstInValuePinIndex(-1),
	m_lastInValuePinIndex(-2),
	m_firstInImpulsePinIndex(-1),
	m_lastInImpulsePinIndex(-2),
	m_firstOutValuePinIndex(-1),
	m_lastOutValuePinIndex(-2),
	m_firstOutImpulsePinIndex(-1),
	m_lastOutImpulsePinIndex(-2)
{
	#ifndef NDEBUG
	m_currentPinIndex = 0;
	#endif
}

Node::~Node()
{
	
}

const char* Node::getPinName(PinIndex pinIndex) const
{
	return "Unknown pin name";
}

NodeRuntime* Node::createRuntime(ScriptRuntime* scriptRuntime, int nodeCall)
{
	return new NodeRuntime(this, nodeCall);
}

#ifndef NDEBUG
bool Node::debugIsInputValuePinIndexValid(PinIndex pinIndex) const
{
	return pinIndex >= m_firstInValuePinIndex && pinIndex <= m_firstInValuePinIndex;
}

bool Node::debugIsInputImpulsePinIndexValid(PinIndex pinIndex) const
{
	return pinIndex >= m_firstInImpulsePinIndex && pinIndex <= m_firstInImpulsePinIndex;
}

bool Node::debugIsOutputValuePinIndexValid(PinIndex pinIndex) const
{
	return pinIndex >= m_firstOutValuePinIndex && pinIndex <= m_firstOutValuePinIndex;
}

bool Node::debugIsOutputImpulsePinIndexValid(PinIndex pinIndex) const
{
	return pinIndex >= m_firstOutImpulsePinIndex && pinIndex <= m_firstOutImpulsePinIndex;
}
#endif



