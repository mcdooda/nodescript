#ifndef NODERUNTIME_H
#define NODERUNTIME_H

#include <map>
#include <type_traits>
#include <cassert>
#include "pin.h"
#include "pinvalue.h"
#include "pinimpulse.h"
#include "scriptruntime.h"

class Node;

class NodeRuntime
{
	friend class Node;
	public:
		NodeRuntime(Node* node, ScriptRuntime* scriptRuntime, int nodeCall);
		virtual ~NodeRuntime();
		
		void execute(int inputPinIndex);
		
		void optimizeLinks();
		
		inline int getNodeCall() const { return m_nodeCall; }
		
		#ifndef NDEBUG
		bool debugIsInputValuePinIndexValid(int pinIndex) const;
		bool debugIsInputImpulsePinIndexValid(int pinIndex) const;
		bool debugIsOutputValuePinIndexValid(int pinIndex) const;
		bool debugIsOutputImpulsePinIndexValid(int pinIndex) const;
		#endif
		
	private:
		void createInputValues(int numValues);
		void createOutputValues(int numValues);
		void createOutputImpulses(int numImpulses);
		
		int getInputValueIndexFromPinIndex(int pinIndex) const;
		int getOutputValueIndexFromPinIndex(int pinIndex) const;
		int getOutputImpulseIndexFromPinIndex(int pinIndex) const;
		
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
			assert(debugIsOutputImpulsePinIndexValid(T::Index));
			int index = getOutputImpulseIndexFromPinIndex(T::Index);
			const PinImpulse& pinImpulse = m_outputImpulses[index];
			NodeRuntime* outputRuntime = pinImpulse.getNodeRuntime();
			outputRuntime->execute(pinImpulse.getInputPinIndex());
		}
		
		Node* m_node;
		int m_nodeCall;
		
		PinValue** m_inputValues;
		PinValue* m_outputValues;
		PinImpulse* m_outputImpulses;
		
		ScriptRuntime* m_scriptRuntime; // TODO get rid of this
}; // NodeRuntime

#endif // NODERUNTIME_H



