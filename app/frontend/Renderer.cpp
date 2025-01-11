#include "Renderer.h"


Renderer::Renderer(Model& model, Controller& controller): model(model), controller(controller)
{
    lineInput = new LineInputData();
}

void Renderer::drawStatusBar()
{
    float x = model.getCursorX();
    float y = model.getCursorY();

    ImGui::Begin("StatusBar");
        std::string cursor = std::to_string(x) + " " + std::to_string(y);
        ImGui::Text(cursor.c_str());

        Node* currentNode = controller.isObjectInSpace(x, y);
        if (currentNode != nullptr)
            ImGui::Text("Pointer is at the object: %s", currentNode->name.c_str());     // here

        ImGui::Text("Active mode: %s", model.modeMap[model.getMode()]);

        ImGui::Text("Current view state: %s", Model::viewStateMap[model.getViewState()]);

        ImGui::BeginChild("Object chosen");
            Node* activeNode = model.getActiveNode();
            if (activeNode == nullptr)
                ImGui::Text("No object chosen");
            else
                ImGui::Text("Chosen: %s", activeNode->name.c_str());     // here
        ImGui::EndChild();
    
    ImGui::End();
}

void Renderer::drawSceneTreePoints()
{
    Nodes* points = &model.getPoints();
    

    if (ImGui::BeginMenu("Points"))
    {   
        for (int i = 0; i < points->size(); i++)
        {
            Node* node = &points->operator[](i);

            Point* point = dynamic_cast<Point*>(node->node);

            if (point == nullptr)
            {
                // "bad cast: object->point"
                throw std::bad_cast();
            }

            // beginMenu has own name. It shouldn't change. So:
            const char* lineMenuName = ("Point #" + std::to_string(i + 1)).c_str();

            if (ImGui::BeginMenu(lineMenuName))
            {
                ImGui::Text("Point name: %s", node->name.c_str());

                ImGui::InputText("Line name", lineInput->lineName, lineInput->lineNameSize);
                if (ImGui::Button("Change name"))
                {
                    node->name = std::string(lineInput->lineName, lineInput->lineNameSize);
                }

                if (ImGui::Button("Set active"))
                {
                    model.setActiveNode(node);
                }

                if (ImGui::Button("Delete point"))
                {
                    if (!model.deletePoint(i))
                        std::cout << "cannot delete";                    
                }

                ImGui::EndMenu();
            }
        }

        ImGui::EndMenu();
    }
}
void Renderer::drawSceneTreeLines()
{
    Nodes* lines = &model.getLines();
    

    if (ImGui::BeginMenu("Lines"))
    {   
        for (int i = 0; i < lines->size(); i++)
        {
            Node* node = &lines->operator[](i);

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
                    model.setActiveNode(node);
                }

                if (ImGui::Button("Delete line"))
                {
                    if (!model.deleteLine(i))
                        std::cout << "cannot delete";                    
                }

                ImGui::EndMenu();
            }
        }

        ImGui::EndMenu();
    }
}

void Renderer::drawSceneTreePolylines()
{
    Nodes* polylines = &model.getPolylines();
    

    if (ImGui::BeginMenu("Polylines"))
    {   
        for (int i = 0; i < polylines->size(); i++)
        {
            Node* node = &polylines->operator[](i);

            Polyline* line = dynamic_cast<Polyline*>(node->node);

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

                //drawLineTransformation(line);

                if (ImGui::Button("Set active"))
                {
                    model.setActiveNode(node);
                }

                if (ImGui::Button("Delete line"))
                {
                    if (!model.deletePolyLine(i))
                        std::cout << "cannot delete";                    
                }

                ImGui::EndMenu();
            }
        }

        ImGui::EndMenu();
    }
}

void Renderer::drawSceneTreeGroups()
{
    Groups& groups = model.getGroups();
    
    if (ImGui::BeginMenu("Groups"))
    {   
        for (int i = 0; i < groups.getSize(); i++)
        {
            std::pair<Nodes&, std::string&> groupPair = groups.getGroup(i);
            Nodes& group = groupPair.first;
            std::string& groupName = groupPair.second;

            // beginMenu has own name. It shouldn't change. So:
            const char* groupMenuName = ("Group #" + std::to_string(i + 1)).c_str();

            if (ImGui::BeginMenu(groupMenuName))
            {
                ImGui::Text("Group name: %s", groupName.c_str());

                for (int j = 0; j < group.size(); j++)
                {
                    
                    ImGui::Text("Object name: %s", group[j].name.c_str());
                }

                if (ImGui::Button("Set active"))
                {
                    model.setActiveGroup(i);
                }

                if (ImGui::Button("Delete group"))
                {
                    model.deleteGroup(i);
                }

                ImGui::EndMenu();
            }
        }

        ImGui::EndMenu();
    }
}

void Renderer::drawSceneTree()
{
    ImGui::Begin("Scene Tree");

    drawSceneTreePoints();
    drawSceneTreeLines();
    drawSceneTreePolylines();
    drawSceneTreeGroups();

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
    
    if (ImGui::BeginMenu("NewCoordinates"))
    {
        std::memcpy(lineInput->coordinates, line->getBuffer(), 6 * sizeof(float));
        float* first = &lineInput->coordinates[0];
        float* second = &lineInput->coordinates[3];


        ImGui::InputFloat3("X1 Y1 Z1", first);
        ImGui::InputFloat3("X2 Y2 Z2", second);
        if (ImGui::Button("Click me for change coordinates."))
        {
            line->updateBuffer(lineInput->coordinates);
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
            model.setMode(iter->first);
        }
    }

    ImGui::End();
}

void Renderer::drawSettings()
{
    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("Save project", "Ctrl+S", false, true))
            {
                std::cout << "SAVED" << std::endl;                      // послать сигнал контроллеру
            }
            if (ImGui::MenuItem("Read project", "Ctrl+O", false, true))
            {
                std::cout << "Readed" << std::endl;                     // послать сигнал контроллеру
            }
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }

    float centerX = model.getCenterX();
    float centerY = model.getCenterY();
    
    ImGui::Begin("Settings");
    
    if (ImGui::InputFloat("CenterX", &centerX))
    {
        model.setCenterX(centerX);
    }

    if (ImGui::InputFloat("CenterY", &centerY))
    {
        model.setCenterY(centerY);
    }

    ImGui::End();
}

void Renderer::drawViewState()
{
    ImGui::Begin("ViewState");

    std::map<ViewState, const char*> map = Model::viewStateMap;

    for (auto iter = map.begin(); iter != map.end(); iter++)
    {
        if (ImGui::Button(iter->second))
        {
            model.setViewState(iter->first);
        }
    }

    ImGui::End();
}


void Renderer::draw()
{
    drawPoints();
    drawLines();
    drawPolylines();
}

void Renderer::drawPoints()
{
    Nodes points = model.getPoints();
    for (int i = 0; i < points.size(); i++)
    {
        Point* point = dynamic_cast<Point*>(points[i].node);

        if (point != nullptr)
        {
            point->draw();
        }
        else
            std::cout << "ERROR IN DRAW POINT" << std::endl;
    }
}

void Renderer::drawLines()
{
    Nodes lines = model.getLines();
    for (int i = 0; i < lines.size(); i++)
    {
        lines[i].node->draw();
    }
}

void Renderer::drawPolylines()
{
    Nodes lines = model.getPolylines(); 
    for (int i = 0; i < lines.size(); i++)
    {
        Polyline* line = dynamic_cast<Polyline*>(lines[i].node);
            
        if (line != nullptr)
            line->draw();
        else
            std::cout << "ERROR IN DRAW POLYLINE.";
    }
}
