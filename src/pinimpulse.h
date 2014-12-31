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
		
	private:
		NodeRuntime* m_nodeRuntime;
		int m_inputPinIndex;
};

#endif // PINIMPULSE_H



