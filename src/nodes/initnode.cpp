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
	writePin<IntOutPin>(runtime, 42);
	impulse<ImpulseOutPin>(runtime);
}

void InitNode::addedToScript(Script* script, int nodeCall) const
{
	script->addEntryPoint(nodeCall);
}



