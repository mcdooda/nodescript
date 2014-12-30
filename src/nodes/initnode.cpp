#include <iostream>
#include "initnode.h"
#include "../script.h"
#include "../noderuntime.h"

InitNode::InitNode()
{
	outImpulsePin<ImpulseOutPin>();
	outValuePin<IntOutPin>();
}

void InitNode::execute(NodeRuntime* runtime) const
{
	std::cout << "INIT!" << std::endl;
	runtime->writeOutputPin<IntOutPin>(42);
	runtime->impulse<ImpulseOutPin>();
}

void InitNode::addedToScript(Script* script, int nodeCall) const
{
	script->addEntryPoint(nodeCall);
}



