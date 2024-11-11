#include <string>
#include <vector>

#include <glm/glm.hpp>
#include "../GL/Line.h"

#ifndef LINE_GROUP_H
#define LINE_GROUP_H

template<class NodeType>
struct NodeGroup
{
    NodeType* node;
    std::string name;

    NodeGroup()
    {
        node = nullptr;
        name = "";
    }
    NodeGroup& operator=(const NodeGroup<NodeType>& other){
        if (this == &other)
            return *this;

        this->node = other.node;
        this->name = other.name;

        return *this;
    }
    virtual ~NodeGroup() = default;
};

template<class NodeType>
struct TransformationNodeGroup: public NodeGroup<NodeType>
{
    glm::mat4x4 transformation;
};

typedef std::vector<NodeGroup<Line>> Lines;


#endif


