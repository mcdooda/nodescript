#include <cstring>
#include "noderuntime.h"
#include "node.h"

NodeRuntime::NodeRuntime(Node* node, ScriptRuntime* scriptRuntime, int nodeCall) :
	m_node(node),
	m_nodeCall(nodeCall),
	m_currentInputPinIndex(-1),
	m_inputValues(nullptr),
	m_outputValues(nullptr),
	m_scriptRuntime(scriptRuntime)
{
	node->createInputValues(this);
	node->createOutputValues(this);
}

NodeRuntime::~NodeRuntime()
{
	delete m_inputValues;
	delete m_outputValues;
}

void NodeRuntime::optimizeLinks()
{
	optimizeInputValueLinks();
	optimizeOutputImpulseLinks();
}

#ifndef NDEBUG
bool NodeRuntime::debugIsInputValuePinIndexValid(int inputPinIndex) const
{
	return m_node->debugIsInputValuePinIndexValid(inputPinIndex);
}

bool NodeRuntime::debugIsOutputValuePinIndexValid(int outputPinIndex) const
{
	return m_node->debugIsOutputValuePinIndexValid(outputPinIndex);
}
#endif

void NodeRuntime::createInputValues(int numValues)
{
	m_inputValues = new PinValue*[numValues];
	memset(m_inputValues, 0, sizeof(PinValue*) * numValues);
}

void NodeRuntime::createOutputValues(int numValues)
{
	m_outputValues = new PinValue[numValues];
	memset(m_outputValues, 0, sizeof(PinValue) * numValues);
}

int NodeRuntime::getInputValueIndexFromPinIndex(int inputPinIndex) const
{
	return inputPinIndex - m_node->m_firstInValuePinIndex;
}

int NodeRuntime::getOutputValueIndexFromPinIndex(int outputPinIndex) const
{
	return outputPinIndex - m_node->m_firstOutValuePinIndex;
}

void NodeRuntime::optimizeInputValueLinks()
{
	Script* script = m_scriptRuntime->getScript();
	Pin outputPin;
	for (int pinIndex = m_node->m_firstInValuePinIndex; pinIndex <= m_node->m_lastInValuePinIndex; pinIndex++)
	{
		script->getOutputPin(m_nodeCall, pinIndex, outputPin);
		assert(script->debugIsNodeCallValid(outputPin.getNodeCall())); // The input pin is not connected to an other pin!
		NodeRuntime* inputRuntime = m_scriptRuntime->getNodeCallRuntime(outputPin.getNodeCall());
		assert(inputRuntime->debugIsOutputValuePinIndexValid(outputPin.getIndex())); // The input pin is connected to an invalid output pin!
		int inIndex = getInputValueIndexFromPinIndex(pinIndex);
		int outIndex = inputRuntime->getOutputValueIndexFromPinIndex(outputPin.getIndex());
		m_inputValues[inIndex] = &inputRuntime->m_outputValues[outIndex];
	}
}

void NodeRuntime::optimizeOutputImpulseLinks()
{
	
}



