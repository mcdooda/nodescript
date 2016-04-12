#include <algorithm>

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

#ifdef NODESCRIPT_INTROSPECTION
const std::map<std::string, const Node*>& ScriptEngine::getAllRegisteredNodes() const
{
	instantiateAllNodes();
	return m_nodes;
}
#endif

const Node* ScriptEngine::getNodeInstance(const std::string& nodeName) const
{
	std::map<std::string, const Node*>::const_iterator it = m_nodes.find(nodeName);
	const Node* node = nullptr;
	if (it != m_nodes.end())
	{
		node = it->second;
	}
	else
	{
		NODESCRIPT_ASSERT_MSG(m_nodeFactories.find(nodeName) != m_nodeFactories.end(), "The node type \"%s\" is not registered", nodeName.c_str());
		const NodeFactory nodeFactory = m_nodeFactories.at(nodeName);
		Node* newNode = nodeFactory();
		newNode->optimize();
		m_nodes[nodeName] = newNode;
		node = const_cast<const Node*>(newNode);
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

#ifdef NODESCRIPT_INTROSPECTION
void ScriptEngine::instantiateAllNodes() const
{
	std::for_each(
		m_nodeFactories.begin(),
		m_nodeFactories.end(),
		[this](const std::pair<std::string, NodeFactory>& pair) { getNodeInstance(pair.first); }
	);
}
#endif



