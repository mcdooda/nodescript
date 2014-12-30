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
		void impulse(NodeRuntime* nodeRuntime, int outputPinIndex);
		NodeRuntime* getNodeCallRuntime(int nodeCall);
		
		inline Script* getScript() const { return m_script; }
		
	private:
		Script* m_script;
		NodeRuntime** m_nodeRuntimes;
};

#endif // SCRIPTRUNTIME_H



