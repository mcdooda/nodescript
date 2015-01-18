#ifndef CONSTANTVALUENODE_H
#define CONSTANTVALUENODE_H

#include <iostream>
#include "../functionalnode.h"
#include "../functionalnoderuntime.h"

namespace node
{

template <class T>
class ConstantValueNodeRuntime : public FunctionalNodeRuntime
{
	public:
		ConstantValueNodeRuntime(const Node* node, NodeCall nodeCall) : FunctionalNodeRuntime(node, nodeCall) {}
		
		void setValue(const T& value);
};

template <class T>
class ConstantValueNode : public FunctionalNode
{
	public:
		NODE_RUNTIME_TYPE(ConstantValueNodeRuntime<T>);
		typedef FunctionalNode Super;
		
		enum
		{
			ValueOutIndex
		};
		
		typedef PinInfo<ValueOutIndex, T> ValueOutPin;
		
	public:
		ConstantValueNode()
		{
			outValuePin<ValueOutPin>();
		}
		
		void execute(NodeRuntime* runtime) const override {}
		
		const char* getPinName(PinIndex pinIndex) const override
		{
			switch (pinIndex)
			{
				case ValueOutPin::Index: return "Value"; break;
				default: return Super::getPinName(pinIndex); break;
			}
		}
};

template <class T>
void ConstantValueNodeRuntime<T>::setValue(const T& value)
{
	writePin<typename ConstantValueNode<T>::ValueOutPin>(value);
}

class BoolConstantValueNode : public ConstantValueNode<bool>
{
	public:
		NODE(BoolConstantValueNode, "Bool Value");
};

class IntConstantValueNode : public ConstantValueNode<int>
{
	public:
		NODE(IntConstantValueNode, "Int Value");
};

class LongConstantValueNode : public ConstantValueNode<long>
{
	public:
		NODE(LongConstantValueNode, "Long Value");
};

class FloatConstantValueNode : public ConstantValueNode<float>
{
	public:
		NODE(FloatConstantValueNode, "Float Value");
};

class DoubleConstantValueNode : public ConstantValueNode<double>
{
	public:
		NODE(DoubleConstantValueNode, "Double Value");
};

class StringConstantValueNodeRuntime : public ConstantValueNodeRuntime<std::string*>
{
	public:
		StringConstantValueNodeRuntime(const Node* node, NodeCall nodeCall) : ConstantValueNodeRuntime(node, nodeCall) {}
		
		void setValue(const std::string& value)
		{
			m_value = value;
			writePin<typename ConstantValueNode<std::string*>::ValueOutPin>(&m_value);
		}
		
	private:
		std::string m_value;
};

class StringConstantValueNode : public ConstantValueNode<std::string*>
{
	public:
		NODE(StringConstantValueNode, "String Value");
		NODE_RUNTIME_TYPE(StringConstantValueNodeRuntime);
};

} // node

#endif // CONSTANTVALUENODE_H



