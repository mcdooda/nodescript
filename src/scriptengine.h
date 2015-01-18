#ifndef SCRIPTENGINE_H
#define SCRIPTENGINE_H

#include <string>
#include <map>

#include "node.h"

class Script;

class ScriptEngine
{
	public:
		ScriptEngine();
		~ScriptEngine();
		
		Node* getNodeInstance(const std::string& nodeName);
		
		Script* newScript();
		
	private:
		void registerNodes();
		
		template <class T>
		void registerNode()
		{
			m_nodeFactories[T::nodeName()] = T::factory;
		}
		
		std::map<std::string, Node*> m_nodes;
		std::map<std::string, NodeFactory> m_nodeFactories;
}; // ScriptEngine

#endif // SCRIPTENGINE_H



