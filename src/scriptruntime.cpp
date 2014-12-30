#include <cstring>
#include <cassert>
#include "scriptruntime.h"
#include "script.h"
#include "node.h"
#include "noderuntime.h"

ScriptRuntime::ScriptRuntime(Script* script) :
	m_script(script)
{
	int numNodes = script->getNumNodes();
	m_nodeRuntimes = new NodeRuntime*[numNodes];
	memset(m_nodeRuntimes, 0, sizeof(NodeRuntime*) * numNodes);
}

ScriptRuntime::~ScriptRuntime()
{
	delete m_nodeRuntimes;
}

void ScriptRuntime::execute()
{
	for (int nodeCall : m_script->getEntryPoints())
	{
		NodeRuntime* nodeRuntime = getNodeCallRuntime(nodeCall);
		Node* node = m_script->getNode(nodeCall);
		node->execute(nodeRuntime);
	}
}

NodeRuntime* ScriptRuntime::getNodeCallRuntime(int nodeCall)
{
	assert(m_script->debugIsNodeCallValid(nodeCall));
	NodeRuntime* nodeRuntime = m_nodeRuntimes[nodeCall];
	if (nodeRuntime == nullptr)
	{
		Node* node = m_script->getNode(nodeCall);
		nodeRuntime = node->createRuntime(this, nodeCall);
		m_nodeRuntimes[nodeCall] = nodeRuntime;
	}
	return nodeRuntime;
}

void ScriptRuntime::impulse(NodeRuntime* nodeRuntime, int outputPinIndex)
{
	Pin inputPin;
	m_script->getInputPin(nodeRuntime->getNodeCall(), outputPinIndex, inputPin);
	NodeRuntime* inputNodeRuntime = getNodeCallRuntime(inputPin.getNodeCall());
	inputNodeRuntime->setCurrentInputPinIndex(inputPin.getIndex());
	Node* inputNode = m_script->getNode(inputPin.getNodeCall());
	inputNode->execute(inputNodeRuntime);
	inputNodeRuntime->clearCurrentInputPinIndex();
}



