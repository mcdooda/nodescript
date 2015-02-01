#ifndef NODES_INITNODE_H
#define NODES_INITNODE_H

#include "../node.h"
#include "../nodecall.h"

namespace node
{

class InitNode : public Node
{
	public:
		NODE(InitNode, "Init");
		typedef Node Super;
		
		enum : PinIndex
		{
			ImpulseOutIndex
		};
		
		typedef PinInfo<ImpulseOutIndex> ImpulseOutPin;
		
	public:
		InitNode();
		void execute(NodeRuntime* runtime, PinIndex inputPinIndex) const override;
		void addedToScript(Script* script, NodeCall nodeCall) const override;

#if defined(NODESCRIPT_DEBUG) || defined(NODESCRIPT_INTROSPECTION)
		const char* getPinName(PinIndex pinIndex) const override;
#endif
}; // TestNode

} // node

#endif // NODES_INITNODE_H
