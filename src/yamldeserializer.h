#ifndef YAMLDESERIALIZER_H
#define YAMLDESERIALIZER_H

#ifdef NODESCRIPT_INTROSPECTION

#include <iostream>
#include <yaml-cpp/yaml.h>

#include "nodecall.h"
#include "pin.h"

class ScriptEngine;
class Script;

class ScriptBuilder;

std::istream& operator>>(std::istream& in, ScriptBuilder&& scriptBuilder);

struct ScriptBuilder
{
	ScriptBuilder(const ScriptEngine& scriptEngine, Script& script) :
		m_scriptEngine(scriptEngine),
		m_script(script)
	{
		
	}
	
	const ScriptEngine& m_scriptEngine;
	Script& m_script;
};

class YamlDeserializer
{
	public:
		YamlDeserializer(const ScriptEngine& scriptEngine, Script& script);
		~YamlDeserializer();
		
		void read(const YAML::Node& in) const;
		
	private:
		void readNode(const YAML::Node& node, NodeCall nodeCall) const;
		void readNodeOutConnections(const YAML::Node& node, NodeCall outNodeCall) const;
		
	private:
		const ScriptEngine& m_scriptEngine;
		Script& m_script;
};

#endif // NODESCRIPT_INTROSPECTION

#endif // YAMLDESERIALIZER_H



