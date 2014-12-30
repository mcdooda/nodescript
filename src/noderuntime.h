#ifndef NODERUNTIME_H
#define NODERUNTIME_H

#include <map>
#include <type_traits>
#include <cassert>
#include "pin.h"
#include "pinvalue.h"
#include "scriptruntime.h"

class Node;

class NodeRuntime
{
	friend class Node;
	public:
		NodeRuntime(Node* node, ScriptRuntime* scriptRuntime, int nodeCall);
		virtual ~NodeRuntime();
		
		inline int getNodeCall() const { return m_nodeCall; }
		
		inline void setCurrentInputPinIndex(int currentInputPinIndex) { m_currentInputPinIndex = currentInputPinIndex; }
		inline void clearCurrentInputPinIndex() { m_currentInputPinIndex = -1; }
		inline int getCurrentInputPinIndex() const { return m_currentInputPinIndex; }
		
		#ifndef NDEBUG
		bool debugIsOutputValuePinIndexValid(int outputPinIndex) const;
		#endif
		
	private:
		void createOutputValues(int numValues);
		void clearOutputValue(int pinIndex);
		
		int getOutputValueIndexFromPinIndex(int outputPinIndex) const;
		
		template <class T>
		void readOutputPinAtIndex(int outputPinIndex, T& value)
		{
			assert(debugIsOutputValuePinIndexValid(outputPinIndex));
			int index = getOutputValueIndexFromPinIndex(outputPinIndex);
			readPinValue<T>(m_outputValues[index], value);
		}
		
		template <class T>
		void readOutputPin(typename T::ValueType& value)
		{
			assert(debugIsOutputValuePinIndexValid(T::Index));
			int index = getOutputValueIndexFromPinIndex(T::Index);
			readPinValue<typename T::ValueType>(m_outputValues[index], value);
		}
	
		template <class T>
		void writeOutputPin(typename T::ValueType value)
		{
			assert(debugIsOutputValuePinIndexValid(T::Index));
			int index = getOutputValueIndexFromPinIndex(T::Index);
			writePinValue<typename T::ValueType>(m_outputValues[index], value);
		}
		
		template <class T>
		void impulse()
		{
			m_scriptRuntime->impulse(this, T::Index);
		}
		
		Node* m_node;
		int m_nodeCall;
		
		int m_currentInputPinIndex;
		PinValue* m_outputValues;
		
		ScriptRuntime* m_scriptRuntime;
}; // NodeRuntime

#endif // NODERUNTIME_H



