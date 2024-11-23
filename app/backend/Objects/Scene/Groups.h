#include <string>
#include <vector>

#include <glm/glm.hpp>
#include "../GL/Line.h"
#include "../../Interfaces/Object.h"

#ifndef LINE_GROUP_H
#define LINE_GROUP_H

enum ObjectType {
    NULLTYPE,
    POINT,
    LINE,
    POLYLINE
};

struct NodeGroup
{
    Object* node;
    std::string name;
    ObjectType type;

    NodeGroup()
    {
        node = nullptr;
        name = "";
        type = NULLTYPE;
    }
    
    NodeGroup& operator=(const NodeGroup other){
        if (this == &other)
            return *this;

        this->node = other.node;
        this->name = other.name;
        this->type = other.type;

        return *this;
    }
    virtual ~NodeGroup() = default;
};

typedef std::vector<NodeGroup> Nodes;

#endif