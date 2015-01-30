#include <cstring>
#include <typeinfo>
#include <iostream>
#include <algorithm>
#include "noderuntime.h"
#include "node.h"
#include "scriptruntime.h"
#include "pinimpulse.h"
#include "debug.h"

NodeRuntime::NodeRuntime(const Node* node, NodeCall nodeCall) :
	m_inputRuntimes(nullptr),
	m_inputValues(nullptr),
	m_outputValues(nullptr),
	m_outputImpulses(nullptr),
	m_node(node),
	m_nodeCall(nodeCall)
{
	createInputValues();
	createOutputValues();
	createOutputImpulses();
}

NodeRuntime::~NodeRuntime()
{
	delete[] m_inputRuntimes;
	delete[] m_inputValues;
	delete[] m_outputValues;
	delete[] m_outputImpulses;
}

void NodeRuntime::execute(PinIndex inputPinIndex)
{
	m_node->execute(this, inputPinIndex);
}

void NodeRuntime::optimizeLinks(ScriptRuntime* scriptRuntime)
{
	optimizeInputValueLinks(scriptRuntime);
	optimizeOutputImpulseLinks(scriptRuntime);
	m_currentExecutionIndex = scriptRuntime->getCurrentExecutionIndex();
}

#ifdef NODESCRIPT_DEBUG
bool NodeRuntime::debugIsInputValuePinIndexValid(PinIndex pinIndex) const
{
	return m_node->debugIsInputValuePinIndexValid(pinIndex);
}

bool NodeRuntime::debugIsInputImpulsePinIndexValid(PinIndex pinIndex) const
{
	return m_node->debugIsInputImpulsePinIndexValid(pinIndex);
}

bool NodeRuntime::debugIsOutputValuePinIndexValid(PinIndex pinIndex) const
{
	return m_node->debugIsOutputValuePinIndexValid(pinIndex);
}

bool NodeRuntime::debugIsOutputImpulsePinIndexValid(PinIndex pinIndex) const
{
	return m_node->debugIsOutputImpulsePinIndexValid(pinIndex);
}

const char* NodeRuntime::debugGetPinType(PinIndex pinIndex) const
{
	return m_node->debugGetPinType(pinIndex);
}
#endif

void NodeRuntime::createInputValues()
{
	int numValues = m_node->m_lastInValuePinIndex - m_node->m_firstInValuePinIndex + 1;
	if (numValues > 0)
	{
		m_inputRuntimes = new NodeRuntime*[numValues];
		memset(m_inputRuntimes, 0, sizeof(NodeRuntime*) * numValues);
		
		m_inputValues = new PinValue*[numValues];
		memcpy(m_inputValues, m_node->m_inputPinDefaultValues.data(), sizeof(PinValue) * numValues);
	}
}

void NodeRuntime::createOutputValues()
{
	int numValues = m_node->m_lastOutValuePinIndex - m_node->m_firstOutValuePinIndex + 1;
	if (numValues > 0)
	{
		m_outputValues = new PinValue[numValues];
		memset(m_outputValues, 0, sizeof(PinValue) * numValues);
	}
}

void NodeRuntime::createOutputImpulses()
{
	int numImpulses = m_node->m_lastOutImpulsePinIndex - m_node->m_firstOutImpulsePinIndex + 1;
	if (numImpulses > 0)
	{
		m_outputImpulses = new std::vector<PinImpulse>[numImpulses];
	}
}

int NodeRuntime::getInputValueIndexFromPinIndex(PinIndex pinIndex) const
{
	return pinIndex - m_node->m_firstInValuePinIndex;
}

int NodeRuntime::getOutputValueIndexFromPinIndex(PinIndex pinIndex) const
{
	return pinIndex - m_node->m_firstOutValuePinIndex;
}

int NodeRuntime::getOutputImpulseIndexFromPinIndex(PinIndex pinIndex) const
{
	return pinIndex - m_node->m_firstOutImpulsePinIndex;
}

void NodeRuntime::optimizeInputValueLinks(ScriptRuntime* scriptRuntime)
{
	Script* script = scriptRuntime->getScript();
	Pin outputPin;
	for (PinIndex pinIndex = m_node->m_firstInValuePinIndex; pinIndex <= m_node->m_lastInValuePinIndex; ++pinIndex)
	{
		script->getOutputPin(m_nodeCall, pinIndex, outputPin);
		NODESCRIPT_ASSERT_MSG(script->debugIsNodeCallValid(outputPin.getNodeCall()), "The input pin is not connected to an other pin!");
		NodeRuntime* inputRuntime = scriptRuntime->getNodeCallRuntime(outputPin.getNodeCall());
		#ifdef NODESCRIPT_DEBUG
		if (!outputPin.isConnected())
		{
			std::cerr << std::cout << typeid(this).name() << " pin#" << pinIndex << " is not connected!" << std::endl;
			NODESCRIPT_BREAK();
		}
		if (!inputRuntime->debugIsOutputValuePinIndexValid(outputPin.getIndex()))
		{
			std::cerr << m_node->debugGetNodeName()
			          << " pin#" << static_cast<int>(pinIndex)
			          << " (" << m_node->debugGetPinType(pinIndex) << ")"
			          << " and " << inputRuntime->m_node->debugGetNodeName()
			          << " pin#" << static_cast<int>(outputPin.getIndex())
			          << " (" << inputRuntime->m_node->debugGetPinType(pinIndex) << ")"
			          << " are not compatible pins!" << std::endl;
			
			std::cerr << "Current node:" << std::endl;
			m_node->debugPrintPins();
			std::cerr << "Input node:" << std::endl;
			inputRuntime->m_node->debugPrintPins();
			NODESCRIPT_BREAK();
		}
		#endif
		int inIndex = getInputValueIndexFromPinIndex(pinIndex);
		int outIndex = inputRuntime->getOutputValueIndexFromPinIndex(outputPin.getIndex());
		if (inputRuntime->m_node->isFunctional())
		{
			m_inputRuntimes[inIndex] = inputRuntime;
		}
		m_inputValues[inIndex] = &inputRuntime->m_outputValues[outIndex];
	}
}

void NodeRuntime::optimizeOutputImpulseLinks(ScriptRuntime* scriptRuntime)
{
	Script* script = scriptRuntime->getScript();
	std::vector<Pin>* inputPins;
	for (PinIndex pinIndex = m_node->m_firstOutImpulsePinIndex; pinIndex <= m_node->m_lastOutImpulsePinIndex; ++pinIndex)
	{
		script->getInputPins(m_nodeCall, pinIndex, inputPins);
		int numConnectedPins = std::count_if(inputPins->begin(), inputPins->end(), [](const Pin& pin){ return pin.isConnected(); });
		int outIndex = getOutputImpulseIndexFromPinIndex(pinIndex);
		m_outputImpulses[outIndex].reserve(numConnectedPins);
		
		for (const Pin& inputPin : *inputPins)
		{
			if (inputPin.isConnected())
			{
				NODESCRIPT_ASSERT_MSG(script->debugIsNodeCallValid(inputPin.getNodeCall()), "The output node #%d is invalid", inputPin.getNodeCall());
				NodeRuntime* outputRuntime = scriptRuntime->getNodeCallRuntime(inputPin.getNodeCall());
				NODESCRIPT_ASSERT_MSG(outputRuntime->debugIsInputImpulsePinIndexValid(inputPin.getIndex()), "The input pin #%d is invalid", inputPin.getIndex());
				m_outputImpulses[outIndex].emplace_back(outputRuntime, inputPin.getIndex());
			}
		}
	}
}



