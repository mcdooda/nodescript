#include "scriptengine.h"
#include "script.h"

#include "nodes/initnode.h"
#include "nodes/testnode.h"
#include "nodes/constantvaluenode.h"
#include "nodes/math/addnode.h"

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
		NODESCRIPT_ASSERT_MSG(m_nodeFactories.find(nodeName) != m_nodeFactories.end(), "The node type \"%s\" is not registered", nodeName.c_str());
		node = m_nodeFactories[nodeName]();
		node->optimize();
		m_nodes[nodeName] = node;
	}
	return node;
}

void ScriptEngine::registerNodes()
{
	registerNode<node::InitNode>();
	registerNode<node::TestNode>();
	registerNode<node::BoolConstantValueNode>();
	registerNode<node::IntConstantValueNode>();
	registerNode<node::LongConstantValueNode>();
	registerNode<node::FloatConstantValueNode>();
	registerNode<node::DoubleConstantValueNode>();
	registerNode<node::StringConstantValueNode>();
	registerNode<node::math::AddNode>();
}



