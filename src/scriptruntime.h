#ifndef SCRIPTRUNTIME_H
#define SCRIPTRUNTIME_H

#include "nodecall.h"

class Script;
class NodeRuntime;

class ScriptRuntime
{
	public:
		ScriptRuntime(Script* script);
		~ScriptRuntime();
		
		void execute();
		
		NodeRuntime* getNodeCallRuntime(NodeCall nodeCall) const;
		
		template <class T>
		T* getNodeCallRuntime(NodeCall nodeCall) const
		{
			return static_cast<T*>(getNodeCallRuntime(nodeCall));
		}
		
		inline Script* getScript() const { return m_script; }
		inline int* getCurrentExecutionIndex() { return &m_currentExecutionIndex; }
		
	private:
		void createNodeRuntimes();
		void optimizeNodeRuntimeLinks();
		
		Script* m_script;
		NodeRuntime** m_nodeRuntimes;
		int m_currentExecutionIndex;
};

#endif // SCRIPTRUNTIME_H



