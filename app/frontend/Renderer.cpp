#include "Renderer.h"


Renderer::Renderer(Controller* controller, const float& width, const float& height)
{
    this->controller = controller;
    wWidth = width;
    wHeight = height;
}

void Renderer::drawStatusBar(const float& x, const float& y, const float& lastClickedX, const float& lastClickedY)
{
    // add data here

    ImGui::Begin("StatusBar");
        std::string cursor = std::to_string(x) + " " + std::to_string(y);
        ImGui::Text(cursor.c_str());

        ImGui::Text("Active mode: %s", controller->modeMap[controller->getMode()]);

        ImGui::BeginChild("Line chosen");


            NodeGroup* activeNode = controller->getActiveNode();
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
            NodeGroup* node = &lines->operator[](i);

            Line* line = dynamic_cast<Line*>(node);

            // beginMenu has own name. It shouldn't change. So:
            //const char* lineMenuName = ("Line #" + std::to_string(i + 1) + ": " + node->name.c_str()).c_str();  /
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
                    controller->setActiveNode(node);
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
    if (ImGui::BeginMenu("ColorPicker"))
    {
        float* color = controller->getLineInput()->color;
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

void Renderer::drawLineCreation()
{
    if (controller->getMode() == WorkModes::DRAW_LINE)
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
                controller->producePixelCoordinatesToGL(coordinates[0], wWidth),
                controller->producePixelCoordinatesToGL(coordinates[1], wHeight),
                coordinates[2],
                controller->producePixelCoordinatesToGL(coordinates[3], wWidth),
                controller->producePixelCoordinatesToGL(coordinates[4], wHeight),
                coordinates[5]
            );
            controller->addLine(line);
        }
        ImGui::End();
    }
}

void Renderer::drawModes()
{
    ImGui::Begin("Modes");

    std::map<WorkModes, const char*> map = controller->modeMap;
    
    for (auto iter = map.begin(); iter != map.end(); iter++)
    {
        if (ImGui::Button(iter->second))
        {
            controller->setMode(iter->first);
        }
    }

    ImGui::End();
}

void Renderer::draw()
{
    controller->drawLines();
}

void Renderer::setActiveNode(float lastClickedX, float lastClickedY)
{
    if (controller->getMode() == WorkModes::POINTER)
        controller->setActiveNode(lastClickedX, lastClickedY, wWidth, wHeight);
}

void Renderer::translateObject(float relX, float relY)    // border
{
    NodeGroup* node = controller->getActiveNode();

    if (controller->getMode() == WorkModes::TRANSLATE && node != nullptr)
    {
        float border = 100;
        
        if (relX > border)
            relX = border;
        else if (relX < -border)
            relX = -border;
        if (relY > border)
            relY = border;
        else if (relY < -border)
            relY = -border;

        float glXRel = 2 * relX / wWidth;
        float glYRel = 2 * relY / wHeight;

        glm::mat4 transformation = node->node->getTransformation();
        transformation = glm::translate(transformation, glm::vec3(glXRel, glYRel, 0.0f));
        node->node->setTransformation(transformation);
    }
}

void Renderer::rotateObject(float relX, float relY)    // border
{
    NodeGroup* node = controller->getActiveNode();

    if (controller->getMode() == WorkModes::ROTATE && node != nullptr)
    {
        float border = 100;
        
        if (relX > border)
            relX = border;
        else if (relX < -border)
            relX = -border;
        if (relY > border)
            relY = border;
        else if (relY < -border)
            relY = -border;

        float glXRel = 2 * relX / wWidth;
        float glYRel = 2 * relY / wHeight;

        glm::mat4 transformation = node->node->getTransformation();
        transformation = glm::rotate(transformation, glm::radians(std::atan(glXRel/glYRel)), glm::vec3(0.0f, 1.0f, 0.0f));
        node->node->setTransformation(transformation);
    }
}