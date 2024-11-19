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
    x1 = y1 = x2 = y2 = 0;

}

Controller::~Controller()
{
}

NodeGroup* Controller::isObjectInSpace(const float& x, const float& y)      // can be problems here.
{
    NodeGroup* current = new NodeGroup();
    Nodes lines = model->getLines();
    for (int i = 0; i < lines.size(); i++)
    {
        Line* line = dynamic_cast<Line*>(lines.operator[](i).node);
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
    NodeGroup* activeNode = model->getActiveNode();
    if (model->getMode() == WorkModes::TRANSLATE &&  activeNode != nullptr)
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
    NodeGroup* activeNode = model->getActiveNode();
    if (model->getMode() == WorkModes::ROTATE && activeNode != nullptr)
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
    if (model->getMode() == WorkModes::POINTER)
    {
        NodeGroup* node = isObjectInSpace(lastClickedX, lastClickedY);
        model->setActiveNode(node);
    }
    
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

void Controller::createObject(const float& x1, const float& y1, const float& x2, const float& y2)
{
    if (model->getMode() == WorkModes::CREATION)
    {
        switch (model->getCreationMode())
        {
            case ObjectCreationModes::POLYLINE:
            {
                break;
            }
            case ObjectCreationModes::LINE:
            {
                addLine(x1, y1, x2, y2);
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

void Controller::processEvent(SDL_Event& event, const float& wWidth, const float& wHeight)
{
    SDL_Rect glRenderArea = model->getRenderRect();

    if (event.type == SDL_MOUSEMOTION)
    {
        x = event.motion.x;
        y = event.motion.y;

        belongX = x - glRenderArea.x;
        belongY = wHeight - glRenderArea.y - y;

        model->setCursorX(belongX);
        model->setCursorY(belongY);        

        if (belongX <= glRenderArea.w && belongY <= glRenderArea.h)
            isCursorInRenderArea = true;
        else
            isCursorInRenderArea = false;

        if (mouseDown)
        {
            float xRel = event.motion.xrel;
            float yRel = event.motion.yrel;

            translateObject(xRel, -yRel);
            rotateObject(xRel, -yRel);
        }
    }
    if (event.type == SDL_MOUSEBUTTONDOWN && 
        event.motion.x < glRenderArea.w + glRenderArea.x && 
        event.motion.y < glRenderArea.h + glRenderArea.y && isCursorInRenderArea)
    {
        lastMouseClickedX = belongX;
        lastMouseClickedY = belongY;
        mouseDown = true;

        trySetActiveNode(lastMouseClickedX, lastMouseClickedY);
        x1 = belongX;
        y1 = belongY;
    }
    if (event.type == SDL_MOUSEBUTTONUP && isCursorInRenderArea)
    {
        mouseDown = false;
        x2 = belongX;
        y2 = belongY;

        createObject(x1, y1, x2, y2);
    }
}

    
