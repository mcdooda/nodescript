#include "initnode.h"
#include "../script.h"
#include "../noderuntime.h"

namespace node
{

InitNode::InitNode()
{
	outImpulsePin<ImpulseOutPin>();
}

void InitNode::execute(NodeRuntime* runtime, PinIndex /*inputPinIndex*/) const
{
	impulse<ImpulseOutPin>(runtime);
}

void InitNode::addedToScript(Script* script, NodeCall nodeCall) const
{
	script->addEntryPoint(nodeCall);
}

#if defined(NODESCRIPT_DEBUG) || defined(NODESCRIPT_INTROSPECTION)
const char* InitNode::getPinName(PinIndex pinIndex) const
{
	switch (pinIndex)
	{
		case ImpulseOutPin::Index: return "Init"; break;
		default: return Super::getPinName(pinIndex); break;
	}
}
#endif

} // node



