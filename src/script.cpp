#include "script.h"
#include "node.h"
#include "scriptruntime.h"
#include <algorithm>

Script::Script()
{
	
}

Script::~Script()
{
	
}

NodeCall Script::addNode(const Node* node)
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
	NODESCRIPT_ASSERT_MSG(
		!linkExists(nodeCall1, outputPinIndex, nodeCall2, inputPinIndex),
		"Trying to add a link that already exists"
	);
	m_outputPins[nodeCall1][outputPinIndex].emplace_back(nodeCall2, inputPinIndex);
	m_inputPins[nodeCall2].emplace(std::piecewise_construct, std::forward_as_tuple(inputPinIndex), std::forward_as_tuple(nodeCall1, outputPinIndex));
}

bool Script::isLinkValid(NodeCall nodeCall1, PinIndex outputPinIndex, NodeCall nodeCall2, PinIndex inputPinIndex)
{
	NODESCRIPT_ASSERT(debugIsNodeCallValid(nodeCall1));
	NODESCRIPT_ASSERT(debugIsNodeCallValid(nodeCall2));
	const Node* node1 = getNode(nodeCall1);
	const Node* node2 = getNode(nodeCall2);
	return nodeCall1 != nodeCall2 && node1->getPinTypeId(outputPinIndex) == node2->getPinTypeId(inputPinIndex);
}

#if defined(NODESCRIPT_DEBUG) || defined(NODESCRIPT_INTROSPECTION)
bool Script::linkExists(NodeCall nodeCall1, PinIndex outputPinIndex, NodeCall nodeCall2, PinIndex inputPinIndex)
{
	std::map<PinIndex, Pin>& nodeInputs = m_inputPins[nodeCall2];
	std::map<PinIndex, Pin>::iterator it = nodeInputs.find(inputPinIndex);	
	
	if (it == nodeInputs.end())
		return false;
		
	const Pin& outputPin = it->second;
	return outputPin.getNodeCall() == nodeCall1 && outputPin.getIndex() == outputPinIndex;
}
#endif

#ifdef NODESCRIPT_INTROSPECTION
void Script::removeNode(NodeCall nodeCall)
{
	NODESCRIPT_ASSERT(debugIsNodeCallValid(nodeCall));
	m_nodes[nodeCall] = nullptr;
	m_inputPins.erase(nodeCall);
	m_outputPins.erase(nodeCall);
}

void Script::removeLink(NodeCall nodeCall1, PinIndex outputPinIndex, NodeCall nodeCall2, PinIndex inputPinIndex)
{
	NODESCRIPT_ASSERT(isLinkValid(nodeCall1, outputPinIndex, nodeCall2, inputPinIndex));

	{
		std::vector<Pin>& nodeOutputs = m_outputPins[nodeCall1][outputPinIndex];
		std::vector<Pin>::iterator it = std::find(nodeOutputs.begin(), nodeOutputs.end(), Pin(nodeCall2, inputPinIndex));
		NODESCRIPT_ASSERT(it != nodeOutputs.end());
		nodeOutputs.erase(it);
	}

	{
		std::map<PinIndex, Pin>& nodeInputs = m_inputPins[nodeCall2];
		std::map<PinIndex, Pin>::iterator it = nodeInputs.find(inputPinIndex);
		NODESCRIPT_ASSERT(it != nodeInputs.end());
		nodeInputs.erase(it);
	}
}
#endif

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
	#if defined(NODESCRIPT_DEBUG) && defined(NODESCRIPT_VERBOSE)
	std::cerr << "[script] we won " << gain << " bytes" << std::endl;
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

const Node* Script::getNode(NodeCall nodeCall) const
{
	NODESCRIPT_ASSERT(debugIsNodeCallValid(nodeCall));
	return m_nodes[nodeCall];
}

const std::vector<Pin>* Script::getInputPins(NodeCall nodeCall, PinIndex outputPinIndex) const
{
	std::map<NodeCall, std::map<PinIndex, std::vector<Pin>>>::const_iterator nodeOutputPinsIt = m_outputPins.find(nodeCall);
	
	if (nodeOutputPinsIt == m_outputPins.cend())
		return nullptr;
	
	std::map<PinIndex, std::vector<Pin>>::const_iterator connectedPinsIt = nodeOutputPinsIt->second.find(outputPinIndex);
	
	if (connectedPinsIt == nodeOutputPinsIt->second.cend())
		return nullptr;
	
	return &connectedPinsIt->second;
}

const Pin& Script::getOutputPin(NodeCall nodeCall, PinIndex inputPinIndex) const
{
	const std::map<PinIndex, Pin>& nodeInputPins = m_inputPins.at(nodeCall);
	return nodeInputPins.at(inputPinIndex);
}

#ifdef NODESCRIPT_INTROSPECTION
const std::map<PinIndex, std::vector<Pin>>* Script::getOutputPins(NodeCall nodeCall) const
{
	std::map<NodeCall, std::map<PinIndex, std::vector<Pin>>>::const_iterator nodeOutputPinsIt = m_outputPins.find(nodeCall);
	
	if (nodeOutputPinsIt == m_outputPins.cend())
		return nullptr;
	
	return &nodeOutputPinsIt->second;
}
#endif

#ifdef NODESCRIPT_DEBUG
bool Script::debugIsNodeCallValid(NodeCall nodeCall) const
{
#ifdef NODESCRIPT_INTROSPECTION
	return nodeCall >= 0 && nodeCall < static_cast<int>(m_nodes.size()) && m_nodes[nodeCall];
#else
	return nodeCall >= 0 && nodeCall < static_cast<int>(m_nodes.size());
#endif
}
#endif



