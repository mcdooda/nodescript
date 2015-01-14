#ifndef NODERUNTIME_H
#define NODERUNTIME_H

#include <map>
#include <type_traits>
#include <cassert>
#include <iostream>
#include "pin.h"
#include "pinvalue.h"
#include "pinimpulse.h"

class Node;
class ScriptRuntime;

class NodeRuntime
{
	friend class Node;
	public:
		NodeRuntime(const Node* node, int nodeCall);
		virtual ~NodeRuntime();
		
		virtual void execute(PinIndex inputPinIndex);
		
		void optimizeLinks(ScriptRuntime* scriptRuntime);
		
		inline int getNodeCall() const { return m_nodeCall; }
		
		#ifndef NDEBUG
		bool debugIsInputValuePinIndexValid(PinIndex pinIndex) const;
		bool debugIsInputImpulsePinIndexValid(PinIndex pinIndex) const;
		bool debugIsOutputValuePinIndexValid(PinIndex pinIndex) const;
		bool debugIsOutputImpulsePinIndexValid(PinIndex pinIndex) const;
		const char* debugGetPinType(PinIndex pinIndex) const;
		#endif
		
	protected:
		void createInputValues();
		void createOutputValues();
		void createOutputImpulses();
		
		int getInputValueIndexFromPinIndex(PinIndex pinIndex) const;
		int getOutputValueIndexFromPinIndex(PinIndex pinIndex) const;
		int getOutputImpulseIndexFromPinIndex(PinIndex pinIndex) const;
		
		void optimizeInputValueLinks(ScriptRuntime* scriptRuntime);
		void optimizeOutputImpulseLinks(ScriptRuntime* scriptRuntime);
		
		void prepareInputRuntimeReading(PinIndex pinIndex) const;
		
		template <class T>
		void readPin(typename T::ValueType& value)
		{
			assert(debugIsInputValuePinIndexValid(T::Index));
			int index = getInputValueIndexFromPinIndex(T::Index);
			NodeRuntime* inputRuntime = m_inputRuntimes[index];
			if (inputRuntime)
			{
				inputRuntime->execute(FUNCTIONAL_AUTO_EXECUTE_PIN_INDEX);
			}
			readPinValue<typename T::ValueType>(*m_inputValues[index], value);
		}
	
		template <class T>
		void writePin(typename T::ValueType value)
		{
			#ifndef NDEBUG
			if (!debugIsOutputValuePinIndexValid(T::Index))
			{
				std::cerr << "Trying to write to a pin of type \"" << debugGetPinType(T::Index) << "\"" << std::endl;
				assert(false);
			}
			#endif
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
		
		template <class T>
		void delayedImpulse()
		{
			++(*m_currentExecutionIndex);
			impulse<T>();
		}
		
		const Node* m_node;
		int m_nodeCall;
		
		NodeRuntime** m_inputRuntimes;
		PinValue** m_inputValues;
		PinValue* m_outputValues;
		PinImpulse* m_outputImpulses;
		
		int* m_currentExecutionIndex;
}; // NodeRuntime

#endif // NODERUNTIME_H



