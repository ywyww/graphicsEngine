#include "Controller.h"

    Controller::Controller()
    {
        activeNode = nullptr;
        lineInputData = new LineInputData();
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

    NodeGroup<Object>* Controller::getActiveNode()
    {
        return activeNode;
    }

    void Controller::setActiveNode(const float& x, const float& y)
    {
        NodeGroup<Object>* current = new NodeGroup<Object>();
        for (int i = 0; i < lines.size(); i++)
        {
            if (lines[i].node->isPointBelongs(x, y, 0, true))
            {
                current->node = lines[i].node;
                current->name = lines[i].name;
            }
                // current = static_cast<NodeGroup<Object>>(lines[i]);
        }
        //activeNode = dynamic_cast<NodeGrsoup<Object>*>(current);
        if (current->node == nullptr)
        {
            delete current;
            current = nullptr;
        }
        activeNode = current;
    }

    float Controller::producePixelCoordinatesToGL(float coord, int dimension)
    {
        return coord * 2 / (dimension) - 1;
    }

    void Controller::addLine(Line* line)
    {
        NodeGroup<Line> lineGrp;
        lineGrp.node = line;
        lineGrp.name = "line1";

        lines.push_back(lineGrp);
    }
    
    void Controller::drawLines()
    {
        for (int i = 0; i < lines.size(); i++)
        {
            lines[i].node->draw();
        }
    }

    LineInputData::LineInputData()
    {
        lineName = new char[lineNameSize] {};
        coordinates = new float[6] {.0f, .0f, .0f, .0f, .0f, .0f};
        translation = new float[3] {.0f, .0f, .0f};
        rotation = new float[3] {.0f, .0f, .0f};
        angle = 0.0f;
    }
    LineInputData::~LineInputData()
    {
        delete[] coordinates;
        delete[] translation;
        delete[] rotation;
    }