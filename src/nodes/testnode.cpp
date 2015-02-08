#include <iostream>
#include "testnode.h"

namespace node
{

TestNode::TestNode()
{
	inValuePin<IntInPin>();
	inValuePin<FloatInPin>();
	inValuePin<StringInPin>();
	inImpulsePin<ImpulseInPin>();
	
	outValuePin<IntOutPin>();
	outValuePin<FloatOutPin>();
	outValuePin<StringOutPin>();
	outImpulsePin<ImpulseOutPin>();
}

void TestNode::execute(NodeRuntime* runtime, PinIndex inputPinIndex) const
{
	int intValue;
	float floatValue;
	std::string* stringValue;
	readPin<IntInPin>(runtime, intValue);
	readPin<FloatInPin>(runtime, floatValue);
	readPin<StringInPin>(runtime, stringValue);
	std::cout << "Int = " << intValue << " ; Float = " << floatValue << " ; String = " << *stringValue << std::endl;
	writePin<IntOutPin>(runtime, intValue);
	writePin<FloatOutPin>(runtime, floatValue);
	writePin<StringOutPin>(runtime, stringValue);
	impulse<ImpulseOutPin>(runtime);
}

#if defined(NODESCRIPT_DEBUG) || defined(NODESCRIPT_INTROSPECTION)
const char* TestNode::getPinName(PinIndex pinIndex) const
{
	switch (pinIndex)
	{
		case IntInPin::Index:      return "Int";     break;
		case FloatInPin::Index:    return "Float";   break;
		case StringInPin::Index:   return "String";  break;
		case ImpulseInPin::Index:  return "Impulse"; break;
		case IntOutPin::Index:     return "Int";     break;
		case FloatOutPin::Index:   return "Float";   break;
		case StringOutPin::Index:  return "String";  break;
		case ImpulseOutPin::Index: return "Impulse"; break;
		default: return Super::getPinName(pinIndex); break;
	}
}
#endif

} // node



