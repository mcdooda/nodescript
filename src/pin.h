#ifndef PIN_H
#define PIN_H

class Node;

template <int Index_, class ValueType_ = void>
class PinInfo
{
	public:
		enum { Index = Index_ };
		typedef ValueType_ ValueType;
};

union PinValue
{
	bool boolValue;
	int intValue;
	long longValue;
	float floatValue;
	double doubleValue;
	void* pointerValue;
};

class Pin
{
	public:
		Pin();
		Pin(int nodeCall, int index);
		Pin(const Pin& pin);
		
		void copy(const Pin& pin);
		
		inline int getNodeCall() const { return m_nodeCall; }
		inline int getIndex() const { return m_index; }
		
	private:
		int m_nodeCall;
		int m_index;
};

#endif // PIN_H



