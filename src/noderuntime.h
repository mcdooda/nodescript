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
		
		void optimizeLinks();
		
		inline int getNodeCall() const { return m_nodeCall; }
		
		inline void setCurrentInputPinIndex(int currentInputPinIndex) { m_currentInputPinIndex = currentInputPinIndex; }
		inline void clearCurrentInputPinIndex() { m_currentInputPinIndex = -1; }
		inline int getCurrentInputPinIndex() const { return m_currentInputPinIndex; }
		
		#ifndef NDEBUG
		bool debugIsInputValuePinIndexValid(int inputPinIndex) const;
		bool debugIsOutputValuePinIndexValid(int outputPinIndex) const;
		#endif
		
	private:
		void createInputValues(int numValues);
		void createOutputValues(int numValues);
		
		int getInputValueIndexFromPinIndex(int inputPinIndex) const;
		int getOutputValueIndexFromPinIndex(int outputPinIndex) const;
		
		void optimizeInputValueLinks();
		void optimizeOutputImpulseLinks();
		
		template <class T>
		void readInputPin(typename T::ValueType& value)
		{
			assert(debugIsInputValuePinIndexValid(T::Index));
			int index = getInputValueIndexFromPinIndex(T::Index);
			readPinValue<typename T::ValueType>(*m_inputValues[index], value);
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
		PinValue** m_inputValues;
		PinValue* m_outputValues;
		
		ScriptRuntime* m_scriptRuntime; // TODO get rid of this
}; // NodeRuntime

#endif // NODERUNTIME_H



