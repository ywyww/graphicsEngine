#include "Objects/Scene/Scene.h"

#ifndef CONTROLLER_H
#define CONTROLLER_H

class Controller
{
    SceneNamespace::Scene scene;
    std::vector<bool> LinesState;
    
public:
    Controller()
    {
        scene = SceneNamespace::Scene();
    }

    static void addLine()
    {

    }

    void draw()
    {

    }

};


#endif