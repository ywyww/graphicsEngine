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
};

template<class NodeType>
struct TransformationNodeGroup: public NodeGroup<NodeType>
{
    glm::mat4x4 transformation;
};

typedef std::vector<NodeGroup<Line>> Lines;


#endif


