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
		
		NodeCall addNode(const Node* node);
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
		
		const Node* getNode(NodeCall nodeCall) const;
		inline int getNumNodes() const { return m_nodes.size(); }
		inline const std::vector<const Node*>& getNodes() const { return m_nodes; }
		
		const std::vector<Pin>* getInputPins(NodeCall nodeCall, PinIndex outputPinIndex) const;
		const Pin& getOutputPin(NodeCall nodeCall, PinIndex inputPinIndex) const;
		
		#ifdef NODESCRIPT_DEBUG
		bool debugIsNodeCallValid(NodeCall nodeCall) const;
		#endif
		
	private:
		// maps a node instance to a node
		std::vector<const Node*> m_nodes;
		
		// the scripts' entry points (init, maybe others...)
		std::vector<NodeCall> m_entryPoints;
		
		// maps a (node instance, pin index) to a pin
		std::map<NodeCall, std::map<PinIndex, Pin>> m_inputPins;
		
		// maps a (node instance instance, pin index) to a set of connected pins
		std::map<NodeCall, std::map<PinIndex, std::vector<Pin>>> m_outputPins;
};

#endif // SCRIPT_H



