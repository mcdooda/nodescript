#ifndef NODES_CASTNODE_H
#define NODES_CASTNODE_H

namespace node
{

template <class FromType, class ToType>
class CastNode : public FunctionalNode
{
	public:
		typedef FunctionalNode Super;
		
		enum : PinIndex
		{
			FromValueIndex,
			
			ToValueIndex
		};
		
		typedef PinInfo<FromValueIndex, FromType> FromValuePin;
		
		typedef PinInfo<ToValueIndex, ToValueIndex> ToValuePin;
		
	public:
		CastNode()
		{
			inValuePin<FromValuePin>();
			
			outValuePin<ToValuePin>();
		}
		
		void execute(NodeRuntime* runtime) const override
		{
			const FromType& fromValue = readPin<FromValueIndex>(runtime);
			writePin<ToValuePin>(runtime, static_cast<ToType>(fromType));
		}
};

} // node

#endif // NODES_CASTNODE_H



