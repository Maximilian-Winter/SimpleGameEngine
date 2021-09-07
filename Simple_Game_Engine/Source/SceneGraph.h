#ifndef _SCENEGRAPH_H_
#define _SCENEGRAPH_H

#include <d3d12.h>
#include <map>
#include <string>
#include <vector>
#include <wrl/client.h>


#include "SceneObjectData.h"
#include "StaticMesh.h"
#include "Transform.h"


class SceneGraphNode
{
public:

    SceneGraphNode();
    SceneGraphNode(const SceneGraphNode&);
    SceneGraphNode(int parentNodeID, int nodeID, Transform transform, std::string nodeName);
    virtual ~SceneGraphNode();
    void Release();

    void AddChild(SceneGraphNode* pNode);

    int GetNodeID();

    int GetParentNodeID();

    SceneGraphNode GetChild(int nodeID);

    SceneGraphNode GetFirstChild(int index);

    SceneGraphNode GetChild(std::string nodeName);

    Transform& GetNodeTransform();

    virtual void Update(std::vector<SceneObjectData>& meshesToRender);

    virtual void Destroy();
protected:
    // list of children
    std::vector<SceneGraphNode*> m_NodeFirstChildren{};
    Transform m_NodeTransform;
    std::string m_NodeName;
    int m_ParentNodeID;
    int m_NodeID;
};


class StaticGeometryNode : public SceneGraphNode
{
public:
    StaticGeometryNode(int parentNodeID, int nodeID, Transform transform, std::string nodeName);
    ~StaticGeometryNode() override;

    void AddStaticMeshToNode(ID3D11Device* device, std::vector<StaticVertex> vertices, std::vector<int> indicies, std::vector<Subset> subsets, float aabbWidth, float aabbHeight, float aabbLength);

    void Update(std::vector<SceneObjectData>& meshesToRender) override;
    void Destroy() override;
private:

    std::vector<StaticMesh> m_StaticGeometry;
};

class EmptyNode : public SceneGraphNode
{
public:
    EmptyNode(int parentNodeID, int nodeID, Transform transform, std::string nodeName);
    ~EmptyNode();

    void Update(std::vector<SceneObjectData>& meshesToRender) override;
};

class SceneGraph
{
public:

    SceneGraph(Transform rootNodeTransform, std::string rootNodeName);
    ~SceneGraph();
    void Release();
   
    void AddChild(SceneGraphNode* pNode);

    int& GetNodeIDCounter();

    SceneGraphNode GetChild(int nodeID);

    SceneGraphNode GetFirstChild(int index);

    SceneGraphNode GetChild(std::string nodeName);

    void Update(std::vector<SceneObjectData>& meshesToRender);
protected:
    EmptyNode m_SceneGraphRootNode;
    int m_NodeIDCounter;
    std::map<int, SceneGraphNode*> m_SceneGraphNodes;
};


#endif
