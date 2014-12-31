#include "initnode.h"
#include "../script.h"
#include "../noderuntime.h"

InitNode::InitNode()
{
	outValuePin<IntOutPin>();
	outImpulsePin<ImpulseOutPin>();
}

void InitNode::execute(NodeRuntime* runtime, int inputPinIndex) const
{
	writePin<IntOutPin>(runtime, 42);
	impulse<ImpulseOutPin>(runtime);
}

void InitNode::addedToScript(Script* script, int nodeCall) const
{
	script->addEntryPoint(nodeCall);
}



