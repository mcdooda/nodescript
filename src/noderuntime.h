#ifndef NODERUNTIME_H
#define NODERUNTIME_H

#include <map>
#include <type_traits>
#include <cassert>
#include "pin.h"
#include "pinvalue.h"
#include "pinimpulse.h"

class Node;
class ScriptRuntime;

class NodeRuntime
{
	friend class Node;
	public:
		NodeRuntime(Node* node, int nodeCall);
		virtual ~NodeRuntime();
		
		void execute(int inputPinIndex);
		
		void optimizeLinks(ScriptRuntime* scriptRuntime);
		
		inline int getNodeCall() const { return m_nodeCall; }
		
		#ifndef NDEBUG
		bool debugIsInputValuePinIndexValid(int pinIndex) const;
		bool debugIsInputImpulsePinIndexValid(int pinIndex) const;
		bool debugIsOutputValuePinIndexValid(int pinIndex) const;
		bool debugIsOutputImpulsePinIndexValid(int pinIndex) const;
		#endif
		
	private:
		void createInputValues();
		void createOutputValues();
		void createOutputImpulses();
		
		int getInputValueIndexFromPinIndex(int pinIndex) const;
		int getOutputValueIndexFromPinIndex(int pinIndex) const;
		int getOutputImpulseIndexFromPinIndex(int pinIndex) const;
		
		void optimizeInputValueLinks(ScriptRuntime* scriptRuntime);
		void optimizeOutputImpulseLinks(ScriptRuntime* scriptRuntime);
		
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
			if (pinImpulse.isConnected())
			{
				NodeRuntime* outputRuntime = pinImpulse.getNodeRuntime();
				assert(outputRuntime->debugIsInputImpulsePinIndexValid(pinImpulse.getInputPinIndex()));
				outputRuntime->execute(pinImpulse.getInputPinIndex());
			}
		}
		
		Node* m_node;
		int m_nodeCall;
		
		PinValue** m_inputValues;
		PinValue* m_outputValues;
		PinImpulse* m_outputImpulses;
}; // NodeRuntime

#endif // NODERUNTIME_H



