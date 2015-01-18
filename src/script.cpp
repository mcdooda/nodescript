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
	node->debugGetNodeName();
	#endif
	m_nodes.push_back(node);
	NodeCall nodeCall = m_nodes.size() - 1;
	m_outputPins[nodeCall] = std::map<PinIndex, std::vector<Pin>>();
	m_inputPins[nodeCall] = std::map<PinIndex, Pin>();
	node->addedToScript(this, nodeCall);
	return nodeCall;
}

void Script::addLink(NodeCall nodeCall1, PinIndex outputPinIndex, NodeCall nodeCall2, PinIndex inputPinIndex)
{
	assert(debugIsNodeCallValid(nodeCall1));
	assert(outputPinIndex >= 0);
	assert(debugIsNodeCallValid(nodeCall2));
	assert(inputPinIndex >= 0);
	m_outputPins[nodeCall1][outputPinIndex].push_back(Pin(nodeCall2, inputPinIndex));
	m_inputPins[nodeCall2][inputPinIndex] = Pin(nodeCall1, outputPinIndex);
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

void Script::getInputPins(NodeCall nodeCall, PinIndex outputPinIndex, std::vector<Pin>& pins)
{
	pins = m_outputPins[nodeCall][outputPinIndex];
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



