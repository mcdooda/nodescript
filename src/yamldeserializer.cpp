#include "yamldeserializer.h"
#include "node.h"
#include "scriptengine.h"
#

std::istream& operator>>(std::istream& in, ScriptBuilder&& scriptBuilder)
{
	YamlDeserializer deserializer(scriptBuilder.m_scriptEngine, scriptBuilder.m_script);
	YAML::Node yaml = YAML::Load(in);
	deserializer.read(yaml);
	return in;
}

YamlDeserializer::YamlDeserializer(const ScriptEngine& scriptEngine, Script& script) :
	m_scriptEngine(scriptEngine),
	m_script(script)
{
	
}

YamlDeserializer::~YamlDeserializer()
{
	
}

void YamlDeserializer::read(const YAML::Node& in) const
{
	const YAML::Node& script = in["script"];
	
	int numNodes = static_cast<int>(script.size());

	for (NodeCall nodeCall = 0; nodeCall < numNodes; ++nodeCall)
	{
		readNode(script[nodeCall], nodeCall);
	}
	
	for (YAML::const_iterator it = script.begin(); it != script.end(); ++it)
	{
		NodeCall outNodeCall = static_cast<NodeCall>(it->first.as<int>());
		readNodeOutConnections(it->second, outNodeCall);
	}
}

void YamlDeserializer::readNode(const YAML::Node& node, NodeCall nodeCall) const
{
	const std::string nodeName = node["name"].as<std::string>();
	const Node* nodeInstance = m_scriptEngine.getNodeInstance(nodeName);
	NodeCall effectiveNodeCall = m_script.addNode(nodeInstance);
	NODESCRIPT_ASSERT(nodeCall == effectiveNodeCall);
}

void YamlDeserializer::readNodeOutConnections(const YAML::Node& node, NodeCall outNodeCall) const
{
	const YAML::Node& outConnections = node["outConnections"];
	if (!outConnections)
		return;
	
	for (YAML::const_iterator it = outConnections.begin(); it != outConnections.end(); ++it)
	{
		PinIndex outPinIndex = static_cast<PinIndex>(it->first.as<int>());
		
		unsigned int numLinks = it->second.size();
		for (unsigned int i = 0; i < numLinks; ++i)
		{
			const YAML::Node& inputPin = it->second[i];
			NodeCall inNodeCall = static_cast<NodeCall>(inputPin["node"].as<int>());
			PinIndex inPinIndex = static_cast<PinIndex>(inputPin["pin"].as<int>());
			m_script.addLink(outNodeCall, outPinIndex, inNodeCall, inPinIndex);
		}
	}
}



