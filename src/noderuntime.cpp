#include <cstring>
#include "noderuntime.h"
#include "node.h"
#include "scriptruntime.h"
#include "pinimpulse.h"

NodeRuntime::NodeRuntime(Node* node, int nodeCall) :
	m_node(node),
	m_nodeCall(nodeCall),
	m_inputValues(nullptr),
	m_outputValues(nullptr),
	m_outputImpulses(nullptr)
{
	m_node->createInputValues(this);
	m_node->createOutputValues(this);
	m_node->createOutputImpulses(this);
}

NodeRuntime::~NodeRuntime()
{
	delete m_inputValues;
	delete m_outputValues;
	delete m_outputImpulses;
}

void NodeRuntime::execute(int inputPinIndex)
{
	m_node->execute(this, inputPinIndex);
}

void NodeRuntime::optimizeLinks(ScriptRuntime* scriptRuntime)
{
	optimizeInputValueLinks(scriptRuntime);
	optimizeOutputImpulseLinks(scriptRuntime);
}

#ifndef NDEBUG
bool NodeRuntime::debugIsInputValuePinIndexValid(int pinIndex) const
{
	return m_node->debugIsInputValuePinIndexValid(pinIndex);
}

bool NodeRuntime::debugIsInputImpulsePinIndexValid(int pinIndex) const
{
	return m_node->debugIsInputImpulsePinIndexValid(pinIndex);
}

bool NodeRuntime::debugIsOutputValuePinIndexValid(int pinIndex) const
{
	return m_node->debugIsOutputValuePinIndexValid(pinIndex);
}

bool NodeRuntime::debugIsOutputImpulsePinIndexValid(int pinIndex) const
{
	return m_node->debugIsOutputImpulsePinIndexValid(pinIndex);
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

void NodeRuntime::createOutputImpulses(int numImpulses)
{
	m_outputImpulses = new PinImpulse[numImpulses];
}

int NodeRuntime::getInputValueIndexFromPinIndex(int pinIndex) const
{
	return pinIndex - m_node->m_firstInValuePinIndex;
}

int NodeRuntime::getOutputValueIndexFromPinIndex(int pinIndex) const
{
	return pinIndex - m_node->m_firstOutValuePinIndex;
}

int NodeRuntime::getOutputImpulseIndexFromPinIndex(int pinIndex) const
{
	return pinIndex - m_node->m_firstOutImpulsePinIndex;
}

void NodeRuntime::optimizeInputValueLinks(ScriptRuntime* scriptRuntime)
{
	Script* script = scriptRuntime->getScript();
	Pin outputPin;
	for (int pinIndex = m_node->m_firstInValuePinIndex; pinIndex <= m_node->m_lastInValuePinIndex; pinIndex++)
	{
		script->getOutputPin(m_nodeCall, pinIndex, outputPin);
		assert(script->debugIsNodeCallValid(outputPin.getNodeCall())); // The input pin is not connected to an other pin!
		NodeRuntime* inputRuntime = scriptRuntime->getNodeCallRuntime(outputPin.getNodeCall());
		assert(inputRuntime->debugIsOutputValuePinIndexValid(outputPin.getIndex())); // The input pin is connected to an invalid output pin!
		int inIndex = getInputValueIndexFromPinIndex(pinIndex);
		int outIndex = inputRuntime->getOutputValueIndexFromPinIndex(outputPin.getIndex());
		m_inputValues[inIndex] = &inputRuntime->m_outputValues[outIndex];
	}
}

void NodeRuntime::optimizeOutputImpulseLinks(ScriptRuntime* scriptRuntime)
{
	Script* script = scriptRuntime->getScript();
	Pin inputPin;
	for (int pinIndex = m_node->m_firstOutImpulsePinIndex; pinIndex <= m_node->m_lastOutImpulsePinIndex; pinIndex++)
	{
		script->getInputPin(m_nodeCall, pinIndex, inputPin);
		if (inputPin.isConnected())
		{
			assert(script->debugIsNodeCallValid(inputPin.getNodeCall())); // The output node is invalid
			NodeRuntime* outputRuntime = scriptRuntime->getNodeCallRuntime(inputPin.getNodeCall());
			assert(outputRuntime->debugIsInputImpulsePinIndexValid(inputPin.getIndex())); // The input pin is invalid
			int outIndex = getOutputImpulseIndexFromPinIndex(pinIndex);
			m_outputImpulses[outIndex] = PinImpulse(outputRuntime, inputPin.getIndex());
		}
	}
}



