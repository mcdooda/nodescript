#ifndef INITNODE_H
#define INITNODE_H

#include "../node.h"

class InitNode : public Node
{
	public:
		NODE(InitNode, "Init");
		
		enum
		{
			IntOutIndex,
			ImpulseOutIndex
		};
		
		typedef PinInfo<IntOutIndex, int> IntOutPin;
		typedef PinInfo<ImpulseOutIndex> ImpulseOutPin;
		
	public:
		InitNode();
		void execute(NodeRuntime* runtime, int inputPinIndex) const override;
		void addedToScript(Script* script, int nodeCall) const override;
		
}; // TestNode

#endif // INITNODE_H
