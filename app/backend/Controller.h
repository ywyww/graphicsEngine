#include "Objects/Scene/Groups.h"
#include "Interfaces/Object.h"
#include "Types/Modes.h"
#include "Types/Data.h"
#include "Helpers/Translator.h"
#include "Objects/Polyline.h"
#include "Model.h"
#include <SDL2/SDL_events.h>
#include <map>

#ifndef CONTROLLER_H
#define CONTROLLER_H

class Controller    // unique coordinates.
{
    Model* model;

    float x, y;             // absoluteCoordinates
    float glX, glY;         // coordinates inside a viewPort (GL-like: eg -1 < x < 1)
    float belongX;      // coordinates inside a viewPort (modified)
    float belongY;
    
    float lastMouseClickedX;
    float lastMouseClickedY;

    float x1, y1, x2, y2;       // coordinates to draw a line.

    bool isCursorVirginClicked = false;
    bool mouseDown = false;
    bool isCursorInRenderArea = false;


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

    void createObject(const float& x1, const float& y1, const float& x2, const float& y2);

    void processEvent(SDL_Event& event, const float& wWidth, const float& wHeight);

};

#endif
