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
		
		virtual const char* getPinName(PinIndex pinIndex) const;
		
		virtual void execute(NodeRuntime* runtime, PinIndex inputPinIndex) const = 0;
		
		virtual void addedToScript(Script* script, int nodeCall) const {}
		
		virtual NodeRuntime* createRuntime(ScriptRuntime* scriptRuntime, int nodeCall);
		
		#ifndef NDEBUG
		bool debugIsInputValuePinIndexValid(PinIndex pinIndex) const;
		bool debugIsInputImpulsePinIndexValid(PinIndex pinIndex) const;
		bool debugIsOutputValuePinIndexValid(PinIndex pinIndex) const;
		bool debugIsOutputImpulsePinIndexValid(PinIndex pinIndex) const;
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
			static_assert(std::is_same<typename T::ValueType, void>::value, "Impulse pins cannot have a type");
			assert(m_currentPinIndex == T::Index);
			m_currentPinIndex++;
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
			static_assert(std::is_same<typename T::ValueType, void>::value, "Impulse pins cannot have a type");
			assert(m_currentPinIndex == T::Index);
			m_currentPinIndex++;
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
		
		PinIndex m_firstInValuePinIndex;
		PinIndex m_lastInValuePinIndex;
		
		PinIndex m_firstInImpulsePinIndex;
		PinIndex m_lastInImpulsePinIndex;
		
		PinIndex m_firstOutValuePinIndex;
		PinIndex m_lastOutValuePinIndex;
		
		PinIndex m_firstOutImpulsePinIndex;
		PinIndex m_lastOutImpulsePinIndex;
		
		#ifndef NDEBUG
		PinIndex m_currentPinIndex;
		#endif
		
}; // Node

typedef Node* (*NodeFactory)();

#endif // NODE_H



