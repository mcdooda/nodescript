#ifndef NODE_H
#define NODE_H

#include <vector>
#include <string>
#include "pin.h"

class ScriptRuntime;
class NodeRuntime;
class Script;

#define NODE(NodeType, NodeName) \
	static Node* factory() { return new NodeType(); } \
	static const std::string getNodeName() { return NodeName; }

class Node
{
	friend class NodeRuntime;
	public:
		Node();
		virtual ~Node();
		
		virtual void optimize();
		
		virtual const char* getPinName(int pinIndex) const;
		
		virtual void execute(NodeRuntime* runtime) const = 0;
		
		virtual void addedToScript(Script* script, int nodeCall) const {}
		
		virtual NodeRuntime* createRuntime(ScriptRuntime* scriptRuntime, int nodeCall);
		
	protected:
		template <class T>
		void inValuePin()
		{
			m_inputValuePins.push_back(T::Index);
		}
		
		template <class T>
		void inImpulsePin()
		{
			m_inputImpulsePins.push_back(T::Index);
		}
		
		template <class T>
		void outValuePin()
		{
			m_outputValuePins.push_back(T::Index);
		}
		
		template <class T>
		void outImpulsePin()
		{
			m_outputImpulsePins.push_back(T::Index);
		}
		
		void createOutputValues(NodeRuntime* runtime);
		void clearOutputValues(NodeRuntime* runtime);
		
		std::vector<int> m_inputValuePins;
		std::vector<int> m_inputImpulsePins;
		
		std::vector<int> m_outputValuePins;
		std::vector<int> m_outputImpulsePins;
}; // Node

typedef Node* (*NodeFactory)();

#endif // NODE_H



