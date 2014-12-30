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
		
		#ifndef NDEBUG
		inline bool debugIsOutputValuePinIndexValid(int outputPinIndex) const { return outputPinIndex >= m_firstOutValuePinIndex && outputPinIndex <= m_firstOutValuePinIndex; }
		#endif
		
	protected:
		template <class T>
		void inValuePin()
		{
			#ifndef NDEBUG
			assert(m_currentPinIndex == T::Index);
			m_currentPinIndex++;
			#endif
		}
		
		template <class T>
		void inImpulsePin()
		{
			#ifndef NDEBUG
			assert(m_currentPinIndex == T::Index);
			m_currentPinIndex++;
			static_assert(std::is_same<typename T::ValueType, void>::value, "Impulse pins cannot have a type");
			#endif
		}
		
		template <class T>
		void outValuePin()
		{
			#ifndef NDEBUG
			assert(m_currentPinIndex == T::Index);
			m_currentPinIndex++;
			#endif
			if (m_firstOutValuePinIndex == -1)
			{
				m_firstOutValuePinIndex = T::Index;
			}
			m_lastOutValuePinIndex = T::Index;
		}
		
		template <class T>
		void outImpulsePin()
		{
			#ifndef NDEBUG
			assert(m_currentPinIndex == T::Index);
			m_currentPinIndex++;
			static_assert(std::is_same<typename T::ValueType, void>::value, "Impulse pins cannot have a type");
			#endif
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
		
		int m_firstOutValuePinIndex;
		int m_lastOutValuePinIndex;
		
		#ifndef NDEBUG
		int m_currentPinIndex;
		#endif
		
}; // Node

typedef Node* (*NodeFactory)();

#endif // NODE_H



