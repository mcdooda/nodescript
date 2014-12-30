#ifndef NODERUNTIME_H
#define NODERUNTIME_H

#include <map>
#include <type_traits>
#include "pin.h"
#include "scriptruntime.h"

class Node;

class NodeRuntime
{
	friend class Node;
	public:
		NodeRuntime(Node* node, ScriptRuntime* scriptRuntime, int nodeCall);
		virtual ~NodeRuntime();
		
		inline int getNodeCall() const { return m_nodeCall; }
		
		template <class T>
		typename T::PinValueType readOutputPin()
		{
			return dynamic_cast<typename T::PinTypeValue>(m_outputValues[T::PinIndex].pointerValue);
		}
		
		template <class T>
		int readOutputPin()
		{
			static_assert(std::is_same<typename T::ValueType, int>::value, "Pin expects an integer value");
			return m_outputValues[T::PinIndex].intValue;
		}
	
		template <class T>
		void writeOutputPin(void* value)
		{
			static_assert(std::is_pointer<typename T::ValueType>::value, "Pin expects a pointer value");
			m_outputValues[T::Index].pointerValue = value;
		}
		
		template <class T>
		void writeOutputPin(int value)
		{
			static_assert(std::is_same<typename T::ValueType, int>::value, "Pin expects an integer value");
			m_outputValues[T::Index].intValue = value;
		}
		
		template <class T>
		void impulse()
		{
			m_scriptRuntime->impulse(this, T::Index);
		}
		
		inline void setCurrentInputPinIndex(int currentInputPinIndex) { m_currentInputPinIndex = currentInputPinIndex; }
		inline void clearCurrentInputPinIndex() { m_currentInputPinIndex = -1; }
		inline int getCurrentInputPinIndex() const { return m_currentInputPinIndex; }
		
	private:
		void createOutputValues(int numValues);
		void clearOutputValue(int pinIndex);
		
		Node* m_node;
		int m_nodeCall;
		
		int m_currentInputPinIndex;
		PinValue* m_outputValues;
		
		ScriptRuntime* m_scriptRuntime;
}; // NodeRuntime

#endif // NODERUNTIME_H



