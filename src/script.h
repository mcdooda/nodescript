#ifndef SCRIPT_H
#define SCRIPT_H

#include <vector>
#include <map>

#include "pin.h"

class ScriptRuntime;

class Script
{
	public:
		Script();
		~Script();
		
		int addNode(Node* node);
		void addLink(int nodeCall1, int outputPinIndex, int nodeCall2, int inputPinIndex);
		
		ScriptRuntime* createRuntime();
		
		void addEntryPoint(int nodeCall);
		inline const std::vector<int>& getEntryPoints() const { return m_entryPoints; }
		
		Node* getNode(int nodeCall) const;
		int getNumNodes() const;
		
		void getInputPin(int nodeCall, int outputPinIndex, Pin& pin);
		void getOutputPin(int nodeCall, int inputPinIndex, Pin& pin);
		
	private:
		std::vector<Node*> m_nodes;
		std::vector<int> m_entryPoints;
		std::map<int, std::map<int, Pin>> m_inputPins;
		std::map<int, std::map<int, Pin>> m_outputPins;
};

#endif // SCRIPT_H



