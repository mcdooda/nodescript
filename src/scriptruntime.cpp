#include <cstring>
#include <cassert>
#include "scriptruntime.h"
#include "script.h"
#include "node.h"
#include "noderuntime.h"

ScriptRuntime::ScriptRuntime(Script* script) :
	m_script(script)
{
	createNodeRuntimes();
	optimizeNodeRuntimeLinks();
}

ScriptRuntime::~ScriptRuntime()
{
	int numNodes = m_script->getNumNodes();
	for (int i = 0; i < numNodes; i++)
	{
		delete m_nodeRuntimes[i];
	}
	delete m_nodeRuntimes;
}

void ScriptRuntime::execute()
{
	for (int nodeCall : m_script->getEntryPoints())
	{
		NodeRuntime* nodeRuntime = getNodeCallRuntime(nodeCall);
		nodeRuntime->execute(ENTRY_POINT_PIN_INDEX);
	}
}

NodeRuntime* ScriptRuntime::getNodeCallRuntime(int nodeCall) const
{
	assert(m_script->debugIsNodeCallValid(nodeCall));
	return m_nodeRuntimes[nodeCall];
}

void ScriptRuntime::createNodeRuntimes()
{
	int numNodes = m_script->getNumNodes();
	m_nodeRuntimes = new NodeRuntime*[numNodes];
	int nodeCall = 0;
	for (Node* node : m_script->getNodes())
	{
		NodeRuntime* nodeRuntime = node->createRuntime(this, nodeCall);
		m_nodeRuntimes[nodeCall] = nodeRuntime;
		nodeCall++;
	}
}

void ScriptRuntime::optimizeNodeRuntimeLinks()
{
	int numNodes = m_script->getNumNodes();
	for (int nodeCall = 0; nodeCall < numNodes; nodeCall++)
	{
		m_nodeRuntimes[nodeCall]->optimizeLinks(this);
	}
}



