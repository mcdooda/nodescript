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

#define CONSTANT_VALUE_NODE(NodeType, Type, NodeName) \
	class NodeType : public ConstantValueNode<Type> \
	{ \
		public: \
			NODE(NodeType, NodeName); \
	};

template <class T>
class PointerConstantValueNodeRuntime : public ConstantValueNodeRuntime<T*>
{
	public:
		PointerConstantValueNodeRuntime(const Node* node, NodeCall nodeCall) : ConstantValueNodeRuntime<T*>(node, nodeCall) {}
		
		void setValue(const T& value)
		{
			m_value = value;
			ConstantValueNodeRuntime<T*>::setValue(&m_value);
		}
		
	private:
		T m_value;
};

#define POINTER_CONSTANT_VALUE_NODE(NodeType, Type, NodeName) \
	class NodeType##Runtime : public PointerConstantValueNodeRuntime<Type> \
	{ \
		public: \
			NodeType##Runtime(const Node* node, NodeCall nodeCall) : PointerConstantValueNodeRuntime<Type>(node, nodeCall) {} \
	}; \
	class NodeType : public ConstantValueNode<Type*> \
	{ \
		public: \
			NODE(NodeType, NodeName); \
			NODE_RUNTIME_TYPE(NodeType##Runtime); \
	};

// native pin types
CONSTANT_VALUE_NODE(BoolConstantValueNode, bool, "Bool Value");
CONSTANT_VALUE_NODE(IntConstantValueNode, int, "Int Value");
CONSTANT_VALUE_NODE(LongConstantValueNode, long, "Long Value");
CONSTANT_VALUE_NODE(FloatConstantValueNode, float, "Float Value");
CONSTANT_VALUE_NODE(DoubleConstantValueNode, double, "Double Value");

// pointer pin types
POINTER_CONSTANT_VALUE_NODE(StringConstantValueNode, std::string, "String Value");

} // node

#endif // CONSTANTVALUENODE_H



