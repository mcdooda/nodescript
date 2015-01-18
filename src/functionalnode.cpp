#include "functionalnode.h"
#include "functionalnoderuntime.h"

FunctionalNode::FunctionalNode() : Node()
{
	
}

FunctionalNode::~FunctionalNode()
{
	
}

void FunctionalNode::execute(NodeRuntime* runtime, PinIndex inputPinIndex) const
{
	assert(inputPinIndex == FUNCTIONAL_AUTO_EXECUTE_PIN_INDEX);
	execute(runtime);
}

NodeRuntime* FunctionalNode::createRuntime(ScriptRuntime* scriptRuntime, NodeCall nodeCall) const
{
	return new FunctionalNodeRuntime(this, nodeCall);
}



