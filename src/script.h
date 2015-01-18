#ifndef SCRIPT_H
#define SCRIPT_H

#include <vector>
#include <map>
#include "pin.h"
#include "nodecall.h"

class ScriptRuntime;
class Node;

class Script
{
	public:
		Script();
		~Script();
		
		NodeCall addNode(Node* node);
		void addLink(NodeCall nodeCall1, PinIndex outputPinIndex, NodeCall nodeCall2, PinIndex inputPinIndex);
		
		ScriptRuntime* createRuntime();
		
		void addEntryPoint(NodeCall nodeCall);
		inline const std::vector<NodeCall>& getEntryPoints() const { return m_entryPoints; }
		
		Node* getNode(NodeCall nodeCall) const;
		int getNumNodes() const;
		inline const std::vector<Node*>& getNodes() const { return m_nodes; }
		
		void getInputPin(NodeCall nodeCall, PinIndex outputPinIndex, Pin& pin);
		void getOutputPin(NodeCall nodeCall, PinIndex inputPinIndex, Pin& pin);
		
		#ifndef NDEBUG
		bool debugIsNodeCallValid(NodeCall nodeCall) const;
		#endif
		
	private:
		std::vector<Node*> m_nodes;
		std::vector<NodeCall> m_entryPoints;
		std::map<NodeCall, std::map<PinIndex, Pin>> m_inputPins;
		std::map<NodeCall, std::map<PinIndex, Pin>> m_outputPins;
};

#endif // SCRIPT_H



