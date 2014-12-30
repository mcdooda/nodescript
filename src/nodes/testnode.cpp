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
	std::cout << "TESTNODE!" << std::endl;
}



