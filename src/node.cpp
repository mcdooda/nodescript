#include "node.h"
#include "noderuntime.h"

Node::Node()
{
	
}

Node::~Node()
{
	
}

void Node::optimize()
{
	m_inputValuePins.shrink_to_fit();
	m_inputImpulsePins.shrink_to_fit();
		
	m_outputValuePins.shrink_to_fit();
	m_outputImpulsePins.shrink_to_fit();
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
	runtime->createOutputValues(
		  m_inputImpulsePins.size()
		+ m_inputValuePins.size()
		+ m_outputImpulsePins.size()
		+ m_outputValuePins.size()
	); // FIX only output values are necessary
}

void Node::clearOutputValues(NodeRuntime* runtime)
{
	for (int pinIndex : m_outputValuePins)
	{
		runtime->clearOutputValue(pinIndex);
	}
}



