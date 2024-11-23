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

    float cursorAbsX, cursorAbsY;   // cursor coordinates in window (absolute)
    float cursorX, cursorY;         // cursor coordinates in viewPort
    float cursorGlX, cursorGlY;     // coordinates inside a viewPort (GL-like: eg -1 < x < 1)
    
    float lastMouseDownX, lastMouseDownY;   // cursor coordinates in viewPort
    float lastMouseUpX, lastMouseUpY;       // cursor coordinates in viewPort

    bool isCursorVirginClicked;
    bool isMouseDown;
    bool isCursorInRenderArea;

    bool isLineModifable;
    
    int xModifableIdx, yModifableIdx;

public:
    Line* rubberThread;
    bool rubberDrawable;


private:
            
    Line* createLine(const float& x1, const float& y1, const float& x2, const float& y2);
    Polyline* createPolyline(const float& x0, const float& y0);

public:
    // after adding set object to active node
    Controller(Model* model);
    ~Controller();


    NodeGroup* isObjectInSpace(const float& x, const float& y);    // check if object in space

    void trySetActiveNode(float lastClickedX, float lastClickedY);

    void translateObject(float relX, float relY);      // now only lines, then all objects.
    void rotateObject(float relX, float relY);

    void addLine(const float& x1, const float& y1, const float& x2, const float& y2);
    void addPolyline(const float& x0, const float& y0);

    void modifyLine(const float& precision);
    
    void addDotInActivePolyline(const float& x1, const float& y1);

    void processEvent(SDL_Event& event, const float& wWidth, const float& wHeight);
};

#endif
