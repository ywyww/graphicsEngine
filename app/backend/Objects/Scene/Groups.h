#include <string>
#include <vector>

#include "GL/Lines.h"

#ifndef LINE_GROUP_H
#define LINE_GROUP_H

template<class NodeType>
struct NodeGroup
{
    NodeType* node;
    std::string name;
};

typedef std::vector<NodeGroup<Line>> Lines;

#endif

