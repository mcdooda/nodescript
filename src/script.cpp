#include <cassert>
#include <iostream>
#include "script.h"
#include "node.h"
#include "scriptruntime.h"

Script::Script()
{
	
}

Script::~Script()
{
	
}

int Script::addNode(Node* node)
{
	m_nodes.push_back(node);
	int nodeCall = m_nodes.size() - 1;
	m_outputPins[nodeCall] = std::map<int, Pin>();
	m_inputPins[nodeCall] = std::map<int, Pin>();
	node->addedToScript(this, nodeCall);
	return nodeCall;
}

void Script::addLink(int nodeCall1, int outputPinIndex, int nodeCall2, int inputPinIndex)
{
	assert(debugIsNodeCallValid(nodeCall1));
	assert(outputPinIndex >= 0);
	assert(debugIsNodeCallValid(nodeCall2));
	assert(inputPinIndex >= 0);
	std::cout << "added link from node #" << nodeCall1 << " pin #" << outputPinIndex << " to node #" << nodeCall2 << " pin #" << inputPinIndex << std::endl;
	m_outputPins[nodeCall1][outputPinIndex] = Pin(nodeCall2, inputPinIndex);
	m_inputPins[nodeCall2][inputPinIndex] = Pin(nodeCall1, outputPinIndex);
}

ScriptRuntime* Script::createRuntime()
{
	return new ScriptRuntime(this);
}

void Script::addEntryPoint(int nodeCall)
{
	m_entryPoints.push_back(nodeCall);
}

Node* Script::getNode(int nodeCall) const
{
	assert(nodeCall >= 0 && nodeCall < static_cast<int>(m_nodes.size()));
	return m_nodes[nodeCall];
}

int Script::getNumNodes() const
{
	return m_nodes.size();
}

void Script::getInputPin(int nodeCall, int outputPinIndex, Pin& pin)
{
	pin = m_outputPins[nodeCall][outputPinIndex];
}

void Script::getOutputPin(int nodeCall, int inputPinIndex, Pin& pin)
{
	pin = m_inputPins[nodeCall][inputPinIndex];
}



