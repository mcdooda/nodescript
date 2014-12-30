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
		
	private:
		NodeRuntime* getNodeCallRuntime(int nodeCall);
		
		Script* m_script;
		NodeRuntime** m_nodeRuntimes;
};

#endif // SCRIPTRUNTIME_H



