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
	
}

Node::~Node()
{
	
}

void Node::optimize()
{
	#ifdef NODESCRIPT_DEBUG
	int gain = m_pinTypeIds.capacity() * sizeof(PinTypeId);
	#endif
	m_pinTypeIds.shrink_to_fit();
	#ifdef NODESCRIPT_DEBUG
	gain -= m_pinTypeIds.capacity() * sizeof(PinTypeId);
	gain += m_inputPinDefaultValues.capacity() * sizeof(PinValue);
	#endif
	m_inputPinDefaultValues.shrink_to_fit();
	#ifdef NODESCRIPT_DEBUG
	gain -= m_inputPinDefaultValues.capacity() * sizeof(PinValue);
	#endif
	
	#if defined(NODESCRIPT_VERBOSE) && defined(NODESCRIPT_DEBUG)
	std::cerr << "[node " << getNodeName() << "] we won " << gain << " bytes" << std::endl;
	#endif
}

#if defined(NODESCRIPT_DEBUG) || defined(NODESCRIPT_INTROSPECTION)
const char* Node::getNodeName() const
{
	NODESCRIPT_ASSERT_MSG(false, "Node \"%s\" misses a name!", typeid(*this).name());
	return "<Unnamed node>";
}

const char* Node::getPinName(PinIndex pinIndex) const
{
	NODESCRIPT_ASSERT_MSG(false, "Node \"%s\" pin #%d misses a name!", getNodeName(), pinIndex);
	return "<Unknown pin name>";
}
#endif

PinTypeId Node::getPinTypeId(PinIndex pinIndex) const
{
	NODESCRIPT_ASSERT(debugIsPinIndexValid(pinIndex));
	return m_pinTypeIds[pinIndex];
}

NodeRuntime* Node::createRuntime(ScriptRuntime* scriptRuntime, NodeCall nodeCall) const
{
	return new NodeRuntime(this, nodeCall);
}

#ifdef NODESCRIPT_DEBUG
bool Node::debugIsPinIndexValid(PinIndex pinIndex) const
{
	return pinIndex >= 0 && pinIndex < static_cast<int>(m_pinTypeIds.size());
}

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
	std::cerr << "Node: " << getNodeName() << std::endl;
	int numPins = static_cast<int>(m_pinTypeIds.size());
	for (PinIndex pinIndex = 0; pinIndex < numPins; ++pinIndex)
	{
		std::cerr << "pin#" << pinIndex << " \"" << getPinName(pinIndex) << "\": " << debugGetPinType(pinIndex) << std::endl;
	}
	std::cerr << std::endl;
}
#endif

#ifdef NODESCRIPT_INTROSPECTION
PinArchetype Node::getPinArchetype(PinIndex pinIndex) const
{
	if (debugIsInputValuePinIndexValid(pinIndex))         return PinArchetype::INPUT_VALUE;
	else if (debugIsInputImpulsePinIndexValid(pinIndex))  return PinArchetype::INPUT_IMPULSE;
	else if (debugIsOutputValuePinIndexValid(pinIndex))   return PinArchetype::OUTPUT_VALUE;
	else if (debugIsOutputImpulsePinIndexValid(pinIndex)) return PinArchetype::OUTPUT_IMPULSE;
	else                                                  return PinArchetype::INVALID;
}
#endif

int Node::getInputValueIndexFromPinIndex(PinIndex pinIndex) const
{
	return pinIndex - m_firstInValuePinIndex;
}


