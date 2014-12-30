#include <iostream>
#include "testnode.h"

TestNode::TestNode()
{
	inImpulsePin<ImpulseInPin>();
	inValuePin<IntInPin>();
	
	outImpulsePin<ImpulseOutPin>();
	outValuePin<IntOutPin>();
}

void TestNode::execute(NodeRuntime* runtime) const
{
	int value;
	readPin<IntInPin>(runtime, value);
	std::cout << "TESTNODE! " << value << std::endl;
}



