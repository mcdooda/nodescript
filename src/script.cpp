#include "script.h"
#include "node.h"
#include "scriptruntime.h"

Script::Script()
{
	
}

Script::~Script()
{
	
}

NodeCall Script::addNode(Node* node)
{
	#ifdef NODESCRIPT_DEBUG
	node->getNodeName(); // ensures a name is given to the actual node type
	#endif
	m_nodes.push_back(node);
	NodeCall nodeCall = m_nodes.size() - 1;
	m_outputPins.emplace(std::piecewise_construct, std::forward_as_tuple(nodeCall), std::forward_as_tuple());
	m_inputPins.emplace(std::piecewise_construct, std::forward_as_tuple(nodeCall), std::forward_as_tuple());
	node->addedToScript(this, nodeCall);
	return nodeCall;
}

void Script::addLink(NodeCall nodeCall1, PinIndex outputPinIndex, NodeCall nodeCall2, PinIndex inputPinIndex)
{
	NODESCRIPT_ASSERT_MSG(
		isLinkValid(nodeCall1, outputPinIndex, nodeCall2, inputPinIndex),
		"Trying to add a link from a pin of type \"%s\" to \"%s\"",
		getNode(nodeCall1)->debugGetPinType(outputPinIndex),
		getNode(nodeCall2)->debugGetPinType(inputPinIndex)
	);
	m_outputPins[nodeCall1][outputPinIndex].emplace_back(nodeCall2, inputPinIndex);
	m_inputPins[nodeCall2].emplace(std::piecewise_construct, std::forward_as_tuple(inputPinIndex), std::forward_as_tuple(nodeCall1, outputPinIndex));
}

bool Script::isLinkValid(NodeCall nodeCall1, PinIndex outputPinIndex, NodeCall nodeCall2, PinIndex inputPinIndex)
{
	NODESCRIPT_ASSERT(debugIsNodeCallValid(nodeCall1));
	NODESCRIPT_ASSERT(debugIsNodeCallValid(nodeCall2));
	
	Node* node1 = getNode(nodeCall1);
	Node* node2 = getNode(nodeCall2);
	return node1 != node2 && node1->getPinTypeId(outputPinIndex) == node2->getPinTypeId(inputPinIndex);
}

void Script::optimize()
{
	#ifdef NODESCRIPT_DEBUG
	int gain = 0;
	gain += m_nodes.capacity() * sizeof(Node*);
	#endif
	m_nodes.shrink_to_fit();
	#ifdef NODESCRIPT_DEBUG
	gain -= m_nodes.capacity() * sizeof(Node*);
	gain += m_entryPoints.capacity() * sizeof(NodeCall);
	#endif
	m_entryPoints.shrink_to_fit();
	#ifdef NODESCRIPT_DEBUG
	gain -= m_entryPoints.capacity() * sizeof(NodeCall);
	#endif
	for (std::map<NodeCall, std::map<PinIndex, std::vector<Pin>>>::value_type& nodeCallOutputPins : m_outputPins)
	{
		for (std::map<PinIndex, std::vector<Pin>>::value_type& outputPin : nodeCallOutputPins.second)
		{
			#ifdef NODESCRIPT_DEBUG
			gain += outputPin.second.capacity() * sizeof(Pin);
			#endif
			outputPin.second.shrink_to_fit();
			#ifdef NODESCRIPT_DEBUG
			gain -= outputPin.second.capacity() * sizeof(Pin);
			#endif
		}
	}
	#ifdef NODESCRIPT_DEBUG
	#ifdef NODESCRIPT_VERBOSE
	std::cerr << "[script] we won " << gain << " bytes" << std::endl;
	#endif
	#endif
}

ScriptRuntime* Script::createRuntime()
{
	return new ScriptRuntime(this);
}

void Script::addEntryPoint(NodeCall nodeCall)
{
	m_entryPoints.push_back(nodeCall);
}

Node* Script::getNode(NodeCall nodeCall) const
{
	NODESCRIPT_ASSERT(debugIsNodeCallValid(nodeCall));
	return m_nodes[nodeCall];
}

int Script::getNumNodes() const
{
	return m_nodes.size();
}

void Script::getInputPins(NodeCall nodeCall, PinIndex outputPinIndex, std::vector<Pin>*& pins)
{
	pins = &m_outputPins[nodeCall][outputPinIndex];
}

void Script::getOutputPin(NodeCall nodeCall, PinIndex inputPinIndex, Pin& pin)
{
	pin = m_inputPins[nodeCall][inputPinIndex];
}

#ifdef NODESCRIPT_DEBUG
bool Script::debugIsNodeCallValid(NodeCall nodeCall) const
{
	return nodeCall >= 0 && nodeCall < static_cast<int>(m_nodes.size());
}
#endif



