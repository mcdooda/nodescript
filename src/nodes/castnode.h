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
		
		typedef PinInfo<ToValueIndex,   ToType  > ToValuePin;
		
	public:
		CastNode()
		{
			inValuePin<FromValuePin>();
			
			outValuePin<ToValuePin>();
		}
		
		void execute(NodeRuntime* runtime) const override
		{
			FromType fromValue;
			readPin<FromValuePin>(runtime, fromValue);
			writePin<ToValuePin>(runtime, static_cast<ToType>(fromValue));
		}
		
		const char* getPinName(PinIndex pinIndex) const override
		{
			switch (pinIndex)
			{
				case FromValuePin::Index: return "Value"; break;
				case ToValuePin::Index:   return "Casted value"; break;
				default: return Super::getPinName(pinIndex); break;
			}
		}
};

#define CAST_NODE(NodeType, NodeName, FromType, ToType) \
	class NodeType : public CastNode<FromType, ToType> \
	{ \
		public: \
			NODE(NodeType, NodeName); \
	};
	
// bool
CAST_NODE(BoolToIntCastNode,     "Bool To Int Cast",      bool,  int);
CAST_NODE(BoolToLongCastNode,    "Bool To Long Cast",     bool,  long);

// int
CAST_NODE(IntToBoolCastNode,     "Int To Bool Cast",      int,   bool);
CAST_NODE(IntToLongCastNode,     "Int To Long Cast",      int,   long);
CAST_NODE(IntToFloatCastNode,    "Int To Float Cast",     int,   float);
CAST_NODE(IntToDoubleCastNode,   "Int To Double Cast",    int,   double);

// long
CAST_NODE(LongToBoolCastNode,    "Long To Bool Cast",     long,  bool);
CAST_NODE(LongToIntCastNode,     "Long To Int Cast",      long,  int);
CAST_NODE(LongToFloatCastNode,   "Long To Float Cast",    long,  float);
CAST_NODE(LongToDoubleCastNode,  "Long To Double Cast",   long,  double);

// float
CAST_NODE(FloatToIntCastNode,    "Float To Int Cast",     float, int);
CAST_NODE(FloatToLongCastNode,   "Float To Long Cast",    float, long);
CAST_NODE(FloatToDoubleCastNode, "Float To Double Cast",  float, double);

// double
CAST_NODE(DoubleToIntCastNode,   "Double To Int Cast",    double, int);
CAST_NODE(DoubleToLongCastNode,  "Double To Long Cast",   double, long);
CAST_NODE(DoubleToFloatCastNode, "Double To Float Cast", double, float);

} // node

#endif // NODES_CASTNODE_H



