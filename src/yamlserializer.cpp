#ifdef NODESCRIPT_INTROSPECTION

#include "yamlserializer.h"
#include "script.h"
#include "node.h"

std::ostream& operator<<(std::ostream& out, const Script& script)
{
	YAML::Emitter yaml;
	yaml << script;
	out << yaml.c_str();
	return out;
}

YAML::Emitter& operator<<(YAML::Emitter& out, const Script& script)
{
	YamlSerializer serializer(script);
	serializer.write(out);
	return out;
}

YamlSerializer::YamlSerializer(const Script& script) :
	m_script(script)
{
	
}

YamlSerializer::~YamlSerializer()
{
	
}

void YamlSerializer::write(YAML::Emitter& out) const
{
	out << YAML::BeginMap;
	
	out << YAML::Key << "script";
	
	out << YAML::Value << YAML::BeginMap;
	for (NodeCall nodeCall = 0; nodeCall < m_script.getNumNodes(); ++nodeCall)
	{
		writeNode(out, nodeCall);
	}
	out << YAML::EndMap;
	
	out << YAML::EndMap;
}

void YamlSerializer::writeNode(YAML::Emitter& out, NodeCall nodeCall) const
{
	out << YAML::Key << nodeCall;
	
	out << YAML::Value << YAML::BeginMap;
	
	const Node* node = m_script.getNode(nodeCall);
	
	out << YAML::Key << "name" << YAML::Value << node->getNodeName();
	
	writeNodeOutConnections(out, nodeCall);
	
	out << YAML::EndMap;
}

void YamlSerializer::writeNodeOutConnections(YAML::Emitter& out, NodeCall nodeCall) const
{
	const std::map<PinIndex, std::vector<Pin>>* outputPins = m_script.getOutputPins(nodeCall);
	
	if (!outputPins || outputPins->empty())
		return;
	
	out << YAML::Key << "outConnections";
	out << YAML::Value << YAML::BeginMap;
	
	for (const std::pair<const PinIndex, std::vector<Pin>>& links : *outputPins)
	{
		PinIndex outputPinIndex = links.first;
		const std::vector<Pin>& inputPins = links.second;
		
		if (inputPins.empty())
			continue;
		
		
		out << YAML::Key << static_cast<int>(outputPinIndex);
		out << YAML::Value << YAML::BeginSeq;
		
		for (const Pin& pin : inputPins)
		{
			NODESCRIPT_ASSERT(pin.isConnected());
			out << YAML::BeginMap;
			out << YAML::Key << "node" << YAML::Value << pin.getNodeCall();
			out << YAML::Key << "pin"  << YAML::Value << static_cast<int>(pin.getIndex());
			out << YAML::EndMap;
		}
		
		out << YAML::EndSeq;
	}
	
	out << YAML::EndMap;
}

#endif // NODESCRIPT_INTROSPECTION



