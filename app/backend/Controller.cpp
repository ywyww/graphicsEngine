#include "Controller.h"

    std::map<WorkModes, const char*> Controller::modeMap = {
        std::pair(WorkModes::POINTER, "Pointer"),
        std::pair(WorkModes::ROTATE, "Rotate"),
        std::pair(WorkModes::TRANSLATE, "Translate"),
        std::pair(WorkModes::CREATION, "CREATION")
    };

    std::map<ObjectCreationModes, const char*> Controller::modeCreationMap = {
        std::pair(ObjectCreationModes::POINT, "Point"),
        std::pair(ObjectCreationModes::LINE, "Line")
    };

    Controller::Controller()
    {
        activeNode = nullptr;
        lineInputData = new LineInputData();
        mode = WorkModes::POINTER;
        creationMode = ObjectCreationModes::LINE;
    }

    Controller::~Controller()
    {
        delete lineInputData;
    }

    Lines& Controller::getLines()
    {
        return lines;
    }

    LineInputData* Controller::getLineInput()
    {
        return lineInputData;
    }

    NodeGroup* Controller::getActiveNode()
    {
        return activeNode;
    }

    NodeGroup* Controller::isObjectInSpace(const float& x, const float& y, const float& width, const float& height)
    {
        NodeGroup* current = new NodeGroup();
        for (int i = 0; i < lines.size(); i++)
        {
            Line* line = dynamic_cast<Line*>(lines[i].node);
            if (line != nullptr && line->isPointBelongs(x, y, 0, width, height, true, 0.07))
            {
                current->node = lines[i].node;
                current->name = lines[i].name;
            }
        }
        if (current->node == nullptr)
        {
            delete current;
            current = nullptr;
        }
        return current;
    }

    void Controller::setActiveNode(NodeGroup* object)
    {
        activeNode = object;
    }

    void Controller::setMode(const WorkModes& mode)
    {
        this->mode = mode;
    }

    const WorkModes& Controller::getMode()
    {
        return mode;
    }

    void Controller::setCreationMode(const ObjectCreationModes& mode)
    {
        this->creationMode = mode;
    }

    const ObjectCreationModes& Controller::getCreationMode()
    {
        return creationMode;
    }

    void Controller::translateObject(float relX, float relY, const float& wWidth, const float& wHeight)
    {
        if (getMode() == WorkModes::TRANSLATE && activeNode != nullptr)
        {
            float border = 50;
            
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

            glm::mat4 transformation = activeNode->node->getTransformation();
            transformation = glm::translate(transformation, glm::vec3(glXRel, glYRel, 0.0f));
            activeNode->node->setTransformation(transformation);
        }
    }

    void Controller::rotateObject(float relX, float relY, const float& wWidth, const float& wHeight)
    {
        if (getMode() == WorkModes::ROTATE && activeNode != nullptr)
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

            glm::mat4 transformation = activeNode->node->getTransformation();
            transformation = glm::rotate(transformation, glm::radians(std::atan(glXRel/glYRel)), glm::vec3(0.0f, 0.0f, 1.0f));
            activeNode->node->setTransformation(transformation);
        }
    }



    float Controller::producePixelCoordinatesToGL(float coord, int dimension)
    {
        return coord * 2 / (dimension) - 1;
    }

    void Controller::addLine(Line* line)
    {
        NodeGroup lineGrp;
        lineGrp.node = line;
        lineGrp.name = "line1";

        lines.push_back(lineGrp);
    }

    bool Controller::deleteLine(int idx)
    {
        int counter = 0;
        auto position = lines.begin();
        while (position != lines.end() && counter < idx)
        {
            position ++;
            counter++;
        }

        if (counter == idx)
        {
            lines.erase(position);
            return true;
        }
        return false;
    }
    
    void Controller::drawLines()
    {
        for (int i = 0; i < lines.size(); i++)
        {
            lines[i].node->draw();
        }
    }

    
