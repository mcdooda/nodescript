#include "initnode.h"
#include "../script.h"
#include "../noderuntime.h"

namespace node
{

InitNode::InitNode()
{
	outImpulsePin<ImpulseOutPin>();
}

void InitNode::execute(NodeRuntime* runtime, PinIndex inputPinIndex) const
{
	impulse<ImpulseOutPin>(runtime);
}

void InitNode::addedToScript(Script* script, int nodeCall) const
{
	script->addEntryPoint(nodeCall);
}

} // node



