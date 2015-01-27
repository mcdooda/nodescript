#include "scriptengine.h"
#include "script.h"

#include "nodes/initnode.h"
#include "nodes/testnode.h"
#include "nodes/constantvaluenode.h"
#include "nodes/castnode.h"
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
	// core
	registerNode<node::InitNode>();
	registerNode<node::TestNode>();
	
	// constants
	registerNode<node::BoolConstantValueNode>();
	registerNode<node::IntConstantValueNode>();
	registerNode<node::LongConstantValueNode>();
	registerNode<node::FloatConstantValueNode>();
	registerNode<node::DoubleConstantValueNode>();
	registerNode<node::StringConstantValueNode>();
	
	// cast
	registerNode<node::BoolToIntCastNode>();
	registerNode<node::BoolToLongCastNode>();
	registerNode<node::IntToBoolCastNode>();
	registerNode<node::IntToLongCastNode>();
	registerNode<node::IntToFloatCastNode>();
	registerNode<node::IntToDoubleCastNode>();
	registerNode<node::LongToBoolCastNode>();
	registerNode<node::LongToIntCastNode>();
	registerNode<node::LongToFloatCastNode>();
	registerNode<node::LongToDoubleCastNode>();
	registerNode<node::FloatToIntCastNode>();
	registerNode<node::FloatToLongCastNode>();
	registerNode<node::FloatToDoubleCastNode>();
	registerNode<node::DoubleToIntCastNode>();
	registerNode<node::DoubleToLongCastNode>();
	registerNode<node::DoubleToFloatCastNode>();
	
	// math
	registerNode<node::math::AddNode>();
}



