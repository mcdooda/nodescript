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

#ifndef NDEBUG
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
		
		#ifndef NDEBUG
		virtual const char* debugGetNodeName() const;
		#endif
		virtual const char* getPinName(PinIndex pinIndex) const;
		
		virtual void execute(NodeRuntime* runtime, PinIndex inputPinIndex) const = 0;
		
		virtual void addedToScript(Script* script, NodeCall nodeCall) const {}
		
		virtual NodeRuntime* createRuntime(ScriptRuntime* scriptRuntime, NodeCall nodeCall) const;
		
		#ifndef NDEBUG
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
			#ifndef NDEBUG
			assert(m_debugCurrentPinIndex == T::Index);
			++m_debugCurrentPinIndex;
			static_assert(PIN_TYPE_VALID(typename T::ValueType), "This pin type is not registered!");
			m_debugPinTypeIds.push_back(PIN_TYPE_ID(typename T::ValueType));
			assert(m_firstInImpulsePinIndex == INVALID_PIN_INDEX_MIN_1);
			assert(m_firstOutValuePinIndex == INVALID_PIN_INDEX_MIN_1);
			assert(m_firstOutImpulsePinIndex == INVALID_PIN_INDEX_MIN_1);
			#endif
			if (m_firstInValuePinIndex == INVALID_PIN_INDEX_MIN_1)
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
			assert(!isFunctional());
			assert(m_debugCurrentPinIndex == T::Index);
			++m_debugCurrentPinIndex;
			m_debugPinTypeIds.push_back(PIN_TYPE_IMPULSE_ID);
			assert(m_firstOutValuePinIndex == INVALID_PIN_INDEX_MIN_1);
			assert(m_firstOutImpulsePinIndex == INVALID_PIN_INDEX_MIN_1);
			#endif
			if (m_firstInImpulsePinIndex == INVALID_PIN_INDEX_MIN_1)
			{
				m_firstInImpulsePinIndex = T::Index;
			}
			m_lastInImpulsePinIndex = T::Index;
		}
		
		template <class T>
		void outValuePin()
		{
			#ifndef NDEBUG
			assert(m_debugCurrentPinIndex == T::Index);
			++m_debugCurrentPinIndex;
			static_assert(PIN_TYPE_VALID(typename T::ValueType), "This pin type is not registered!");
			m_debugPinTypeIds.push_back(PIN_TYPE_ID(typename T::ValueType));
			assert(m_firstOutImpulsePinIndex == INVALID_PIN_INDEX_MIN_1);
			#endif
			if (m_firstOutValuePinIndex == INVALID_PIN_INDEX_MIN_1)
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
			assert(m_debugCurrentPinIndex == T::Index);
			++m_debugCurrentPinIndex;
			m_debugPinTypeIds.push_back(PIN_TYPE_IMPULSE_ID);
			#endif
			if (m_firstOutImpulsePinIndex == INVALID_PIN_INDEX_MIN_1)
			{
				m_firstOutImpulsePinIndex = T::Index;
			}
			m_lastOutImpulsePinIndex = T::Index;
		}
		
		template <class T>
		void readPin(NodeRuntime* runtime, typename T::ValueType& value) const
		{
			assert(m_debugPinTypeIds[T::Index] == PIN_TYPE_ID(typename T::ValueType));
			runtime->readPin<T>(value);
		}
		
		template <class T>
		void writePin(NodeRuntime* runtime, typename T::ValueType value) const
		{
			assert(m_debugPinTypeIds[T::Index] == PIN_TYPE_ID(typename T::ValueType));
			runtime->writePin<T>(value);
		}
		
		template <class T>
		void impulse(NodeRuntime* runtime) const
		{
			assert(m_debugPinTypeIds[T::Index] == PIN_TYPE_IMPULSE_ID);
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
		PinIndex m_debugCurrentPinIndex;
		std::vector<PinTypeId> m_debugPinTypeIds;
		#endif
		
}; // Node

typedef Node* (*NodeFactory)();

#endif // NODE_H



