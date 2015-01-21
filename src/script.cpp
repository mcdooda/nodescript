#include <cassert>
#include "script.h"
#include "node.h"
#include "scriptruntime.h"

Script::Script()
{
	
}

Script::~Script()
{
	
}

NodeCall Script::addNode(Node* node)
{
	#ifndef NDEBUG
	node->debugGetNodeName(); // ensures a name is given to the actual node type
	#endif
	m_nodes.push_back(node);
	NodeCall nodeCall = m_nodes.size() - 1;
	m_outputPins.emplace(std::piecewise_construct, std::forward_as_tuple(nodeCall), std::forward_as_tuple());
	m_inputPins.emplace(std::piecewise_construct, std::forward_as_tuple(nodeCall), std::forward_as_tuple());
	node->addedToScript(this, nodeCall);
	return nodeCall;
}

void Script::addLink(NodeCall nodeCall1, PinIndex outputPinIndex, NodeCall nodeCall2, PinIndex inputPinIndex)
{
	assert(isLinkValid(nodeCall1, outputPinIndex, nodeCall2, inputPinIndex));
	m_outputPins[nodeCall1][outputPinIndex].emplace_back(nodeCall2, inputPinIndex);
	m_inputPins[nodeCall2].emplace(std::piecewise_construct, std::forward_as_tuple(inputPinIndex), std::forward_as_tuple(nodeCall1, outputPinIndex));
}

bool Script::isLinkValid(NodeCall nodeCall1, PinIndex outputPinIndex, NodeCall nodeCall2, PinIndex inputPinIndex)
{
	assert(debugIsNodeCallValid(nodeCall1));
	assert(debugIsNodeCallValid(nodeCall2));
	Node* node1 = getNode(nodeCall1);
	Node* node2 = getNode(nodeCall2);
	return node1->getPinTypeId(outputPinIndex) == node2->getPinTypeId(inputPinIndex);
}

ScriptRuntime* Script::createRuntime()
{
	return new ScriptRuntime(this);
}

void Script::addEntryPoint(NodeCall nodeCall)
{
	m_entryPoints.push_back(nodeCall);
}

Node* Script::getNode(NodeCall nodeCall) const
{
	assert(debugIsNodeCallValid(nodeCall));
	return m_nodes[nodeCall];
}

int Script::getNumNodes() const
{
	return m_nodes.size();
}

void Script::getInputPins(NodeCall nodeCall, PinIndex outputPinIndex, std::vector<Pin>*& pins)
{
	pins = &m_outputPins[nodeCall][outputPinIndex];
}

void Script::getOutputPin(NodeCall nodeCall, PinIndex inputPinIndex, Pin& pin)
{
	pin = m_inputPins[nodeCall][inputPinIndex];
}

#ifndef NDEBUG
bool Script::debugIsNodeCallValid(NodeCall nodeCall) const
{
	return nodeCall >= 0 && nodeCall < static_cast<int>(m_nodes.size());
}
#endif



