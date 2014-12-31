#ifndef TESTNODE_H
#define TESTNODE_H

#include "../node.h"

class TestNode : public Node
{
	public:
		NODE(TestNode, "Test Node 1");
		
		enum
		{
			IntInIndex,
			ImpulseInIndex,
			IntOutIndex,
			ImpulseOutIndex
		};
		
		typedef PinInfo<IntInIndex, int> IntInPin;
		typedef PinInfo<ImpulseInIndex> ImpulseInPin;
		
		typedef PinInfo<IntOutIndex, int> IntOutPin;
		typedef PinInfo<ImpulseOutIndex> ImpulseOutPin;
		
	public:
		TestNode();
		void execute(NodeRuntime* runtime, PinIndex inputPinIndex) const override;
}; // TestNode

#endif // TESTNODE_H



