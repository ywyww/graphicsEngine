#include <SFML/Graphics/Shape.hpp>
#include <vector>
#include <string>

#include "app/backend/Interfaces/ISceneObjects.h"

#ifndef SCENE_NODE_H
#define SCENE_NODE_H

namespace SceneNamespace
{

    template <class Object>
    class SceneGroup: public ISceneObjects<Object>
    {
    protected:
        std::vector<Node<Object>> objects;
        std::string groupName;
        int objectCounter;

    public:

        SceneGroup()
        {
            objects = std::vector<Node<Object>>();
            objectCounter = 0;
            groupName = "";
        }

        SceneGroup(std::string name): SceneGroup()
        {
            groupName = name;
        }

        Node<Object> getNode(int position) override
        {
            return objects[position];
        }

        void addNode(Node<Object> node) override
        {
            objects.push_back(node);
            objectCounter++;
        }

        void deleteNode(Node<Object> Node) override
        {
            // todo
        }

        void deleteNode(int position) override
        {
            // todo
        }

        void clearNodes() override
        {
            objects.clear();
        }

        void setName(std::string name)
        {
            groupName = name;
        }

        std::string getName()
        {
            return groupName;
        }

        int getCounter()
        {
            return objectCounter;
        }
    };
}

#endif