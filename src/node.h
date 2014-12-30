#ifndef NODE_H
#define NODE_H

#include <vector>
#include <string>
#include "pin.h"
#include "noderuntime.h"
#include "script.h"

class NodeRuntime;
class ScriptRuntime;

#define NODE(NodeType, NodeName) \
	static Node* factory() { return new NodeType(); } \
	static const std::string getNodeName() { return NodeName; }

class Node
{
	friend class NodeRuntime;
	public:
		Node();
		virtual ~Node();
		
		virtual void optimize();
		
		virtual const char* getPinName(int pinIndex) const;
		
		virtual void execute(NodeRuntime* runtime) const = 0;
		
		virtual void addedToScript(Script* script, int nodeCall) const {}
		
		virtual NodeRuntime* createRuntime(ScriptRuntime* scriptRuntime, int nodeCall);
		
		inline bool debugIsOutputValuePinIndexValid(int outputPinIndex) const { return outputPinIndex >= 0 && outputPinIndex < getNumNodes(); }
		
	protected:
		template <class T>
		void inValuePin()
		{
			m_inputValuePins.push_back(T::Index);
		}
		
		template <class T>
		void inImpulsePin()
		{
			m_inputImpulsePins.push_back(T::Index);
		}
		
		template <class T>
		void outValuePin()
		{
			m_outputValuePins.push_back(T::Index);
		}
		
		template <class T>
		void outImpulsePin()
		{
			m_outputImpulsePins.push_back(T::Index);
		}
		
		template <class T>
		void readPin(NodeRuntime* runtime, typename T::ValueType& value) const
		{
			int nodeCall = runtime->getNodeCall();
			ScriptRuntime* scriptRuntime = runtime->m_scriptRuntime;
			Script* script = scriptRuntime->getScript();
			Pin outputPin;
			script->getOutputPin(nodeCall, T::Index, outputPin);
			assert(script->debugIsNodeCallValid(outputPin.getNodeCall())); // The input pin is not connected to an other pin!
			NodeRuntime* inputRuntime = scriptRuntime->getNodeCallRuntime(outputPin.getNodeCall());
			inputRuntime->readOutputPinAtIndex<typename T::ValueType>(outputPin.getIndex(), value);
		}
		
		template <class T>
		void writePin(NodeRuntime* runtime, typename T::ValueType value) const
		{
			runtime->writeOutputPin<T>(value);
		}
		
		template <class T>
		void impulse(NodeRuntime* runtime) const
		{
			runtime->impulse<T>();
		}
		
		void createOutputValues(NodeRuntime* runtime);
		void clearOutputValues(NodeRuntime* runtime);
		
		inline int getNumNodes() const { return m_inputValuePins.size() + m_inputImpulsePins.size() + m_outputValuePins.size() + m_outputImpulsePins.size(); }
		
		std::vector<int> m_inputValuePins;
		std::vector<int> m_inputImpulsePins;
		
		std::vector<int> m_outputValuePins;
		std::vector<int> m_outputImpulsePins;
}; // Node

typedef Node* (*NodeFactory)();

#endif // NODE_H



