#include <string>

#ifndef NODE_H
#define NODE_H

namespace SceneNamespace
{
    template <class Object>
    struct Node
    {
        Object* node;
        std::string name;

        Node(Object* node, std::string name)
        {
            this->node = node;
            this->name = name;
        }
    };
}


#endif