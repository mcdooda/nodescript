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
		
		virtual const char* getPinName(int pinIndex) const;
		
		virtual void execute(NodeRuntime* runtime, int inputPinIndex) const = 0;
		
		virtual void addedToScript(Script* script, int nodeCall) const {}
		
		virtual NodeRuntime* createRuntime(ScriptRuntime* scriptRuntime, int nodeCall);
		
		#ifndef NDEBUG
		bool debugIsInputValuePinIndexValid(int pinIndex) const;
		bool debugIsInputImpulsePinIndexValid(int pinIndex) const;
		bool debugIsOutputValuePinIndexValid(int pinIndex) const;
		bool debugIsOutputImpulsePinIndexValid(int pinIndex) const;
		#endif
		
	protected:
		template <class T>
		void inValuePin()
		{
			#ifndef NDEBUG
			assert(m_currentPinIndex == T::Index);
			m_currentPinIndex++;
			assert(m_firstInImpulsePinIndex == -1);
			assert(m_firstOutValuePinIndex == -1);
			assert(m_firstOutImpulsePinIndex == -1);
			#endif
			if (m_firstInValuePinIndex == -1)
			{
				m_firstInValuePinIndex = T::Index;
			}
			m_lastInValuePinIndex = T::Index;
		}
		
		template <class T>
		void inImpulsePin()
		{
			#ifndef NDEBUG
			assert(m_currentPinIndex == T::Index);
			m_currentPinIndex++;
			static_assert(std::is_same<typename T::ValueType, void>::value, "Impulse pins cannot have a type");
			assert(m_firstOutValuePinIndex == -1);
			assert(m_firstOutImpulsePinIndex == -1);
			#endif
			if (m_firstInImpulsePinIndex == -1)
			{
				m_firstInImpulsePinIndex = T::Index;
			}
			m_lastInImpulsePinIndex = T::Index;
		}
		
		template <class T>
		void outValuePin()
		{
			#ifndef NDEBUG
			assert(m_currentPinIndex == T::Index);
			m_currentPinIndex++;
			assert(m_firstOutImpulsePinIndex == -1);
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
			if (m_firstOutImpulsePinIndex == -1)
			{
				m_firstOutImpulsePinIndex = T::Index;
			}
			m_lastOutImpulsePinIndex = T::Index;
		}
		
		template <class T>
		void readPin(NodeRuntime* runtime, typename T::ValueType& value) const
		{
			runtime->readInputPin<T>(value);
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
		
		void createInputValues(NodeRuntime* runtime);
		void createOutputValues(NodeRuntime* runtime);
		void createOutputImpulses(NodeRuntime* runtime);
		
		int m_firstInValuePinIndex;
		int m_lastInValuePinIndex;
		
		int m_firstInImpulsePinIndex;
		int m_lastInImpulsePinIndex;
		
		int m_firstOutValuePinIndex;
		int m_lastOutValuePinIndex;
		
		int m_firstOutImpulsePinIndex;
		int m_lastOutImpulsePinIndex;
		
		#ifndef NDEBUG
		int m_currentPinIndex;
		#endif
		
}; // Node

typedef Node* (*NodeFactory)();

#endif // NODE_H



