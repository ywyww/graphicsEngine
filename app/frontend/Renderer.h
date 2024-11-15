#include "../../include/imgui_impl_sdl2.h"
#include "../../include/imgui_impl_opengl3.h"
#include "../../include/imgui.h"
#include "../backend/Controller.h"

#ifndef RENDERER_H
#define REDNERER_H

class Renderer {
    Controller* controller;

    float wWidth;
    float wHeight;

public:
    Renderer(Controller* controller, const float& width, const float& height);

    // IMGUI
    void drawStatusBar(const float& x, const float& y, const float& lastClickedX, const float& lastClickedY);
    void drawSceneTree();
    void drawLineCreation();  // glViewport width and height
    void drawModes();


    // GL
    void draw();


    // events
    void setActiveNode(float lastClickedX, float lastClickedY);

    void translateObject(float relX, float relY);      // now only lines, then all objects.
    void rotateObject(float relX, float relY);

    void createLine(const float& x1, const float& y1, const float& x2, const float& y2);

private:
    void drawLineTransformation(Line* line);


};

#endif