#include "Model.h"
std::map<WorkModes, const char*> Model::modeMap = {
        std::pair(WorkModes::POINTER, "Pointer"),
        std::pair(WorkModes::CREATE_POINT, "Point"),
        std::pair(WorkModes::CREATE_LINE, "Line"),
        std::pair(WorkModes::CREATE_POLYLINE, "Polyline"),

        std::pair(WorkModes::TRANSLATE, "Translate"),
        std::pair(WorkModes::ROTATE, "Rotate"),
        std::pair(WorkModes::MODIFY, "Modify active"),
    };


Model::Model(const SDL_Rect& renderArea): renderRect(renderArea), windowHeight(renderArea.h), windowWidth(renderArea.w), shiftX(renderArea.x), shiftY(renderArea.y)
{
    lines = Nodes();
    polyLines = Nodes();
        
    mode = WorkModes::POINTER;

    activeNode = nullptr;
    activeNodeType = ObjectType::NULLTYPE;
}

Model::~Model()
{
    if (activeNode != nullptr)
        delete activeNode;
}

const SDL_Rect& Model::getRenderRect()
{
    return renderRect;
}

const float& Model::getCursorX()
{
    return cursorX;
}

void Model::setCursorX(const float& cursorX)
{
    this->cursorX = cursorX;
}

const float& Model::getCursorY()
{
    return cursorY;
}

void Model::setCursorY(const float& cursorY)
{
    this->cursorY = cursorY;
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

    if (activeNode == nullptr)
        activeNodeType = ObjectType::NULLTYPE;
    else
        activeNodeType = activeNode->type;
}

const ObjectType& Model::getActiveNodeType()
{
    return activeNodeType;
}

void Model::setMode(const WorkModes& mode)
{
    this->mode = mode;
}

const WorkModes& Model::getMode()
{
    return mode;
}

void Model::addLine(Line* line)
{
    NodeGroup lineGrp;
    lineGrp.node = line;
    lineGrp.name = "line1";
    lineGrp.type = ObjectType::LINE;

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
    grp.type = ObjectType::POLYLINE;

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