#include "Model.h"
std::map<WorkModes, const char*> Model::modeMap = {
        std::pair(WorkModes::POINTER, "Pointer"),
        std::pair(WorkModes::ROTATE, "Rotate"),
        std::pair(WorkModes::TRANSLATE, "Translate"),
        std::pair(WorkModes::CREATION, "Creation")
    };

std::map<ObjectCreationModes, const char*> Model::modeCreationMap = {
    std::pair(ObjectCreationModes::POINT, "Point"),
    std::pair(ObjectCreationModes::LINE, "Line"),
    std::pair(ObjectCreationModes::POLYLINE, "Polyline")
};

Model::Model(const float& windowHeight, const float& windowWidth)
{
    windowHeight = windowHeight;
    windowWidth = windowWidth;
    
    lines = Nodes();
    polyLines = Nodes();
        
    mode = WorkModes::POINTER;
    creationMode = ObjectCreationModes::LINE;

    activeNode = nullptr;
}

Model::~Model()
{
    if (activeNode != nullptr)
        delete activeNode;
}

const float& Model::getWidth()
{
    return windowWidth;
}

const float& Model::getHeight()
{
    return windowHeight;
}

Nodes& Model::getLines()
{
    return lines;
}

Nodes& Model::getPolylines()
{
    return polyLines;
}

NodeGroup* Model::getActiveNode()
{
    return activeNode;
}

void Model::setActiveNode(NodeGroup* object)
{
    activeNode = object;
}

void Model::setMode(const WorkModes& mode)
{
    this->mode = mode;
}

const WorkModes& Model::getMode()
{
    return mode;
}

void Model::setCreationMode(const ObjectCreationModes& mode)
{
    this->creationMode = mode;
}

const ObjectCreationModes& Model::getCreationMode()
{
    return creationMode;
}

void Model::addLine(Line* line)
{
    NodeGroup lineGrp;
    lineGrp.node = line;
    lineGrp.name = "line1";

    lines.push_back(lineGrp);
}

bool Model::deleteLine(int idx)
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

void Model::addPolyLine(Polyline* polyline)
{
    NodeGroup grp;
    grp.name = "polyline1";
    grp.node = polyline;

    polyLines.push_back(grp);
}

bool Model::deletePolyLine(int idx)
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