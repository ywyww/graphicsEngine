#include "Objects/Scene/Scene.h"

#ifndef CONTROLLER_H
#define CONTROLLER_H

class Controller
{
    SceneNamespace::Scene* scene;


    
public:
    Controller(Scene* scene)
    {
        this->scene = scene;
    }

    static void render()
    {

    }

    void draw()
    {

    }

};


#endif