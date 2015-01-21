#include "functionalnoderuntime.h"
#include "node.h"

FunctionalNodeRuntime::FunctionalNodeRuntime(const Node* node, NodeCall nodeCall) : NodeRuntime(node, nodeCall),
	m_lastExecutionIndex(-1)
{
	
}

FunctionalNodeRuntime::~FunctionalNodeRuntime()
{
	
}

void FunctionalNodeRuntime::execute(PinIndex inputPinIndex)
{
	NODESCRIPT_ASSERT(inputPinIndex == FUNCTIONAL_AUTO_EXECUTE_PIN_INDEX);
	if (m_lastExecutionIndex != *m_currentExecutionIndex)
	{
		m_lastExecutionIndex = *m_currentExecutionIndex;
		Super::execute(inputPinIndex);
	}
}



