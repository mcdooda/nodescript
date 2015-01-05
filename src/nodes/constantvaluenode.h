#ifndef CONSTANTVALUENODE_H
#define CONSTANTVALUENODE_H

#include "../functionalnode.h"
#include "../functionalnoderuntime.h"

namespace node
{

template <class T>
class ConstantValueNodeRuntime : public FunctionalNodeRuntime
{
	public:
		ConstantValueNodeRuntime(Node* node, int nodeCall) : FunctionalNodeRuntime(node, nodeCall) {}
		
		void setValue(const T& value);
};

template <class T>
class ConstantValueNode : public FunctionalNode
{
	public:
		NODE_FACTORY(new ConstantValueNode<T>());
		
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
		
		NodeRuntime* createRuntime(ScriptRuntime* scriptRuntime, int nodeCall) override
		{
			return new ConstantValueNodeRuntime<T>(this, nodeCall);
		}
};

template <class T>
void ConstantValueNodeRuntime<T>::setValue(const T& value)
{
	writePin<typename ConstantValueNode<T>::ValueOutPin>(value);
}

class PointerConstantValueNode : public ConstantValueNode<void*>
{
	public:
		NODE_NAME("Pointer Value");
};

class BoolConstantValueNode : public ConstantValueNode<bool>
{
	public:
		NODE_NAME("Bool Value");
};

class IntConstantValueNode : public ConstantValueNode<int>
{
	public:
		NODE_NAME("Int Value");
};

class LongConstantValueNode : public ConstantValueNode<long>
{
	public:
		NODE_NAME("Long Value");
};

class FloatConstantValueNode : public ConstantValueNode<float>
{
	public:
		NODE_NAME("Float Value");
};

class DoubleConstantValueNode : public ConstantValueNode<double>
{
	public:
		NODE_NAME("Double Value");
};

} // node

#endif // CONSTANTVALUENODE_H



