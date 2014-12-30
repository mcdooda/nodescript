#include <cstring>
#include "noderuntime.h"
#include "node.h"

NodeRuntime::NodeRuntime(Node* node, ScriptRuntime* scriptRuntime, int nodeCall) :
	m_node(node),
	m_nodeCall(nodeCall),
	m_currentInputPinIndex(-1),
	m_scriptRuntime(scriptRuntime)
{
	node->createOutputValues(this);
}

NodeRuntime::~NodeRuntime()
{
	delete m_outputValues;
}

void NodeRuntime::createOutputValues(int numValues)
{
	m_outputValues = new PinValue[numValues];
	memset(m_outputValues, 0, sizeof(PinValue) * numValues);
}

void NodeRuntime::clearOutputValue(int pinIndex)
{
	memset(&m_outputValues[pinIndex], 0, sizeof(PinValue));
}



