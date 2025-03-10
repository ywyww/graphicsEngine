#include <SDL2/SDL_events.h>
#include <map>

#include "Helpers/Translator.h"
#include "Types/Modes.h"
#include "Types/Data.h"

#include "Interfaces/Object.h"
#include "Objects/Scene/Groups.h"
#include "Objects/Polyline.h"

#include "Model.h"


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
    bool isPolylineCreationMode;
    
    int xModifableIdx, yModifableIdx;

    float scaleX, scaleY;


public:
    Line* rubberThread;
    bool rubberDrawable;
    Point centerPoint;

private:
    
    NodeGroup* isPointInSpace(const float& x, const float& y);  // implement
    NodeGroup* isLineInSpace(const float& x, const float& y);
    NodeGroup* isPolylineInSpace(const float& x, const float& y);

    Point* createPoint(const float& x, const float& y);
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
    void scaleObject(float relX, float relY);
    void mirrorObject(float lastUpX, float lastUpY);

    void projectionObject(float lastUpX, float lastUpY);        // implement
    
    void addPoint(const float& x, const float& y);
    void addLine(const float& x1, const float& y1, const float& x2, const float& y2);
    void addPolyline(const float& x0, const float& y0);

    bool setIfLineModifable(const float& precision);
    void modifyLine();
    
    void addDotInActivePolyline(const float& x1, const float& y1);

    void processRubberThread();

    void processEvent(SDL_Event& event, const float& wWidth, const float& wHeight);
};

#endif
