#include "../../include/imgui_impl_sdl2.h"
#include "../../include/imgui_impl_opengl3.h"
#include "../../include/imgui.h"
#include "../backend/Controller.h"
#include "../backend/Helpers/Translator.h"

#ifndef RENDERER_H
#define REDNERER_H

class Renderer {
    Controller* controller;
    Model* model;

    LineInputData* lineInput;

public:
    Renderer(Model* model, Controller* controller);

    // IMGUI
    void drawStatusBar(const float& x, const float& y);
    void drawSceneTree();
    void drawModes();
    void drawObjectPallete();

    // GL
    void draw();
    void drawLines();
    void drawPolylines();

private:
    void drawLineTransformation(Line* line);


};

#endif