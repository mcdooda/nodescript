#include <iostream>
#include <typeinfo>
#include "node.h"

Node::Node() :
	m_firstInValuePinIndex(INVALID_PIN_INDEX_MIN_1),
	m_lastInValuePinIndex(INVALID_PIN_INDEX_MIN_2),
	m_firstInImpulsePinIndex(INVALID_PIN_INDEX_MIN_1),
	m_lastInImpulsePinIndex(INVALID_PIN_INDEX_MIN_2),
	m_firstOutValuePinIndex(INVALID_PIN_INDEX_MIN_1),
	m_lastOutValuePinIndex(INVALID_PIN_INDEX_MIN_2),
	m_firstOutImpulsePinIndex(INVALID_PIN_INDEX_MIN_1),
	m_lastOutImpulsePinIndex(INVALID_PIN_INDEX_MIN_2)
{
	#ifndef NDEBUG
	m_debugCurrentPinIndex = 0;
	#endif
}

Node::~Node()
{
	
}

#ifndef NDEBUG
const char* Node::debugGetNodeName() const
{
	std::cerr << "Node \"" << typeid(*this).name() << "\" misses a name!" << std::endl;
	assert(false);
	return "<Unnamed node>";
}
#endif

const char* Node::getPinName(PinIndex pinIndex) const
{
	return "<Unknown pin name>";
}

NodeRuntime* Node::createRuntime(ScriptRuntime* scriptRuntime, int nodeCall) const
{
	return new NodeRuntime(this, nodeCall);
}

#ifndef NDEBUG
bool Node::debugIsInputValuePinIndexValid(PinIndex pinIndex) const
{
	return pinIndex >= m_firstInValuePinIndex && pinIndex <= m_lastInValuePinIndex;
}

bool Node::debugIsInputImpulsePinIndexValid(PinIndex pinIndex) const
{
	return pinIndex >= m_firstInImpulsePinIndex && pinIndex <= m_lastInImpulsePinIndex;
}

bool Node::debugIsOutputValuePinIndexValid(PinIndex pinIndex) const
{
	return pinIndex >= m_firstOutValuePinIndex && pinIndex <= m_lastOutValuePinIndex;
}

bool Node::debugIsOutputImpulsePinIndexValid(PinIndex pinIndex) const
{
	return pinIndex >= m_firstOutImpulsePinIndex && pinIndex <= m_lastOutImpulsePinIndex;
}

const char* Node::debugGetPinType(PinIndex pinIndex) const
{
	if      (pinIndex == INVALID_PIN_INDEX_MIN_1)           return "Invalid Pin Index Min 1";
	else if (pinIndex == INVALID_PIN_INDEX_MIN_2)           return "Invalid Pin Index Min 2";
	else if (pinIndex == INVALID_PIN_INDEX)                 return "Invalid Pin Index";
	else if (pinIndex == ENTRY_POINT_PIN_INDEX)             return "Entry Point";
	else if (pinIndex == FUNCTIONAL_AUTO_EXECUTE_PIN_INDEX) return "Functional Auto Execute";
	else if (debugIsInputValuePinIndexValid(pinIndex))      return "Input Value";
	else if (debugIsInputImpulsePinIndexValid(pinIndex))    return "Input Impulse";
	else if (debugIsOutputValuePinIndexValid(pinIndex))     return "Output Value";
	else if (debugIsOutputImpulsePinIndexValid(pinIndex))   return "Output Impulse";
	else                                                    return "Unknown Pin Index";
}

void Node::debugPrintPins() const
{
	std::cerr << "Node: " << debugGetNodeName() << std::endl;
	for (PinIndex pinIndex = 0; pinIndex < m_debugCurrentPinIndex; ++pinIndex)
	{
		std::cerr << "pin#" << (int)pinIndex << " \"" << getPinName(pinIndex) << "\": " << debugGetPinType(pinIndex) << std::endl;
	}
	std::cerr << std::endl;
}
#endif



