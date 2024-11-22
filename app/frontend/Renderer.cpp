#include "Renderer.h"


Renderer::Renderer(Model* model, Controller* controller)
{
    this->model = model;
    this->controller = controller;
    lineInput = new LineInputData();
    std::cout << "HELLO WORLD!" << std::endl;
}

void Renderer::drawStatusBar()
{
    float x = model->getCursorX();
    float y = model->getCursorY();

    ImGui::Begin("StatusBar");
        std::string cursor = std::to_string(x) + " " + std::to_string(y);
        ImGui::Text(cursor.c_str());

        NodeGroup* currentNode = controller->isObjectInSpace(x, y);
        if (currentNode != nullptr)
            ImGui::Text("Pointer is at the object: %s", currentNode->name.c_str());     // here

        ImGui::Text("Active mode: %s", model->modeMap[model->getMode()]);

        ImGui::BeginChild("Object chosen");
            NodeGroup* activeNode = model->getActiveNode();
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

    Nodes* lines = &model->getLines();
    

    if (ImGui::BeginMenu("Lines"))
    {   
        for (int i = 0; i < lines->size(); i++)
        {
            NodeGroup* node = &lines->operator[](i);

            Line* line = dynamic_cast<Line*>(node->node);

            if (line == nullptr)
            {
                // "bad cast: object->line"
                throw std::bad_cast();
            }

            // beginMenu has own name. It shouldn't change. So:
            const char* lineMenuName = ("Line #" + std::to_string(i + 1)).c_str();

            if (ImGui::BeginMenu(lineMenuName))
            {
                ImGui::Text("Line name: %s", node->name.c_str());

                ImGui::InputText("Line name", lineInput->lineName, lineInput->lineNameSize);
                if (ImGui::Button("Change name"))
                {
                    node->name = std::string(lineInput->lineName, lineInput->lineNameSize);
                }

                drawLineTransformation(line);

                if (ImGui::Button("Set active"))
                {
                    model->setActiveNode(node);
                }

                if (ImGui::Button("Delete line"))
                {
                    if (!model->deleteLine(i))
                        std::cout << "cannot delete";                    
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
        float* angle = &lineInput->angle;

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
        float* translation = lineInput->translation;
        ImGui::InputFloat3("Translations: x, y, z", translation);

        if (ImGui::Button("Click me for the translation"))
        {
            glm::mat4x4 transform = line->getTransformation();
            transform = glm::translate(transform, glm::vec3(translation[0], translation[1], translation[2]));
            line->setTransformation(transform);
        }

        ImGui::EndMenu();
    }
    if (ImGui::BeginMenu("ColorPicker"))
    {
        float* color = lineInput->color;
        ImGui::InputFloat3("Colors: rgb", color);

        if (ImGui::Button("Click me for change color"))
        {
            glm::vec3 colorVec = glm::vec3(color[0], color[1], color[2]);
            line->setColor(colorVec);
        }
        ImGui::EndMenu();
    }
    if (ImGui::Button("Reset"))
    {
        line->setTransformation(glm::mat4x4(1.0f));
        line->setColor(glm::vec3(1.0f));
    }
}

void Renderer::drawModes()
{
    ImGui::Begin("Modes");

    std::map<WorkModes, const char*> map = Model::modeMap;
    
    for (auto iter = map.begin(); iter != map.end(); iter++)
    {
        if (ImGui::Button(iter->second))
        {
            model->setMode(iter->first);
        }
    }

    ImGui::End();
}

void Renderer::drawObjectPallete()
{
    ImGui::Begin("Creation modes");
    std::map<ObjectCreationModes, const char*> map = Model::modeCreationMap;

    for (auto iter = map.begin(); iter != map.end(); iter++)
    {
        if (ImGui::Button(iter->second))
        {
            model->setCreationMode(iter->first);
        }
    }

    ImGui::End();
}

void Renderer::draw()
{
    drawLines();
    drawPolylines();
}

void Renderer::drawLines()
{
    Nodes lines = model->getLines();
    for (int i = 0; i < lines.size(); i++)
    {
        lines[i].node->draw();
    }
}

void Renderer::drawPolylines()
{
    Nodes lines = model->getPolylines(); 
    for (int i = 0; i < lines.size(); i++)
    {
        Polyline* line = dynamic_cast<Polyline*>(lines[i].node);
            
        if (line != nullptr)
            line->draw();
        else
            std::cout << "ERROR IN DRAW POLYLINE.";
    }
}
