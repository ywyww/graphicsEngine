#include <string>
#include <vector>

#include <glm/glm.hpp>
#include "../GL/Line.h"
#include "../../Interfaces/Object.h"

#ifndef LINE_GROUP_H
#define LINE_GROUP_H

struct NodeGroup
{
    Object* node;
    std::string name;

    NodeGroup()
    {
        node = nullptr;
        name = "";
    }
    
    NodeGroup& operator=(const NodeGroup other){
        if (this == &other)
            return *this;

        this->node = other.node;
        this->name = other.name;

        return *this;
    }
    virtual ~NodeGroup() = default;
};

typedef std::vector<NodeGroup> Lines;

#endif