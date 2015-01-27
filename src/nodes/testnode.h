#ifndef NODES_TESTNODE_H
#define NODES_TESTNODE_H

#include "../node.h"

namespace node
{

class TestNode : public Node
{
	public:
		NODE(TestNode, "Test Node 1");
		typedef Node Super;
		
		enum : PinIndex
		{
			IntInIndex,
			FloatInIndex,
			StringInIndex,
			ImpulseInIndex,
			IntOutIndex,
			FloatOutIndex,
			StringOutIndex,
			ImpulseOutIndex
		};
		
		typedef PinInfo<IntInIndex,     int         > IntInPin;
		typedef PinInfo<FloatInIndex,   float       > FloatInPin;
		typedef PinInfo<StringInIndex,  std::string*> StringInPin;
		typedef PinInfo<ImpulseInIndex              > ImpulseInPin;
		
		typedef PinInfo<IntOutIndex,    int         > IntOutPin;
		typedef PinInfo<FloatOutIndex,  float       > FloatOutPin;
		typedef PinInfo<StringOutIndex, std::string*> StringOutPin;
		typedef PinInfo<ImpulseOutIndex             > ImpulseOutPin;
		
	public:
		TestNode();
		void execute(NodeRuntime* runtime, PinIndex inputPinIndex) const override;
		const char* getPinName(PinIndex pinIndex) const override;
}; // TestNode

} // node

#endif // NODES_TESTNODE_H



