#include "Objects/Scene/Groups.h"
#include "Interfaces/Object.h"
#include "Types/Modes.h"
#include "Types/Data.h"
#include "Helpers/Translator.h"
#include "Objects/Polyline.h"
#include "Model.h"
#include <map>

#ifndef CONTROLLER_H
#define CONTROLLER_H

class Controller    // unique coordinates.
{
    Model* model;

private:
            
    Line* createLine(const float& x1, const float& y1, const float& x2, const float& y2);
    Polyline* createPolyline();

public:
    Controller(Model* model);
    ~Controller();

    NodeGroup* isObjectInSpace(const float& x, const float& y);    // check if object in space

    void trySetActiveNode(float lastClickedX, float lastClickedY);

    void translateObject(float relX, float relY);      // now only lines, then all objects.
    void rotateObject(float relX, float relY);

    void addLine(const float& x1, const float& y1, const float& x2, const float& y2);
    
    void addLineInPolyline(Polyline* polyline, const float& x1, const float& y1);


    // delete these:
    void drawLines();
    void drawPolyLines();
};

#endif
