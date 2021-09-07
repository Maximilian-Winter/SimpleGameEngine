#include "SceneGraph.h"


SceneGraphNode::SceneGraphNode() :m_NodeName("NODE_IS_NULL"),
                                  m_ParentNodeID(-100),
                                  m_NodeID(-1)
{
}

SceneGraphNode::SceneGraphNode(const SceneGraphNode& other)
{
	this->m_NodeName = other.m_NodeName;
	this->m_NodeTransform = other.m_NodeTransform;
	this->m_NodeFirstChildren = other.m_NodeFirstChildren;
	this->m_ParentNodeID = other.m_ParentNodeID;
	this->m_NodeID = other.m_NodeID;
}

SceneGraphNode::SceneGraphNode(int parentNodeID, int nodeID, Transform transform, std::string nodeName):
	m_NodeTransform(transform),
	m_NodeName(nodeName),
	m_ParentNodeID(parentNodeID),
	m_NodeID(nodeID)
{
}

SceneGraphNode::~SceneGraphNode()
{
	Destroy();
}

void SceneGraphNode::Release()
{
	delete this;
}

void SceneGraphNode::Update(std::vector<SceneObjectData>& meshesToRender)
{
	for (std::vector<SceneGraphNode*>::iterator i = m_NodeFirstChildren.begin();
	     i != m_NodeFirstChildren.end(); ++i)
	{
		(*i)->Update(meshesToRender);
	}
}

void SceneGraphNode::Destroy()
{
	for (std::vector<SceneGraphNode*>::iterator i = m_NodeFirstChildren.begin(); i != m_NodeFirstChildren.end(); ++i)
	{
		(*i)->Release();
	}
	m_NodeFirstChildren.clear();
}

StaticGeometryNode::StaticGeometryNode(int parentNodeID, int nodeID, Transform transform, std::string nodeName): SceneGraphNode(parentNodeID, nodeID, transform, nodeName)
{
}

StaticGeometryNode::~StaticGeometryNode()
= default;

void StaticGeometryNode::AddStaticMeshToNode(ID3D11Device* device, std::vector<StaticVertex> vertices, std::vector<int> indicies, std::vector<Subset> subsets, float aabbWidth, float aabbHeight, float aabbLength)
{
	StaticMesh staticMesh;
	staticMesh.CreateMeshData(device, vertices, indicies, subsets, aabbWidth, aabbHeight, aabbLength);
	m_StaticGeometry.push_back(staticMesh);
}

void StaticGeometryNode::Update(std::vector<SceneObjectData>& meshesToRender)
{
	for (auto element : m_StaticGeometry)
	{
		for(int i = 0; i < element.GetSubsetCount(); i++)
		{
			SceneObjectData temp;
			temp.ObjectRenderData = element.GetSubsetRenderData(i);
			DirectX::XMStoreFloat4x4(&temp.WorldMatrix, GetNodeTransform().GetLocalToWorldSpaceTransformMatrix());
			DirectX::XMVECTOR pos;
			DirectX::XMVECTOR rotation;
			DirectX::XMVECTOR scale;
			DirectX::XMMatrixDecompose(&scale, &rotation, &pos, DirectX::XMLoadFloat4x4(&temp.WorldMatrix));
			DirectX::XMStoreFloat3(&temp.Position, pos);
			DirectX::XMStoreFloat3(&temp.Scale, scale);
			DirectX::XMStoreFloat4(&temp.QuaternionRotation, rotation);
			meshesToRender.push_back(temp);
		}
	}
	SceneGraphNode::Update(meshesToRender);
}

void StaticGeometryNode::Destroy()
{
	for (auto element : m_StaticGeometry)
	{
		element.Release();
	}
	SceneGraphNode::Destroy();
}

EmptyNode::EmptyNode(int parentNodeID, int nodeID, Transform transform, std::string nodeName):SceneGraphNode(parentNodeID, nodeID, transform, nodeName)
{
}

EmptyNode::~EmptyNode()
= default;

void EmptyNode::Update(std::vector<SceneObjectData>& meshesToRender)
{
	SceneGraphNode::Update(meshesToRender);
}

SceneGraph::SceneGraph(Transform rootNodeTransform, std::string rootNodeName): m_NodeIDCounter(0), m_SceneGraphRootNode(-1, m_NodeIDCounter, rootNodeTransform, rootNodeName)
{
	m_SceneGraphNodes[0] = &m_SceneGraphRootNode;
}

SceneGraph::~SceneGraph()
{
}

void SceneGraph::Release()
{
	m_SceneGraphRootNode.Destroy();
}

void SceneGraph::Update(std::vector<SceneObjectData>& meshesToRender)
{
	m_SceneGraphRootNode.Update(meshesToRender);
}

void SceneGraphNode::AddChild(SceneGraphNode* pNode)
{
	m_NodeFirstChildren.push_back(pNode);
}

int SceneGraphNode::GetNodeID()
{
	return m_NodeID;
}

int SceneGraphNode::GetParentNodeID()
{
	return m_ParentNodeID;
}

SceneGraphNode SceneGraphNode::GetChild(int nodeId)
{
	for (SceneGraphNode* element : m_NodeFirstChildren)
	{
		if (nodeId == element->m_NodeID)
		{
			return *element;
		}
	}

	for (SceneGraphNode* element : m_NodeFirstChildren)
	{
		if (element->GetChild(nodeId).m_NodeID >= 0)
		{
			return *element;
		}
	}

	return SceneGraphNode();
}

SceneGraphNode SceneGraphNode::GetFirstChild(int index)
{
	return *m_NodeFirstChildren[index];
}

SceneGraphNode SceneGraphNode::GetChild(std::string nodeName)
{
	for (SceneGraphNode* element : m_NodeFirstChildren)
	{
		if(nodeName == element->m_NodeName)
		{
			return *element;
		}
	}

	for (SceneGraphNode* element : m_NodeFirstChildren)
	{
		if (element->GetChild(nodeName).m_NodeID >= 0)
		{
			return *element;
		}
	}

	return SceneGraphNode();
}

Transform& SceneGraphNode::GetNodeTransform()
{
	return m_NodeTransform;
}

void SceneGraph::AddChild(SceneGraphNode* pNode)
{
	m_SceneGraphRootNode.AddChild(pNode);
	m_SceneGraphNodes[pNode->GetNodeID()];
}

int& SceneGraph::GetNodeIDCounter()
{
	return m_NodeIDCounter;
}

SceneGraphNode SceneGraph::GetChild(int nodeId)
{
	return m_SceneGraphRootNode.GetChild(nodeId);
}

SceneGraphNode SceneGraph::GetFirstChild(int index)
{
	return m_SceneGraphRootNode.GetFirstChild(index);
}

SceneGraphNode SceneGraph::GetChild(std::string nodeName)
{
	return m_SceneGraphRootNode.GetChild(nodeName);
}

