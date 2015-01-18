#ifndef PIN_H
#define PIN_H

#include "pinvalue.h"
#include "nodecall.h"

typedef char PinIndex;

enum MagicPinIndex : char
{
	INVALID_PIN_INDEX_MIN_1           = -127,
	INVALID_PIN_INDEX_MIN_2           = -128,
	INVALID_PIN_INDEX                 = -1,
	ENTRY_POINT_PIN_INDEX             = -2,
	FUNCTIONAL_AUTO_EXECUTE_PIN_INDEX = -3
};

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
		Pin(NodeCall nodeCall, PinIndex index);
		Pin();
		Pin(const Pin& pin);
		void operator=(const Pin& pin);
		
		inline NodeCall getNodeCall() const { return m_nodeCall; }
		inline PinIndex getIndex() const { return m_index; }
		
		// returns whether the pin may be connected, can return true with invalid pins
		inline bool isConnected() const { return m_nodeCall != INVALID_NODE_CALL || m_index != INVALID_PIN_INDEX; }
		
	private:
		NodeCall m_nodeCall;
		PinIndex m_index;
};

#endif // PIN_H



