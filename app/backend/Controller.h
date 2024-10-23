#include "Objects/Scene/Scene.h"

#ifndef CONTROLLER_H
#define CONTROLLER_H

    struct LineInputData
    {
        float* coordinates; // 6 (x, y, z) * 2

        float* translation; // 3 x y z
        float* rotation; // 3 x y z
        float angle;

    public:
        LineInputData();
        ~LineInputData();
    };

    class Controller
    {
        SceneNamespace::Scene* scene;

        LineInputData* lineInputData; 

    public:
        Controller();
        Controller(SceneNamespace::Scene* scene);
        ~Controller();

        SceneNamespace::Scene* getScene();
        Lines& getLines();
        LineInputData* getLineInput();
        
        
        float producePixelCoordinatesToGL(float coord, int dimension);
    };

#endif
