#include <iostream>
#include <vector>

#include "Groups.h"

#ifndef SCENE_H
#define SCENE_H

namespace SceneNamespace
{
    class Scene
    {
        // polyline as std::vector<Lines>
        Lines lines;
        std::string linesName;

        NodeGroup<Object>* activeNode;

    public:
        Scene();
        
        void addLine(Line* line);

        Lines& getLines();

        void drawLines();
    };
}
#endif