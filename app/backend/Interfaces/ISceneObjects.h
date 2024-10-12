#include "app/backend/Types/Node.h"

#ifndef ISCENE_OBJECTS_H
#define ISCENE_OBJECTS_H

namespace SceneNamespace 
{
    
    template <class InterfaceType>
    class ISceneObjects
    {
    public:
        virtual Node<InterfaceType> getNode(int position) = 0;
        virtual void addNode(Node<InterfaceType> node) = 0;
        virtual void deleteNode(Node<InterfaceType> node) = 0;
        virtual void deleteNode(int NodePosition) = 0;
        virtual void clearNodes() = 0;
    };

}

#endif