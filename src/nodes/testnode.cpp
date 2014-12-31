#include <iostream>
#include "testnode.h"

TestNode::TestNode()
{
	inValuePin<IntInPin>();
	inImpulsePin<ImpulseInPin>();
	
	outValuePin<IntOutPin>();
	outImpulsePin<ImpulseOutPin>();
}

void TestNode::execute(NodeRuntime* runtime, int inputPinIndex) const
{
	int value;
	readPin<IntInPin>(runtime, value);
	std::cout << "Int = " << value << std::endl;
	writePin<IntOutPin>(runtime, value);
	impulse<ImpulseOutPin>(runtime);
}



