#include "../../include/imgui_impl_sdl2.h"
#include "../../include/imgui_impl_opengl3.h"
#include "../../include/imgui.h"
#include "../backend/Controller.h"
#include "../backend/Translator.h"

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
    void drawModes();
    void drawObjectPallete();

    // GL
    void draw();


    // events

private:
    void drawLineTransformation(Line* line);


};

#endif