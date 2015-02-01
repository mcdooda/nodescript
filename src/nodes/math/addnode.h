#ifndef NODES_MATH_ADD_H
#define NODES_MATH_ADD_H

#include "../../functionalnode.h"

namespace node
{
namespace math
{

class AddNode : public FunctionalNode
{
	typedef FunctionalNode Super;
	public:
		NODE(AddNode, "Add");
		
		enum : PinIndex
		{
			Int1InIndex,
			Int2InIndex,
			
			IntOutIndex
		};
		
		typedef PinInfo<Int1InIndex, int> Int1InPin;
		typedef PinInfo<Int2InIndex, int> Int2InPin;
		
		typedef PinInfo<IntOutIndex, int> IntOutPin;
		
	public:
		AddNode();
		void execute(NodeRuntime* runtime) const override;
		#if defined(NODESCRIPT_DEBUG) || defined(NODESCRIPT_INTROSPECTION)
		const char* getPinName(PinIndex pinIndex) const override;
		#endif
};

} // math
} // node

#endif // NODES_MATH_ADD_H



