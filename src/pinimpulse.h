#ifndef PINIMPULSE_H
#define PINIMPULSE_H

#include "pin.h"

class NodeRuntime;

class PinImpulse
{
	public:
		PinImpulse(NodeRuntime* nodeRuntime, PinIndex inputPinIndex);
		PinImpulse();
		PinImpulse(const PinImpulse& pinImpulse);
		void operator=(const PinImpulse& pinImpulse);
		
		inline void setNodeRuntime(NodeRuntime* nodeRuntime) { m_nodeRuntime = nodeRuntime; }
		inline NodeRuntime* getNodeRuntime() const { return m_nodeRuntime; }
		
		inline void setInputPinIndex(PinIndex inputPinIndex) { m_inputPinIndex = inputPinIndex; }
		inline PinIndex getInputPinIndex() const { return m_inputPinIndex; }
		
		// returns whether the pin may be connected, can return true with invalid pins
		inline bool isConnected() const { return m_nodeRuntime != nullptr; }
		
	private:
		NodeRuntime* m_nodeRuntime;
		PinIndex m_inputPinIndex;
};

#endif // PINIMPULSE_H



