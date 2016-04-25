#ifndef YAMLSERIALIZER_H
#define YAMLSERIALIZER_H

#ifdef NODESCRIPT_INTROSPECTION

#include <iostream>
#include <yaml-cpp/yaml.h>

#include "nodecall.h"

class Script;

std::ostream& operator<<(std::ostream& out, const Script& script);
YAML::Emitter& operator<<(YAML::Emitter& out, const Script& script);

class YamlSerializer
{
	public:
		YamlSerializer(const Script& script);
		~YamlSerializer();
		
		void write(YAML::Emitter& out) const;
		
	private:
		void writeNode(YAML::Emitter& out, NodeCall nodeCall) const;
		void writeNodeOutConnections(YAML::Emitter& out, NodeCall nodeCall) const;
		
	private:
		const Script& m_script;
};

#endif // NODESCRIPT_INTROSPECTION

#endif // YAMLSERIALIZER_H



