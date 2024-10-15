#include <iostream>
#include <vector>

#include "Groups.h"

#ifndef SCENE_H
#define SCENE_H

namespace SceneNamespace
{
    class Scene
    {
        Lines lines;
        std::string linesName;

    public:
        Scene();
        void addLine(Line* line);
        void drawLines();


    };
}
#endif