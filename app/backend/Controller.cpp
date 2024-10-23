#include "Controller.h"

    Controller::Controller()
    {
        lineInputData = LineInputData();
    }

    Controller::Controller(SceneNamespace::Scene* scene)
    {
        lineInputData = LineInputData();
        this->scene = scene;
    }

    Lines& Controller::getLines()
    {
        return scene->getLines();
    }

    SceneNamespace::Scene* Controller::getScene()
    {
        return scene;
    }

    LineInputData& Controller::getLineInput()
    {
        return lineInputData;
    }

    float Controller::producePixelCoordinatesToGL(float coord, int dimension)
    {
        return coord * 2 / (dimension) - 1;
    }




    LineInputData::LineInputData()
    {
        coordinates = new float[6] {.0f, .0f, .0f, .0f, .0f, .0f};
        translation = new float[3] {.0f, .0f, .0f};
        rotation = new float[3] {.0f, .0f, .0f};
        angle = 0.0f;
    }
    LineInputData::~LineInputData()
    {
        delete[] coordinates;
        delete[] translation;
        delete[] rotation;
    }