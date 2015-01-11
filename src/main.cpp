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
		Node* stringValueNode = engine.getNodeInstance("String Value");
		Node* mathAddNode = engine.getNodeInstance("Add");
		
		int initCall = script->addNode(initNode);
		int intValueCall1 = script->addNode(intValueNode);
		int intValueCall2 = script->addNode(intValueNode);
		int floatValueCall = script->addNode(floatValueNode);
		int stringValueCall = script->addNode(stringValueNode);
		
		/*
		   init +> ==============+
		                          \
		                           \
		                 +++++++    +===== <+++++++++++++> ==== <+++++++++++++
		   int1 +> ---- <+ add +> -------- <+ testnode1 +> ---- <+ testnode2 +
		   int2 +> ---- <+++++++   +------ <+           +> ---- <+           +
		                          /  +---- <+++++++++++++> ---- <+++++++++++++
		  float +> --------------+  /
		                           /
		string* +> ---------------+
		*/
		
		int mathAddCall = script->addNode(mathAddNode);
		script->addLink(intValueCall1, IntConstantValueNode::ValueOutPin::Index, mathAddCall, math::AddNode::Int1InPin::Index);
		script->addLink(intValueCall2, IntConstantValueNode::ValueOutPin::Index, mathAddCall, math::AddNode::Int2InPin::Index);
		
		int testNodeCall1 = script->addNode(testNode);
		script->addLink(initCall, InitNode::ImpulseOutPin::Index, testNodeCall1, TestNode::ImpulseInPin::Index);
		script->addLink(mathAddCall, math::AddNode::IntOutPin::Index, testNodeCall1, TestNode::IntInPin::Index);
		script->addLink(floatValueCall, FloatConstantValueNode::ValueOutPin::Index, testNodeCall1, TestNode::FloatInPin::Index);
		script->addLink(stringValueCall, StringConstantValueNode::ValueOutPin::Index, testNodeCall1, TestNode::StringInPin::Index);
		
		int testNodeCall2 = script->addNode(testNode);
		script->addLink(testNodeCall1, TestNode::IntOutPin::Index, testNodeCall2, TestNode::IntInPin::Index);
		script->addLink(testNodeCall1, TestNode::FloatOutPin::Index, testNodeCall2, TestNode::FloatInPin::Index);
		script->addLink(testNodeCall1, TestNode::StringOutPin::Index, testNodeCall2, TestNode::StringInPin::Index);
		script->addLink(testNodeCall1, TestNode::ImpulseOutPin::Index, testNodeCall2, TestNode::ImpulseInPin::Index);
		
		for (int i = 0; i < 10; i++)
		{
			ScriptRuntime* scriptRuntime = script->createRuntime();
			
			ConstantValueNodeRuntime<int>* intConstantValueNodeRuntime1 = scriptRuntime->getNodeCallRuntime<ConstantValueNodeRuntime<int>>(intValueCall1);
			intConstantValueNodeRuntime1->setValue(i);
			
			ConstantValueNodeRuntime<int>* intConstantValueNodeRuntime2 = scriptRuntime->getNodeCallRuntime<ConstantValueNodeRuntime<int>>(intValueCall2);
			intConstantValueNodeRuntime2->setValue(1);
			
			ConstantValueNodeRuntime<float>* floatConstantValueNodeRuntime = scriptRuntime->getNodeCallRuntime<ConstantValueNodeRuntime<float>>(floatValueCall);
			floatConstantValueNodeRuntime->setValue(static_cast<float>(i) / 2);
			
			StringConstantValueNodeRuntime* stringConstantValueNodeRuntime = scriptRuntime->getNodeCallRuntime<StringConstantValueNodeRuntime>(stringValueCall);
			stringConstantValueNodeRuntime->setValue(std::string("String#") + std::to_string(i));
			
			std::cout << "Execution #" << i << " ======================================" << std::endl;
			scriptRuntime->execute();
			delete scriptRuntime;
		}
		
		delete script;
	}
	
	return 0;
}



