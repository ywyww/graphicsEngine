#include "../../include/imgui_impl_sdl2.h"
#include "../../include/imgui_impl_opengl3.h"
#include "../../include/imgui.h"
#include "../backend/Controller.h"

#ifndef RENDERER_H
#define REDNERER_H

class Renderer {
    Controller* controller;

public:
    Renderer(Controller* controller);

    // IMGUI
    void drawStatusBar(const float& x, const float& y);
    void drawSceneTree();
    void drawLineCreation(int width, int height);  // glViewport width and height


    // GL
    void draw();

private:
    void drawLineTransformation(Line* line);


};

#endif