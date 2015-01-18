#ifndef FUNCTIONALNODERUNTIME_H
#define FUNCTIONALNODERUNTIME_H

#include "noderuntime.h"

class FunctionalNodeRuntime : public NodeRuntime
{
	typedef NodeRuntime Super;
	public:
		FunctionalNodeRuntime(const Node* node, NodeCall nodeCall);
		~FunctionalNodeRuntime() override;
		
		void execute(PinIndex inputPinIndex) override;
		
	private:
		int m_lastExecutionIndex;
};

#endif // FUNCTIONALNODERUNTIME_H



