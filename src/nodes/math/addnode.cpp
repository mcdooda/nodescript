#include "addnode.h"

namespace node
{
namespace math
{

AddNode::AddNode()
{
	inValuePin<Int1InPin>();
	inValuePin<Int2InPin>();
	
	outValuePin<IntOutPin>();
}

void AddNode::execute(NodeRuntime* runtime) const
{
	int int1;
	readPin<Int1InPin>(runtime, int1);
	
	int int2;
	readPin<Int2InPin>(runtime, int2);
	
	writePin<IntOutPin>(runtime, int1 + int2);
}

#if defined(NODESCRIPT_DEBUG) || defined(NODESCRIPT_INTROSPECTION)
const char* AddNode::getPinName(PinIndex pinIndex) const
{
	switch (pinIndex)
	{
		case Int1InPin::Index: return "Int 1"; break;
		case Int2InPin::Index: return "Int 2"; break;
		case IntOutPin::Index: return "Result"; break;
		default: return Super::getPinName(pinIndex); break;
	}
}
#endif

} // math
} // node


