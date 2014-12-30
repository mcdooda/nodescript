#include "node.h"

Node::Node() :
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

void Node::optimize()
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

void Node::createOutputValues(NodeRuntime* runtime)
{
	int numOutputValuePins = m_lastOutValuePinIndex - m_firstOutValuePinIndex + 1;
	if (numOutputValuePins > 0)
	{
		runtime->createOutputValues(numOutputValuePins);
	}
}

void Node::clearOutputValues(NodeRuntime* runtime)
{
	for (int pinIndex = m_firstOutValuePinIndex; pinIndex <= m_lastOutValuePinIndex; pinIndex++)
	{
		runtime->clearOutputValue(pinIndex);
	}
}



