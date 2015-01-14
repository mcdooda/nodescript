#ifndef SCRIPTRUNTIME_H
#define SCRIPTRUNTIME_H

class Script;
class NodeRuntime;

class ScriptRuntime
{
	public:
		ScriptRuntime(Script* script);
		~ScriptRuntime();
		
		void execute();
		
		NodeRuntime* getNodeCallRuntime(int nodeCall) const;
		
		template <class T>
		T* getNodeCallRuntime(int nodeCall) const
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



