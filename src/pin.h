#ifndef PIN_H
#define PIN_H

template <int Index_, class ValueType_ = void>
class PinInfo
{
	public:
		enum { Index = Index_ };
		typedef ValueType_ ValueType;
};

class Pin
{
	public:
		Pin(int nodeCall, int index);
		Pin();
		Pin(const Pin& pin);
		void operator=(const Pin& pin);
		
		inline int getNodeCall() const { return m_nodeCall; }
		inline int getIndex() const { return m_index; }
		
		// returns whether the pin may be connected, can return true with invalid pins
		inline bool isConnected() const { return m_nodeCall != -1 || m_index != -1; }
		
	private:
		int m_nodeCall;
		int m_index;
};

#endif // PIN_H



