#include "../src/nodescript.h"

#define NUM_NEW_SCRIPT 100
#define NUM_EXECS 10000

using namespace node;

int main(int argc, char* argv[])
{
	for (int i = 0; i < NUM_NEW_SCRIPT; i++)
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
		
			NodeCall initCall = script->addNode(initNode);
			NodeCall intValueCall = script->addNode(intValueNode);
			NodeCall floatValueCall = script->addNode(floatValueNode);
			NodeCall stringValueCall = script->addNode(stringValueNode);
		
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
		
			NodeCall mathAddCall = script->addNode(mathAddNode);
			script->addLink(intValueCall, IntConstantValueNode::ValueOutPin::Index, mathAddCall, math::AddNode::Int1InPin::Index);
			script->addLink(intValueCall, IntConstantValueNode::ValueOutPin::Index, mathAddCall, math::AddNode::Int2InPin::Index);
		
			NodeCall testNodeCall1 = script->addNode(testNode);
			script->addLink(initCall, InitNode::ImpulseOutPin::Index, testNodeCall1, TestNode::ImpulseInPin::Index);
			script->addLink(mathAddCall, math::AddNode::IntOutPin::Index, testNodeCall1, TestNode::IntInPin::Index);
			script->addLink(floatValueCall, FloatConstantValueNode::ValueOutPin::Index, testNodeCall1, TestNode::FloatInPin::Index);
			script->addLink(stringValueCall, StringConstantValueNode::ValueOutPin::Index, testNodeCall1, TestNode::StringInPin::Index);
		
			NodeCall testNodeCall2 = script->addNode(testNode);
			script->addLink(testNodeCall1, TestNode::IntOutPin::Index, testNodeCall2, TestNode::IntInPin::Index);
			script->addLink(testNodeCall1, TestNode::FloatOutPin::Index, testNodeCall2, TestNode::FloatInPin::Index);
			script->addLink(testNodeCall1, TestNode::StringOutPin::Index, testNodeCall2, TestNode::StringInPin::Index);
			script->addLink(testNodeCall1, TestNode::ImpulseOutPin::Index, testNodeCall2, TestNode::ImpulseInPin::Index);
			
			script->optimize();
		
			for (int j = 0; j < NUM_EXECS; ++j)
			{
				ScriptRuntime* scriptRuntime = script->createRuntime();
			
				ConstantValueNodeRuntime<int>* intConstantValueNodeRuntime = scriptRuntime->getNodeCallRuntime<ConstantValueNodeRuntime<int>>(intValueCall);
				intConstantValueNodeRuntime->setValue(j);
			
				ConstantValueNodeRuntime<float>* floatConstantValueNodeRuntime = scriptRuntime->getNodeCallRuntime<ConstantValueNodeRuntime<float>>(floatValueCall);
				floatConstantValueNodeRuntime->setValue(static_cast<float>(j) / 2);
			
				StringConstantValueNodeRuntime* stringConstantValueNodeRuntime = scriptRuntime->getNodeCallRuntime<StringConstantValueNodeRuntime>(stringValueCall);
				stringConstantValueNodeRuntime->setValue(std::string("String#") + std::to_string(j));
			
				//std::cout << "Execution #" << j << " ======================================" << std::endl;
				scriptRuntime->execute();
				delete scriptRuntime;
			}
		
			delete script;
		}
	}
	
	return 0;
}



