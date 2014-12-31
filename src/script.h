#ifndef SCRIPT_H
#define SCRIPT_H

#include <vector>
#include <map>
#include "pin.h"

class ScriptRuntime;
class Node;

class Script
{
	public:
		Script();
		~Script();
		
		int addNode(Node* node);
		void addLink(int nodeCall1, PinIndex outputPinIndex, int nodeCall2, PinIndex inputPinIndex);
		
		ScriptRuntime* createRuntime();
		
		void addEntryPoint(int nodeCall);
		inline const std::vector<int>& getEntryPoints() const { return m_entryPoints; }
		
		Node* getNode(int nodeCall) const;
		int getNumNodes() const;
		inline const std::vector<Node*>& getNodes() const { return m_nodes; }
		
		void getInputPin(int nodeCall, PinIndex outputPinIndex, Pin& pin);
		void getOutputPin(int nodeCall, PinIndex inputPinIndex, Pin& pin);
		
		#ifndef NDEBUG
		bool debugIsNodeCallValid(int nodeCall) const;
		#endif
		
	private:
		std::vector<Node*> m_nodes;
		std::vector<int> m_entryPoints;
		std::map<int, std::map<PinIndex, Pin>> m_inputPins;
		std::map<int, std::map<PinIndex, Pin>> m_outputPins;
};

#endif // SCRIPT_H



