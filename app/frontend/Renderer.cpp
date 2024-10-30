#include "Renderer.h"


Renderer::Renderer(Controller* controller)
{
    this->controller = controller;
}

void Renderer::drawStatusBar(const float& x, const float& y, const float& lastClickedX, const float& lastClickedY)
{
    // add data here

    ImGui::Begin("StatusBar");
    std::string cursor = std::to_string(x) + " " + std::to_string(y);
    ImGui::Text(cursor.c_str());

    ImGui::BeginChild("Line chosen");
        controller->setActiveNode(lastClickedX, lastClickedY);
        
        NodeGroup<Object>* activeNode = controller->getActiveNode();
        if (activeNode == nullptr)
            ImGui::Text("No object chosen");
        else
            ImGui::Text("Chosen: %s", activeNode->name.c_str());     // here
    ImGui::EndChild();
    
    ImGui::End();
}

void Renderer::drawSceneTree()
{
    ImGui::Begin("Scene Tree");

    Lines* lines = &controller->getLines();
       

    if (ImGui::BeginMenu("Lines"))
    {   
        LineInputData* lineInput = controller->getLineInput();
        for (int i = 0; i < lines->size(); i++)
        {
            NodeGroup<Line>* node = &lines->operator[](i);
            Line* line = node->node;

            // beginMenu has own name. It shouldn't change. So:
            //const char* lineMenuName = ("Line #" + std::to_string(i + 1) + ": " + node->name.c_str()).c_str();  /
            const char* lineMenuName = ("Line #" + std::to_string(i + 1)).c_str();

            if (ImGui::BeginMenu(lineMenuName))
            {
                ImGui::Text("Line name: %s", node->name.c_str());
                
                drawLineTransformation(line);

                ImGui::InputText("Line name", lineInput->lineName, lineInput->lineNameSize);

                if (ImGui::Button("Change name"))
                {
                    node->name = std::string(lineInput->lineName, lineInput->lineNameSize);
                }

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
        controller->addLine(line);
    }
    ImGui::End();
}

void Renderer::draw()
{
    controller->drawLines();
}