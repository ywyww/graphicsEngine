#include "../../include/imgui_impl_sdl2.h"
#include "../../include/imgui_impl_opengl3.h"
#include "../../include/imgui.h"

#ifndef RENDERER_H
#define REDNERER_H

class Renderer {
    SceneNamespace::Scene* scene;
    float* lineCoordinates;

    // transformation coefficents
    float* translationValues;
public:

    Renderer(SceneNamespace::Scene& scene)
    {
       this->scene = &scene; 
       lineCoordinates = new float[6] {0, 0, 0, 0, 0, 0};
       translationValues = new float[3] {0, 0, 0};
    }

    ~Renderer()
    {
        delete[] lineCoordinates;
        delete[] translationValues;
    }

    void drawStatusBar(const float& x, const float& y)
    {
        ImGui::Begin("StatusBar");
        std::string cursor = std::to_string(x) + " " + std::to_string(y);
        ImGui::Text(cursor.c_str());
        ImGui::End();
    }

    void drawSceneTree()
    {
        ImGui::Begin("Scene Tree");

        Lines lines = scene->getLines();
        

        if (ImGui::BeginMenu("Lines"))
        {   
            for (int i = 0; i < lines.size(); i++)
            {
                Line* line = lines[i].node;
                const char* lineName = (std::to_string(i) + ": " + lines[i].name).c_str();

                if (ImGui::BeginMenu(lineName))
                {
                    drawLineTransformation(line);
                    
                    ImGui::Text(lineName);

                    ImGui::EndMenu();
                }
            }

            ImGui::EndMenu();
        }

        ImGui::End();
    }

    void drawLineTransformation(Line* line)
    {
        if (ImGui::BeginMenu("Rotation"))
        {
            float data = 0.0f;
            if (ImGui::InputFloat("Rotation coeff", &data))
            {
                glm::mat4x4 transform = line->getTransformation();
                transform = glm::rotate(transform, data, glm::vec3(1.0f, 0.3f, 1.0f));
                line->setTransformation(transform);
            }
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Translation"))
        {
            ImGui::InputFloat3("Translations: x, y, z", translationValues);

            if (ImGui::Button("Click me for the translation"))
            {
                glm::mat4x4 transform = line->getTransformation();
                transform = glm::translate(transform, glm::vec3(translationValues[0], translationValues[1], translationValues[2]));
                line->setTransformation(transform);
            }

            ImGui::EndMenu();
        }

        // add other menu

        if (ImGui::Button("Reset"))
        {
            line->setTransformation(glm::mat4x4(1.0f));
        }
    }

    void drawLineInfo()
    {
        // TODO
        // dynamic VBO
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