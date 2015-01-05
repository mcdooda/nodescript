#include <iostream>
#include "testnode.h"

namespace node
{

TestNode::TestNode()
{
	inValuePin<IntInPin>();
	inValuePin<FloatInPin>();
	inImpulsePin<ImpulseInPin>();
	
	outValuePin<IntOutPin>();
	outValuePin<FloatOutPin>();
	outImpulsePin<ImpulseOutPin>();
}

void TestNode::execute(NodeRuntime* runtime, PinIndex inputPinIndex) const
{
	int intValue;
	float floatValue;
	readPin<IntInPin>(runtime, intValue);
	readPin<FloatInPin>(runtime, floatValue);
	//std::cout << "Int = " << intValue << " ; Float = " << floatValue << std::endl;
	writePin<IntOutPin>(runtime, intValue);
	writePin<FloatOutPin>(runtime, floatValue);
	impulse<ImpulseOutPin>(runtime);
}

} // node



