#ifndef NODE_H
#define NODE_H

#include <vector>
#include <string>
#include "pin.h"
#include "noderuntime.h"
#include "script.h"

class NodeRuntime;
class ScriptRuntime;

#define NODE_FACTORY(NodeConstructor) \
	static Node* factory() { return NodeConstructor; } \

#ifdef NODESCRIPT_DEBUG
#define NODE_NAME(NodeName) \
	static const char* nodeName() { return NodeName; } \
	const char* debugGetNodeName() const override { return NodeName; }
#else
#define NODE_NAME(NodeName) \
	static const char* nodeName() { return NodeName; }
#endif

#define NODE_RUNTIME_TYPE(NodeRuntimeType) \
	NodeRuntime* createRuntime(ScriptRuntime* scriptRuntime, NodeCall nodeCall) const override { return new NodeRuntimeType(this, nodeCall); }

#define NODE(NodeType, NodeName) \
	NODE_FACTORY(new NodeType()) \
	NODE_NAME(NodeName)

class Node
{
	friend class NodeRuntime;
	public:
		Node();
		virtual ~Node();
		
		virtual bool isFunctional() const { return false; }
		
		void optimize();
		
		#ifdef NODESCRIPT_DEBUG
		virtual const char* debugGetNodeName() const;
		#endif
		virtual const char* getPinName(PinIndex pinIndex) const;
		PinTypeId getPinTypeId(PinIndex pinIndex) const;
		
		virtual void execute(NodeRuntime* runtime, PinIndex inputPinIndex) const = 0;
		
		virtual void addedToScript(Script* script, NodeCall nodeCall) const {}
		
		virtual NodeRuntime* createRuntime(ScriptRuntime* scriptRuntime, NodeCall nodeCall) const;
		
		#ifdef NODESCRIPT_DEBUG
		bool debugIsPinIndexValid(PinIndex pinIndex) const;
		bool debugIsInputValuePinIndexValid(PinIndex pinIndex) const;
		bool debugIsInputImpulsePinIndexValid(PinIndex pinIndex) const;
		bool debugIsOutputValuePinIndexValid(PinIndex pinIndex) const;
		bool debugIsOutputImpulsePinIndexValid(PinIndex pinIndex) const;
		const char* debugGetPinType(PinIndex pinIndex) const;
		void debugPrintPins() const;
		#endif
		
	protected:
		template <class T>
		void inValuePin()
		{
			static_assert(PIN_TYPE_VALID(typename T::ValueType), "This pin type is not registered!");
			NODESCRIPT_ASSERT(m_pinTypeIds.size() == T::Index);
			NODESCRIPT_ASSERT(m_firstInImpulsePinIndex == INVALID_PIN_INDEX_MIN_1);
			NODESCRIPT_ASSERT(m_firstOutValuePinIndex == INVALID_PIN_INDEX_MIN_1);
			NODESCRIPT_ASSERT(m_firstOutImpulsePinIndex == INVALID_PIN_INDEX_MIN_1);
			
			m_pinTypeIds.push_back(PIN_TYPE_ID(typename T::ValueType));
			if (m_firstInValuePinIndex == INVALID_PIN_INDEX_MIN_1)
			{
				m_firstInValuePinIndex = T::Index;
			}
			m_lastInValuePinIndex = T::Index;
		}
		
		template <class T>
		void inImpulsePin()
		{
			static_assert(std::is_same<typename T::ValueType, void>::value, "Impulse pins cannot have a type");
			NODESCRIPT_ASSERT(!isFunctional());
			NODESCRIPT_ASSERT(m_pinTypeIds.size() == T::Index);
			NODESCRIPT_ASSERT(m_firstOutValuePinIndex == INVALID_PIN_INDEX_MIN_1);
			NODESCRIPT_ASSERT(m_firstOutImpulsePinIndex == INVALID_PIN_INDEX_MIN_1);
			
			m_pinTypeIds.push_back(PIN_TYPE_IMPULSE_ID);
			if (m_firstInImpulsePinIndex == INVALID_PIN_INDEX_MIN_1)
			{
				m_firstInImpulsePinIndex = T::Index;
			}
			m_lastInImpulsePinIndex = T::Index;
		}
		
		template <class T>
		void outValuePin()
		{
			static_assert(PIN_TYPE_VALID(typename T::ValueType), "This pin type is not registered!");
			NODESCRIPT_ASSERT(m_pinTypeIds.size() == T::Index);
			NODESCRIPT_ASSERT(m_firstOutImpulsePinIndex == INVALID_PIN_INDEX_MIN_1);
			
			m_pinTypeIds.push_back(PIN_TYPE_ID(typename T::ValueType));
			if (m_firstOutValuePinIndex == INVALID_PIN_INDEX_MIN_1)
			{
				m_firstOutValuePinIndex = T::Index;
			}
			m_lastOutValuePinIndex = T::Index;
		}
		
		template <class T>
		void outImpulsePin()
		{
			static_assert(std::is_same<typename T::ValueType, void>::value, "Impulse pins cannot have a type");
			NODESCRIPT_ASSERT(m_pinTypeIds.size() == T::Index);
			
			m_pinTypeIds.push_back(PIN_TYPE_IMPULSE_ID);
			if (m_firstOutImpulsePinIndex == INVALID_PIN_INDEX_MIN_1)
			{
				m_firstOutImpulsePinIndex = T::Index;
			}
			m_lastOutImpulsePinIndex = T::Index;
		}
		
		template <class T>
		void readPin(NodeRuntime* runtime, typename T::ValueType& value) const
		{
			NODESCRIPT_ASSERT(m_pinTypeIds[T::Index] == PIN_TYPE_ID(typename T::ValueType));
			runtime->readPin<T>(value);
		}
		
		template <class T>
		void writePin(NodeRuntime* runtime, typename T::ValueType value) const
		{
			NODESCRIPT_ASSERT(m_pinTypeIds[T::Index] == PIN_TYPE_ID(typename T::ValueType));
			runtime->writePin<T>(value);
		}
		
		template <class T>
		void impulse(NodeRuntime* runtime) const
		{
			NODESCRIPT_ASSERT(m_pinTypeIds[T::Index] == PIN_TYPE_IMPULSE_ID);
			runtime->impulse<T>();
		}
		
		std::vector<PinTypeId> m_pinTypeIds;
		
		PinIndex m_firstInValuePinIndex;
		PinIndex m_lastInValuePinIndex;
		
		PinIndex m_firstInImpulsePinIndex;
		PinIndex m_lastInImpulsePinIndex;
		
		PinIndex m_firstOutValuePinIndex;
		PinIndex m_lastOutValuePinIndex;
		
		PinIndex m_firstOutImpulsePinIndex;
		PinIndex m_lastOutImpulsePinIndex;
		
}; // Node

typedef Node* (*NodeFactory)();

#endif // NODE_H



