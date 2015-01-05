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

} // math
} // node



