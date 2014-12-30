#include "scriptengine.h"
#include "script.h"

#include "nodes/initnode.h"
#include "nodes/testnode.h"

ScriptEngine::ScriptEngine()
{
	registerNodes();
}

ScriptEngine::~ScriptEngine()
{
	
}

Script* ScriptEngine::newScript()
{
	return new Script();
}

Node* ScriptEngine::getNodeInstance(const std::string& nodeName)
{
	std::map<std::string, Node*>::iterator it = m_nodes.find(nodeName);
	Node* node;
	if (it != m_nodes.end())
	{
		node = it->second;
	}
	else
	{
		node = m_nodeFactories[nodeName]();
		node->optimize();
		m_nodes[nodeName] = node;
	}
	return node;
}

void ScriptEngine::registerNodes()
{
	registerNode<InitNode>();
	registerNode<TestNode>();
}



