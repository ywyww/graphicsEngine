#include "Controller.h"

    std::map<WorkModes, const char*> Controller::modeMap = {
        std::pair(WorkModes::POINTER, "Pointer"),
        std::pair(WorkModes::ROTATE, "Rotate"),
        std::pair(WorkModes::TRANSLATE, "Translate"),
        std::pair(WorkModes::CREATION, "Creation")
    };

    std::map<ObjectCreationModes, const char*> Controller::modeCreationMap = {
        std::pair(ObjectCreationModes::POINT, "Point"),
        std::pair(ObjectCreationModes::LINE, "Line"),
        std::pair(ObjectCreationModes::POLYLINE, "Polyline")
    };

    Controller::Controller()
    {
        lines = Nodes();
        polyLines = Nodes();
        
        mode = WorkModes::POINTER;
        creationMode = ObjectCreationModes::LINE;

        lineInputData = new LineInputData();
        activeNode = nullptr;
    }

    Controller::~Controller()
    {
        delete lineInputData;
    }

    Nodes& Controller::getLines()
    {
        return lines;
    }

    Nodes& Controller::getPolylines()
    {
        return polyLines;
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

    void Controller::addPolyLine(Polyline* polyline)
    {
        NodeGroup grp;
        grp.name = "polyline1";
        grp.node = polyline;

        polyLines.push_back(grp);
    }

    bool Controller::deletePolyLine(int idx)
    {
        int counter = 0;
        auto position = polyLines.begin();
        while (position != polyLines.end() && counter < idx)
        {
            position ++;
            counter++;
        }

        if (counter == idx)
        {
            polyLines.erase(position);
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

    void Controller::drawPolyLines()
    {
        for (int i = 0; i < polyLines.size(); i++)
        {
            Polyline* line = dynamic_cast<Polyline*>(polyLines[i].node);
            
            if (line != nullptr)
                line->draw();
            else
                std::cout << "ERROR IN DRAW POLYLINE.";
        }
    }

    void Controller::trySetActiveNode(float lastClickedX, float lastClickedY, const float& wWidth, const float& wHeight)
    {
        if (getMode() == WorkModes::POINTER)
        {
            NodeGroup* node = isObjectInSpace(lastClickedX, lastClickedY, wWidth, wHeight);
            setActiveNode(node);
        }
    }

    void Controller::createLine(const float& x1, const float& y1, const float& x2, const float& y2, const float& wWidth, const float& wHeight)
    {
        Line* line = new Line(
                        Translator::producePixelCoordinatesToGL(x1, wWidth),
                        Translator::producePixelCoordinatesToGL(y1, wHeight),
                        0,
                        Translator::producePixelCoordinatesToGL(x2, wWidth),
                        Translator::producePixelCoordinatesToGL(y2, wHeight),
                        0
                    );
        addLine(line);
    }

    Polyline* Controller::createPolyline()
    {
        Polyline* polyline = new Polyline();
        addPolyLine(polyline); 
        return polyline;       
    }

    void Controller::addLineInPolyline(Polyline* polyline, const float& x1, const float& y1)
    {
        polyline->addDot(x1, y1);
        std::cout << "added dot: " << x1 << " " << y1 << std::endl;
    }

    void Controller::createObject(const float& x1, const float& y1, const float& x2, const float& y2, const float& wWidth, const float& wHeight)
    {
        if (getMode() == WorkModes::CREATION)
        {
            switch (getCreationMode())
            {
                case ObjectCreationModes::POLYLINE:
                {
                    break;
                }
                case ObjectCreationModes::LINE:
                {
                    createLine(x1, y1, x2, y2, wWidth, wHeight);
                    break;
                }
                case ObjectCreationModes::POINT:
                {
                    // not implemented
                    break;
                }
            }
        }
    }


    
