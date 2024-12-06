#include "Model.h"
std::map<WorkModes, const char*> Model::modeMap = {
        std::pair(WorkModes::POINTER, "Pointer"),
        std::pair(WorkModes::CREATE_POINT, "Point"),
        std::pair(WorkModes::CREATE_LINE, "Line"),
        std::pair(WorkModes::CREATE_POLYLINE, "Polyline"),

        std::pair(WorkModes::TRANSLATE, "Translate"),
        std::pair(WorkModes::ROTATE, "Rotate"),
        std::pair(WorkModes::SCALE, "Scale"),
        std::pair(WorkModes::MIRROR, "Mirror"),
        std::pair(WorkModes::PROJECTION, "Projection"),


        std::pair(WorkModes::MODIFY, "Modify active"),
        std::pair(WorkModes::COLORIZE, "Colorize"),
    };


Model::Model(const SDL_Rect& renderArea): renderRect(renderArea), windowHeight(renderArea.h), windowWidth(renderArea.w), shiftX(renderArea.x), shiftY(renderArea.y)
{
    points = Nodes();
    lines = Nodes();
    polyLines = Nodes();
        
    mode = WorkModes::POINTER;

    activeNode = nullptr;
    activeNodeType = ObjectType::NULLTYPE;

    centerX = windowWidth / 2;
    centerY = windowHeight / 2;
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

const float& Model::getCenterX()
{
    return centerX;
}

void Model::setCenterX(const float& x)
{
    centerX = x;
}

const float& Model::getCenterY()
{
    return centerY;
}

void Model::setCenterY(const float& y)
{
    centerY = y;
}


const float& Model::getWidth()
{
    return windowWidth;
}

const float& Model::getHeight()
{
    return windowHeight;
}

Nodes& Model::getPoints()
{
    return points;
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

void Model::addPoint(Point* point)
{
    NodeGroup pnt;
    pnt.node = point;
    pnt.name = "point1";
    pnt.type = ObjectType::POINT;

    points.push_back(pnt);
}

bool Model::deletePoint(int idx)
{
    int counter = 0;
    auto position = points.begin();
    while (position != points.end() && counter < idx)
    {
        position ++;
        counter++;
    }

    if (counter == idx)
    {
        points.erase(position);
        return true;
    }
    return false;
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

// std::ostream& operator<<(std::ostream& os, const Model& model)
// {
//     os << model.renderRect.h << " " << model.renderRect.w << " " << model.renderRect.x << " " << model.renderRect.y << "\n"
//     << model.shiftX << " " << model.shiftY << "\n"
//     << model.windowWidth << " " << model.windowHeight << "\n"
//     << model.cursorX << " " << model.cursorY << "\n"
//     << model.centerX << " " << model.centerY << "\n"
//     << ObjectType::NULLTYPE << "\n"
//     << WorkModes::POINTER << "\n"
//     << model.points << "\n"
//     << model.lines << "\n"
//     << model.polyLines << "\n";


//     return os;
// }

// std::istream& operator>>(std::istream& is, Model& model)
// {
//     is >> model.renderRect.h >> model.renderRect.w >> model.renderRect.x >> model.renderRect.y;
//     is >> model.shiftX >> model.shiftY;
//     is >> model.windowWidth >> model.windowHeight;
//     is >> model.cursorX >> model.cursorY;
//     is >> model.centerX >> model.centerY;

//     int nodeType;

//     is >> nodeType;
//     model.activeNodeType = static_cast<ObjectType>(nodeType);

//     int mode;
//     is >> mode;
//     model.mode = static_cast<WorkModes>(mode);

//     model.activeNode = nullptr;

    

            
//     return is;  
// }