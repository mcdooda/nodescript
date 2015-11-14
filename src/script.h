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
		bool isLinkValid(NodeCall nodeCall1, PinIndex outputPinIndex, NodeCall nodeCall2, PinIndex inputPinIndex);
		
		#if defined(NODESCRIPT_DEBUG) || defined(NODESCRIPT_INTROSPECTION)
		bool linkExists(NodeCall nodeCall1, PinIndex outputPinIndex, NodeCall nodeCall2, PinIndex inputPinIndex);
		#endif

		#ifdef NODESCRIPT_INTROSPECTION
		void removeNode(NodeCall nodeCall);
		void removeLink(NodeCall nodeCall1, PinIndex outputPinIndex, NodeCall nodeCall2, PinIndex inputPinIndex);
		#endif
		
		void optimize();
		
		ScriptRuntime* createRuntime();
		
		void addEntryPoint(NodeCall nodeCall);
		inline const std::vector<NodeCall>& getEntryPoints() const { return m_entryPoints; }
		
		Node* getNode(NodeCall nodeCall) const;
		int getNumNodes() const;
		inline const std::vector<Node*>& getNodes() const { return m_nodes; }
		
		void getInputPins(NodeCall nodeCall, PinIndex outputPinIndex, std::vector<Pin>*& pins);
		void getOutputPin(NodeCall nodeCall, PinIndex inputPinIndex, Pin& pin);
		
		#ifdef NODESCRIPT_DEBUG
		bool debugIsNodeCallValid(NodeCall nodeCall) const;
		#endif
		
	private:
		std::vector<Node*> m_nodes;
		std::vector<NodeCall> m_entryPoints;
		std::map<NodeCall, std::map<PinIndex, Pin>> m_inputPins;
		std::map<NodeCall, std::map<PinIndex, std::vector<Pin>>> m_outputPins;
};

#endif // SCRIPT_H



