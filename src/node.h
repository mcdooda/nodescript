#ifndef NODE_H
#define NODE_H

#include <vector>
#include <string>
#include <cstring>
#include "pin.h"
#include "noderuntime.h"
#include "script.h"
#include "pinarchetype.h"

class NodeRuntime;
class ScriptRuntime;

#define NODE_FACTORY(NodeConstructor) \
	static Node* factory() { return NodeConstructor; } \

#if defined(NODESCRIPT_DEBUG) || defined(NODESCRIPT_INTROSPECTION)
#define NODE_NAME(NodeName) \
	static const char* nodeName() { return NodeName; } \
	const char* getNodeName() const override { return NodeName; }
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
		
		#if defined(NODESCRIPT_DEBUG) || defined(NODESCRIPT_INTROSPECTION)
		virtual const char* getNodeName() const;
		virtual const char* getPinName(PinIndex pinIndex) const;
		#endif
		PinTypeId getPinTypeId(PinIndex pinIndex) const;
		
		virtual void execute(NodeRuntime* runtime, PinIndex inputPinIndex) const = 0;
		
		virtual void addedToScript(Script* script, NodeCall nodeCall) const {}
		
		virtual NodeRuntime* createRuntime(ScriptRuntime* scriptRuntime, NodeCall nodeCall) const;
		
        #if defined(NODESCRIPT_DEBUG) || defined(NODESCRIPT_INTROSPECTION)
        bool isPinIndexValid(PinIndex pinIndex) const;
        bool isInputValuePinIndexValid(PinIndex pinIndex) const;
        bool isInputImpulsePinIndexValid(PinIndex pinIndex) const;
        bool isOutputValuePinIndexValid(PinIndex pinIndex) const;
        bool isOutputImpulsePinIndexValid(PinIndex pinIndex) const;
        #endif

        #ifdef NODESCRIPT_DEBUG
		const char* debugGetPinType(PinIndex pinIndex) const;
		void debugPrintPins() const;
		#endif
		
		#ifdef NODESCRIPT_INTROSPECTION
		PinArchetype getPinArchetype(PinIndex pinIndex) const;
        int getNumPins() const;
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
			
			PinValue defaultPinValue;
			memset(&defaultPinValue, 0, sizeof(PinValue));
			m_inputPinDefaultValues.push_back(defaultPinValue);
		}
		
		template <class T>
		void inValuePin(typename T::ValueType& defaultValue)
		{
			inValuePin<T>();
            NODESCRIPT_ASSERT(isInputValuePinIndexValid(T::Index));
			int index = getInputValueIndexFromPinIndex(T::Index);
			writePinValue(m_inputPinDefaultValues[index], defaultValue);
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
		
		int getInputValueIndexFromPinIndex(PinIndex pinIndex) const;
		
		std::vector<PinValue> m_inputPinDefaultValues;
		
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



