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
		
		inline Script* getScript() const { return m_script; }
		
	private:
		void createNodeRuntimes();
		void optimizeNodeRuntimeLinks();
		
		Script* m_script;
		NodeRuntime** m_nodeRuntimes;
};

#endif // SCRIPTRUNTIME_H



