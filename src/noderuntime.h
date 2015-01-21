#ifndef NODERUNTIME_H
#define NODERUNTIME_H

#include <vector>
#include <map>
#include <type_traits>
#include <iostream>
#include "pin.h"
#include "pinvalue.h"
#include "pinimpulse.h"
#include "debug.h"

class Node;
class ScriptRuntime;

class NodeRuntime
{
	friend class Node;
	public:
		NodeRuntime(const Node* node, NodeCall nodeCall);
		virtual ~NodeRuntime();
		
		virtual void execute(PinIndex inputPinIndex);
		
		void optimizeLinks(ScriptRuntime* scriptRuntime);
		
		inline NodeCall getNodeCall() const { return m_nodeCall; }
		
		#ifdef NODESCRIPT_DEBUG
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
		
		template <class T>
		void readPin(typename T::ValueType& value)
		{
			NODESCRIPT_ASSERT(m_inputValues);
			NODESCRIPT_ASSERT(debugIsInputValuePinIndexValid(T::Index));
			
			int index = getInputValueIndexFromPinIndex(T::Index);
			if (m_inputRuntimes)
			{
				NodeRuntime* inputRuntime = m_inputRuntimes[index];
				if (inputRuntime)
				{
					inputRuntime->execute(FUNCTIONAL_AUTO_EXECUTE_PIN_INDEX);
				}
			}
			readPinValue<typename T::ValueType>(*m_inputValues[index], value);
		}
	
		template <class T>
		void writePin(typename T::ValueType value)
		{
			NODESCRIPT_ASSERT(m_outputValues);
			NODESCRIPT_ASSERT_MSG(debugIsOutputValuePinIndexValid(T::Index), "Trying to write to a pin of type \"%s\"", debugGetPinType(T::Index));
			int index = getOutputValueIndexFromPinIndex(T::Index);
			writePinValue<typename T::ValueType>(m_outputValues[index], value);
		}
		
		template <class T>
		void impulse()
		{
			NODESCRIPT_ASSERT(m_outputImpulses);
			NODESCRIPT_ASSERT(debugIsOutputImpulsePinIndexValid(T::Index));
			
			int index = getOutputImpulseIndexFromPinIndex(T::Index);
			const std::vector<PinImpulse>& pinImpulses = m_outputImpulses[index];
			for (const PinImpulse& pinImpulse : pinImpulses)
			{
				NODESCRIPT_ASSERT(pinImpulse.isConnected());
				NodeRuntime* outputRuntime = pinImpulse.getNodeRuntime();
				NODESCRIPT_ASSERT(outputRuntime->debugIsInputImpulsePinIndexValid(pinImpulse.getInputPinIndex()));
				outputRuntime->execute(pinImpulse.getInputPinIndex());
			}
		}
		
		template <class T>
		void delayedImpulse()
		{
			NODESCRIPT_BREAK(); // does not work yet... (some delays may not be intentional)
			++(*m_currentExecutionIndex);
			impulse<T>();
		}
		
		NodeRuntime** m_inputRuntimes;
		PinValue** m_inputValues;
		PinValue* m_outputValues;
		std::vector<PinImpulse>* m_outputImpulses;
		
		int* m_currentExecutionIndex;
		
		const Node* m_node;
		NodeCall m_nodeCall;
}; // NodeRuntime

#endif // NODERUNTIME_H



