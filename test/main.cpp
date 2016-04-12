#include "../src/nodescript.h"

#define NUM_NEW_SCRIPT 1
#define NUM_EXECS 10

using namespace node;

int main(int argc, char* argv[])
{
	for (int i = 0; i < NUM_NEW_SCRIPT; i++)
	{
		ScriptEngine engine;
	
		{
			Script* script = engine.newScript();
		
			const Node* initNode = engine.getNodeInstance("Init");
			const Node* testNode = engine.getNodeInstance("Test Node 1");
			const Node* intValueNode = engine.getNodeInstance("Int Value");
			const Node* floatValueNode = engine.getNodeInstance("Float Value");
			const Node* doubleValueNode = engine.getNodeInstance("Double Value");
			const Node* stringValueNode = engine.getNodeInstance("String Value");
			const Node* doubleToIntCastNode = engine.getNodeInstance("Double To Int Cast");
			const Node* mathAddNode = engine.getNodeInstance("Add");
		
			NodeCall initCall = script->addNode(initNode);
			NodeCall intValueCall = script->addNode(intValueNode);
			NodeCall doubleValueCall = script->addNode(doubleValueNode);
			NodeCall floatValueCall = script->addNode(floatValueNode);
			NodeCall stringValueCall = script->addNode(stringValueNode);
		
			/*
			   init +> ======================================+
			                                                  \
			                                                   \
			                                          +++++++   +====== <+++++++++++++> ==== <+++++++++++++
			    int +> ----------------------------- <+ add +> -------- <+ testnode1 +> ---- <+ testnode2 +
			 double +> ---- <+ double to int +> ---- <+++++++   +------ <+           +> ---- <+           +
			                                                   /  +---- <+++++++++++++> ---- <+++++++++++++
			  float +> ---------------------------------------+  /
			                                                    /
			string* +> ----------------------------------------+
			*/
			
			NodeCall doubleToIntCastCall = script->addNode(doubleToIntCastNode);
			script->addLink(doubleValueCall, DoubleConstantValueNode::ValueOutPin::Index, doubleToIntCastCall, DoubleToIntCastNode::FromValuePin::Index);
		
			NodeCall mathAddCall = script->addNode(mathAddNode);
			script->addLink(intValueCall, IntConstantValueNode::ValueOutPin::Index, mathAddCall, math::AddNode::Int1InPin::Index);
			script->addLink(doubleToIntCastCall, DoubleToIntCastNode::ToValuePin::Index, mathAddCall, math::AddNode::Int2InPin::Index);
		
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
				
				ConstantValueNodeRuntime<double>* doubleConstantValueNodeRuntime = scriptRuntime->getNodeCallRuntime<ConstantValueNodeRuntime<double>>(doubleValueCall);
				doubleConstantValueNodeRuntime->setValue(3.14);
			
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



