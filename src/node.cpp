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

const char* Node::getPinName(int pinIndex) const
{
	return "Unknown pin name";
}

NodeRuntime* Node::createRuntime(ScriptRuntime* scriptRuntime, int nodeCall)
{
	return new NodeRuntime(this, scriptRuntime, nodeCall);
}

#ifndef NDEBUG
bool Node::debugIsInputValuePinIndexValid(int pinIndex) const
{
	return pinIndex >= m_firstInValuePinIndex && pinIndex <= m_firstInValuePinIndex;
}

bool Node::debugIsInputImpulsePinIndexValid(int pinIndex) const
{
	return pinIndex >= m_firstInImpulsePinIndex && pinIndex <= m_firstInImpulsePinIndex;
}

bool Node::debugIsOutputValuePinIndexValid(int pinIndex) const
{
	return pinIndex >= m_firstOutValuePinIndex && pinIndex <= m_firstOutValuePinIndex;
}

bool Node::debugIsOutputImpulsePinIndexValid(int pinIndex) const
{
	return pinIndex >= m_firstOutImpulsePinIndex && pinIndex <= m_firstOutImpulsePinIndex;
}
#endif

void Node::createInputValues(NodeRuntime* runtime)
{
	int numInputValuePins = m_lastInValuePinIndex - m_firstInValuePinIndex + 1;
	if (numInputValuePins > 0)
	{
		runtime->createInputValues(numInputValuePins);
	}
}

void Node::createOutputValues(NodeRuntime* runtime)
{
	int numOutputValuePins = m_lastOutValuePinIndex - m_firstOutValuePinIndex + 1;
	if (numOutputValuePins > 0)
	{
		runtime->createOutputValues(numOutputValuePins);
	}
}

void Node::createOutputImpulses(NodeRuntime* runtime)
{
	int numOutputImpulsePins = m_lastOutImpulsePinIndex - m_firstOutImpulsePinIndex + 1;
	if (numOutputImpulsePins > 0)
	{
		runtime->createOutputImpulses(numOutputImpulsePins);
	}
}



