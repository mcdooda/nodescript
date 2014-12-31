#include "scriptengine.h"
#include "script.h"
#include "scriptruntime.h"

#include "nodes/initnode.h"
#include "nodes/testnode.h"

int main(int argc, char* argv[])
{
	ScriptEngine engine;
	
	{
		Script* script = engine.newScript();
	
		Node* initNode = engine.getNodeInstance("Init");
		int initCall = script->addNode(initNode);
	
		{
			Node* node = engine.getNodeInstance("Test Node 1");
			int nodeCall = script->addNode(node);
			script->addLink(initCall, InitNode::ImpulseOutPin::Index, nodeCall, TestNode::ImpulseInPin::Index);
			script->addLink(initCall, InitNode::IntOutPin::Index, nodeCall, TestNode::IntInPin::Index);
		}
		
		for (int i = 0; i < 1000; i++)
		{
			ScriptRuntime* scriptRuntime = script->createRuntime();
			scriptRuntime->execute();
			delete scriptRuntime;
		}
		
		delete script;
	}
	
	return 0;
}



