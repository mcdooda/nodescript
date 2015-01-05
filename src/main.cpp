#include "scriptengine.h"
#include "script.h"
#include "scriptruntime.h"

#include "nodes/initnode.h"
#include "nodes/testnode.h"
#include "nodes/constantvaluenode.h"
#include "nodes/math/addnode.h"

using namespace node;

int main(int argc, char* argv[])
{
	ScriptEngine engine;
	
	{
		Script* script = engine.newScript();
		
		Node* initNode = engine.getNodeInstance("Init");
		Node* testNode = engine.getNodeInstance("Test Node 1");
		Node* intValueNode = engine.getNodeInstance("Int Value");
		Node* floatValueNode = engine.getNodeInstance("Float Value");
		Node* mathAddNode = engine.getNodeInstance("Add");
		
		int initCall = script->addNode(initNode);
		int intValueCall1 = script->addNode(intValueNode);
		int intValueCall2 = script->addNode(intValueNode);
		int floatValueCall = script->addNode(floatValueNode);
		
		/*
		init+> --------------\
		                      \
		             +++++++   \---- <+++++++++++++> ---- <+++++++++++++
		int1+> ---- <+ add +> ------ <+ testnode1 +> ---- <+ testnode2 +
		int2+> ---- <+++++++   /---- <+++++++++++++> ---- <+++++++++++++
		                      /
		float+> -------------/
		*/
		
		int mathAddCall = script->addNode(mathAddNode);
		script->addLink(intValueCall1, IntConstantValueNode::ValueOutPin::Index, mathAddCall, math::AddNode::Int1InPin::Index);
		script->addLink(intValueCall2, IntConstantValueNode::ValueOutPin::Index, mathAddCall, math::AddNode::Int2InPin::Index);
		
		int testNodeCall1 = script->addNode(testNode);
		script->addLink(initCall, InitNode::ImpulseOutPin::Index, testNodeCall1, TestNode::ImpulseInPin::Index);
		script->addLink(mathAddCall, math::AddNode::IntOutPin::Index, testNodeCall1, TestNode::IntInPin::Index);
		script->addLink(floatValueCall, FloatConstantValueNode::ValueOutPin::Index, testNodeCall1, TestNode::FloatInPin::Index);
		
		int testNodeCall2 = script->addNode(testNode);
		script->addLink(testNodeCall1, TestNode::IntOutPin::Index, testNodeCall2, TestNode::IntInPin::Index);
		script->addLink(testNodeCall1, TestNode::FloatOutPin::Index, testNodeCall2, TestNode::FloatInPin::Index);
		script->addLink(testNodeCall1, TestNode::ImpulseOutPin::Index, testNodeCall2, TestNode::ImpulseInPin::Index);
		
		for (int i = 0; i < 1000000; i++)
		{
			ScriptRuntime* scriptRuntime = script->createRuntime();
			
			ConstantValueNodeRuntime<int>* intConstantValueNodeRuntime1 = static_cast<ConstantValueNodeRuntime<int>*>(scriptRuntime->getNodeCallRuntime(intValueCall1));
			intConstantValueNodeRuntime1->setValue(i);
			
			ConstantValueNodeRuntime<int>* intConstantValueNodeRuntime2 = static_cast<ConstantValueNodeRuntime<int>*>(scriptRuntime->getNodeCallRuntime(intValueCall2));
			intConstantValueNodeRuntime2->setValue(i);
			
			ConstantValueNodeRuntime<float>* floatConstantValueNodeRuntime = static_cast<ConstantValueNodeRuntime<float>*>(scriptRuntime->getNodeCallRuntime(floatValueCall));
			floatConstantValueNodeRuntime->setValue((float)i);
			
			scriptRuntime->execute();
			delete scriptRuntime;
		}
		
		delete script;
	}
	
	return 0;
}



