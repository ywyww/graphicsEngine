#include "Controller.h"


Line* Controller::createLine(const float& x1, const float& y1, const float& x2, const float& y2)
{
    float width = model->getWidth();
    float height = model->getHeight();
    Line* line = new Line(
                    Translator::producePixelCoordinatesToGL(x1, width),
                    Translator::producePixelCoordinatesToGL(y1, height),
                    0,
                    Translator::producePixelCoordinatesToGL(x2, width),
                    Translator::producePixelCoordinatesToGL(y2, height),
                    0
                );
    return line;
}

Polyline* Controller::createPolyline()
{
    Polyline* polyline = new Polyline();
    model->addPolyLine(polyline); 
    return polyline;       
}

Controller::Controller(Model* model)
{
    this->model = model;
}

Controller::~Controller()
{
}

NodeGroup* Controller::isObjectInSpace(const float& x, const float& y)
{
    NodeGroup* current = new NodeGroup();
    for (int i = 0; i < lines.size(); i++)
    {
        Line* line = dynamic_cast<Line*>(lines[i].node);
        if (line != nullptr && line->isPointBelongs(x, y, 0, model->getWidth(), model->getHeight(), true, 0.07))
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

void Controller::translateObject(float relX, float relY)
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

        float glXRel = 2 * relX / model->getWidth();
        float glYRel = 2 * relY / model->getHeight();

        glm::mat4 transformation = activeNode->node->getTransformation();
        transformation = glm::translate(transformation, glm::vec3(glXRel, glYRel, 0.0f));
        activeNode->node->setTransformation(transformation);
    }
}

void Controller::rotateObject(float relX, float relY)
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

        float glXRel = 2 * relX / model->getWidth();
        float glYRel = 2 * relY / model->getHeight();

        glm::mat4 transformation = activeNode->node->getTransformation();
        transformation = glm::rotate(transformation, glm::radians(std::atan(glXRel/glYRel)), glm::vec3(0.0f, 0.0f, 1.0f));
        activeNode->node->setTransformation(transformation);
    }
}

void Controller::trySetActiveNode(float lastClickedX, float lastClickedY)
{
    NodeGroup* node = isObjectInSpace(lastClickedX, lastClickedY);
    model->setActiveNode(node);
}

void Controller::addLine(const float& x1, const float& y1, const float& x2, const float& y2)
{
    Line* line = createLine(x1, y1, x2, y2);
    model->addLine(line);
}

void Controller::addLineInPolyline(Polyline* polyline, const float& x1, const float& y1)
{
    polyline->addDot(x1, y1);
    std::cout << "added dot: " << x1 << " " << y1 << std::endl;
}



    
