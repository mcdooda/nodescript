#ifndef TESTNODE_H
#define TESTNODE_H

#include "../node.h"

class TestNode : public Node
{
	public:
		NODE(TestNode, "Test Node 1");
		
		enum
		{
			ImpulseInIndex,
			IntInIndex,
			ImpulseOutIndex,
			IntOutIndex
		};
		
		typedef PinInfo<ImpulseInIndex> ImpulseInPin;
		typedef PinInfo<IntInIndex, int> IntInPin;
		
		typedef PinInfo<ImpulseOutIndex> ImpulseOutPin;
		typedef PinInfo<IntOutIndex, int> IntOutPin;
		
	public:
		TestNode();
		void execute(NodeRuntime* runtime, int inputPinIndex) const override;
}; // TestNode

#endif // TESTNODE_H



