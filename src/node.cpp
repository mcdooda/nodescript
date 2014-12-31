#include "node.h"

Node::Node() :
	m_firstInValuePinIndex(-1),
	m_lastInValuePinIndex(-2),
	m_firstOutValuePinIndex(-1),
	m_lastOutValuePinIndex(-2)
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
bool Node::debugIsInputValuePinIndexValid(int inputPinIndex) const
{
	return inputPinIndex >= m_firstInValuePinIndex && inputPinIndex <= m_firstInValuePinIndex;
}

bool Node::debugIsOutputValuePinIndexValid(int outputPinIndex) const
{
	return outputPinIndex >= m_firstOutValuePinIndex && outputPinIndex <= m_firstOutValuePinIndex;
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



