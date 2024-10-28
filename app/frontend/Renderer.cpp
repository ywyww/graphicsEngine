#include "Renderer.h"

Renderer::Renderer(Controller* controller)
{
    this->controller = controller;
}

void Renderer::drawStatusBar(const float& x, const float& y)
{
    ImGui::Begin("StatusBar");
    std::string cursor = std::to_string(x) + " " + std::to_string(y);
    ImGui::Text(cursor.c_str());
    ImGui::End();
}

void Renderer::drawSceneTree()
{
    ImGui::Begin("Scene Tree");

    Lines* lines = &controller->getLines();
       

    if (ImGui::BeginMenu("Lines"))
    {   
        for (int i = 0; i < lines->size(); i++)
        {
            Line* line = lines->operator[](i).node;
            const char* lineName = (std::to_string(i) + ": " + lines->operator[](i).name).c_str();

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

void Renderer::drawLineTransformation(Line* line)
{
    if (ImGui::BeginMenu("Rotation"))
    {
        float* angle = &controller->getLineInput()->angle;

        if (ImGui::InputFloat("Rotation coeff", angle))
        {
            glm::mat4x4 transform = line->getTransformation();
            transform = glm::rotate(transform, *angle, glm::vec3(0.0f, 0.0f, 1.0f));
            line->setTransformation(transform);
        }
        ImGui::EndMenu();
    }

    if (ImGui::BeginMenu("Translation"))
    {
        float* translation = controller->getLineInput()->translation;
        ImGui::InputFloat3("Translations: x, y, z", translation);

        if (ImGui::Button("Click me for the translation"))
        {
            glm::mat4x4 transform = line->getTransformation();
            transform = glm::translate(transform, glm::vec3(translation[0], translation[1], translation[2]));
            line->setTransformation(transform);
        }

        ImGui::EndMenu();
    }

    if (ImGui::Button("Reset"))
    {
        line->setTransformation(glm::mat4x4(1.0f));
    }
}

void Renderer::drawLineCreation(int width, int height)
{
    float* coordinates = controller->getLineInput()->coordinates;
    ImGui::Begin("Create line");
        ImGui::InputFloat("x1", &coordinates[0]);
        ImGui::InputFloat("y1", &coordinates[1]);
        ImGui::InputFloat("z1", &coordinates[2]);
        ImGui::InputFloat("x2", &coordinates[3]);
        ImGui::InputFloat("y2", &coordinates[4]);
        ImGui::InputFloat("z2", &coordinates[5]);

    if (ImGui::Button("Go"))
    {
        Line* line = new Line(
            controller->producePixelCoordinatesToGL(coordinates[0], width),
            controller->producePixelCoordinatesToGL(coordinates[1], height),
            coordinates[2],
            controller->producePixelCoordinatesToGL(coordinates[3], width),
            controller->producePixelCoordinatesToGL(coordinates[4], height),
            coordinates[5]
        );
        controller->getScene()->addLine(line);
    }
    ImGui::End();
}

void Renderer::drawPointBelongWindow(const float& x, const float& y)
{
    // hard code
    ImGui::Begin("Create line");

    if (ImGui::Button("Check"))
    {
        
        Lines lines = controller->getLines();

        if (lines.size() != 0)
        {
            Line* lineToCheck = lines[0].node;


            if (lineToCheck->isPointBelongs(x, y, 0))
                std::cout << "WORK!" << std::endl;
            else 
                std::cout << "NOT WORK!" << std::endl;
        }
        else
        {
            std::cout << "ADD 1 element.";
        }
    }
    ImGui::End();
}

void Renderer::draw()
{
    controller->getScene()->drawLines();
}