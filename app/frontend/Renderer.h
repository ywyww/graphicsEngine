#include "../../include/imgui_impl_sdl2.h"
#include "../../include/imgui_impl_opengl3.h"
#include "../../include/imgui.h"
#include "../backend/Controller.h"
#include "../backend/Helpers/Translator.h"

#ifndef RENDERER_H
#define REDNERER_H

class Renderer {
    Controller& controller;
    Model& model;

    LineInputData* lineInput;


private:
    void drawSceneTreePoints();
    void drawSceneTreeLines();
    void drawSceneTreePolylines();
    void drawSceneTreeGroups();
    
public:
    Renderer(Model& model, Controller& controller);

    // IMGUI
    void drawStatusBar();

    void drawSceneTree();
    void drawModes();
    void drawSettings();
    void drawViewState();
    void drawProjectionState();

    // GL
    void draw();
    void drawPoints();
    void drawLines();
    void drawPolylines();

private:
    void drawLineTransformation(Line* line);


};

#endif