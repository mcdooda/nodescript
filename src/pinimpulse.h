#ifndef PINIMPULSE_H
#define PINIMPULSE_H

class NodeRuntime;

class PinImpulse
{
	public:
		PinImpulse(NodeRuntime* nodeRuntime, int inputPinIndex);
		PinImpulse();
		PinImpulse(const PinImpulse& pinImpulse);
		void operator=(const PinImpulse& pinImpulse);
		
		inline NodeRuntime* getNodeRuntime() const { return m_nodeRuntime; }
		inline int getInputPinIndex() const { return m_inputPinIndex; }
		
		// returns whether the pin may be connected, can return true with invalid pins
		inline bool isConnected() const { return m_nodeRuntime != nullptr; }
		
	private:
		NodeRuntime* m_nodeRuntime;
		int m_inputPinIndex;
};

#endif // PINIMPULSE_H



