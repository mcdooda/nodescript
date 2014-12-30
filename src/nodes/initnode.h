#ifndef INITNODE_H
#define INITNODE_H

#include "../node.h"

class InitNode : public Node
{
	public:
		NODE(InitNode, "Init");
		
		enum
		{
			ImpulseOutIndex,
			IntOutIndex
		};
		
		typedef PinInfo<ImpulseOutIndex> ImpulseOutPin;
		typedef PinInfo<IntOutIndex, int> IntOutPin;
		
	public:
		InitNode();
		void execute(NodeRuntime* runtime) const override;
		void addedToScript(Script* script, int nodeCall) const override;
		
}; // TestNode

#endif // INITNODE_H
