#include "../../include/imgui_impl_sdl2.h"
#include "../../include/imgui_impl_opengl3.h"
#include "../../include/imgui.h"

#ifndef RENDERER_H
#define REDNERER_H

class Renderer {
    SceneNamespace::Scene* scene;
    float* lineCoordinates;
public:

    Renderer(SceneNamespace::Scene& scene)
    {
       this->scene = &scene; 
       lineCoordinates = new float[6]{0, 0, 0, 0, 0, 0};
    }

    void status(const float& x, const float& y)
    {
        ImGui::Begin("StatusBar");
        std::string cursor = std::to_string(x) + " " + std::to_string(y);
        ImGui::Text(cursor.c_str());
        ImGui::End();
    }

    void sceneInfo()    // make for all scene   // can i NOT render this many times?
    {
        ImGui::Begin("Properties");

        Lines lines = scene->getLines();

        ImGui::BeginChild("Lines");
        for (int i = 0; i < lines.size(); i++)
        {
            ImGui::Text("line %s", lines[i].name.c_str());
            
            float* buffer = lines[i].node->getBuffer();
            for (int j = 0; j < 6; j++)
            {
                ImGui::Text("%f", buffer[j]);
            }
            ImGui::Text("End----");
        }
        ImGui::EndChild();

        ImGui::End();
    }

    void createLine(bool& flag)
    {
        ImGui::Begin("Create line");
            ImGui::InputFloat("x1", &lineCoordinates[0]);
            ImGui::InputFloat("y1", &lineCoordinates[1]);
            ImGui::InputFloat("z1", &lineCoordinates[2]);

            ImGui::InputFloat("x2", &lineCoordinates[3]);
            ImGui::InputFloat("y2", &lineCoordinates[4]);
            ImGui::InputFloat("z2", &lineCoordinates[5]);


        if (ImGui::Button("Go"))
        {
            Line* line = new Line(
                lineCoordinates[0],
                lineCoordinates[1],
                lineCoordinates[2],
                lineCoordinates[3],
                lineCoordinates[4],
                lineCoordinates[5]
            );
            scene->addLine(line);
        }
        ImGui::End();
    }

    void modes()
    {

    }

};

#endif