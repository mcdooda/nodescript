#ifndef FUNCTIONALNODE_H
#define FUNCTIONALNODE_H

#include "node.h"

class ScriptRuntime;

class FunctionalNode : public Node
{
	public:
		FunctionalNode();
		~FunctionalNode() override;
		
		bool isFunctional() const override { return true; }
		
		void execute(NodeRuntime* runtime, PinIndex inputPinIndex) const override;
		virtual void execute(NodeRuntime* runtime) const = 0;
		NodeRuntime* createRuntime(ScriptRuntime* scriptRuntime, int nodeCall) override;
};

#endif // FUNCTIONALNODE_H



