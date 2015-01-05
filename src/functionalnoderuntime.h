#ifndef FUNCTIONALNODERUNTIME_H
#define FUNCTIONALNODERUNTIME_H

#include "noderuntime.h"

class FunctionalNodeRuntime : public NodeRuntime
{
	public:
		FunctionalNodeRuntime(Node* node, int nodeCall);
		~FunctionalNodeRuntime() override;
};

#endif // FUNCTIONALNODERUNTIME_H



