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

Polyline* Controller::createPolyline(const float& x0, const float& y0)
{
    Polyline* polyline = new Polyline();
    polyline->addDot(x0, y0);
    return polyline;       
}

Controller::Controller(Model* model)
{
    this->model = model;

    cursorAbsX = cursorAbsY = 0;
    cursorX = cursorY = 0;
    cursorGlX = cursorGlY = 0;  

    lastMouseDownX = lastMouseDownY = 0;
    lastMouseUpX = lastMouseUpY = 0;

    isCursorVirginClicked = isMouseDown = isCursorInRenderArea = false;
    
    isLineModifable = false;
    xModifableIdx = yModifableIdx = 0;

    rubberThread = new Line();
    rubberThread->setColor(glm::vec3(1.f));
    rubberDrawable = false;
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
            current->type = lines[i].type;
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
    NodeGroup* node = isObjectInSpace(lastClickedX, lastClickedY);
    model->setActiveNode(node);
}

void Controller::addLine(const float& x1, const float& y1, const float& x2, const float& y2)
{
    Line* line = createLine(x1, y1, x2, y2);
    model->addLine(line);
}

void Controller::addPolyline(const float& x0, const float& y0)
{
    Polyline* line = createPolyline(x0, y0);
    model->addPolyLine(line); 
}

void Controller::addDotInActivePolyline(const float& x1, const float& y1)
{
    NodeGroup* activeNode = model->getActiveNode();
    if (activeNode != nullptr)
    {
        Polyline* polyline = dynamic_cast<Polyline*>(activeNode->node); 
        if (polyline != nullptr)
        {
            polyline->addDot(x1, y1);
            std::cout << "added dot: " << x1 << " " << y1 << std::endl;
        }
        else
        {
            std::cout << "Failed to add dot in polyline";
        }
        
    }
    else
    {
        std::cout << "ActiveNode is nullptr";
    }
    
}

void Controller::modifyLine(const float& precision)
{
    NodeGroup* active = model->getActiveNode();
    if (active != nullptr)
    {
        Line* line = dynamic_cast<Line*>(active->node);
        if (line != nullptr)
        {
            float* buffer = line->getBuffer();
            float height = model->getHeight();
            float width = model->getWidth();

            float x1 = Translator::produceGLCoordinatesToPixel(buffer[0], width);
            float y1 = Translator::produceGLCoordinatesToPixel(buffer[1], height);
            float x2 = Translator::produceGLCoordinatesToPixel(buffer[3], width);
            float y2 = Translator::produceGLCoordinatesToPixel(buffer[4], height);

            if (std::fabs(lastMouseDownX - x1) <= precision && 
                std::fabs(lastMouseDownY - y1) <= precision)
            {
                isLineModifable = true;
                xModifableIdx = 0;         // position of modifable
                yModifableIdx = 1;
            }
            if (std::fabs(lastMouseDownX - x2) <= precision && 
                std::fabs(lastMouseDownY - y2) <= precision)
            {
                isLineModifable = true;
                xModifableIdx = 3;         // position of modifable
                yModifableIdx = 4;
            }
        }
    }

}

void Controller::processEvent(SDL_Event& event, const float& wWidth, const float& wHeight)      // get full model data in 1 structure
{
    SDL_Rect glRenderArea = model->getRenderRect();
    WorkModes mode = model->getMode();
   
    // get active node 

    if (event.type == SDL_MOUSEMOTION)
    {
        cursorAbsX = event.motion.x;
        cursorAbsY = event.motion.y;

        cursorX = cursorAbsX - glRenderArea.x;
        cursorY = wHeight - glRenderArea.y - cursorAbsY;

        model->setCursorX(cursorX);
        model->setCursorY(cursorY);        

        if (cursorX <= glRenderArea.w && cursorY <= glRenderArea.h)
            isCursorInRenderArea = true;
        else
            isCursorInRenderArea = false;

        if (isMouseDown)
        {
            float xRel = event.motion.xrel;
            float yRel = event.motion.yrel;
            
            if (mode == WorkModes::TRANSLATE)
                translateObject(xRel, -yRel);       // ubrat', postavit' flagi
            else if (mode == WorkModes::ROTATE)
                rotateObject(xRel, -yRel);
        }
    }
    if (event.type == SDL_MOUSEBUTTONDOWN && isCursorInRenderArea)
    {
        lastMouseDownX = cursorX;
        lastMouseDownY = cursorY;
        isMouseDown = true;


        if (mode == WorkModes::POINTER)     // also other nodes
        {
            trySetActiveNode(lastMouseDownX, lastMouseDownY); // ubrat', postavit' flagi
        }

        if (mode == WorkModes::CREATE_POLYLINE)
        {
            addPolyline(lastMouseDownX, lastMouseDownY);
            Nodes massive = model->getPolylines();
            int size = massive.size() - 1;
            if (size >= 1)
            {
                NodeGroup* newActiveNode = new NodeGroup();
                newActiveNode->node = massive[size - 1].node;
                newActiveNode->name = massive[size - 1].name;
                newActiveNode->type = massive[size - 1].type;

                model->setActiveNode(newActiveNode);
                model->setMode(WorkModes::MODIFY);
            }
            else
                std::cout << "TROUBLE";
                
        }
        if (model->getActiveNodeType() == ObjectType::POLYLINE && 
            mode == WorkModes::MODIFY)       // modification
        {
            addDotInActivePolyline(lastMouseDownX, lastMouseDownY);
        }


        if (model->getActiveNodeType() == ObjectType::LINE && 
            mode == MODIFY)
        { 
            modifyLine(5.0f);
        }

    }
    if (event.type == SDL_MOUSEBUTTONUP && 
        isCursorInRenderArea)
    {
        isMouseDown = false;
        lastMouseUpX = cursorX;
        lastMouseUpY = cursorY;

        
        if (mode == WorkModes::CREATE_LINE)
            addLine(lastMouseDownX, lastMouseDownY, lastMouseUpX, lastMouseUpY);

        if (model->getActiveNodeType() == ObjectType::LINE &&
            mode == WorkModes::MODIFY && 
            isLineModifable)
        {
                NodeGroup* active = model->getActiveNode();
                if (active != nullptr)
                {
                    Line* line = dynamic_cast<Line*>(active->node);
                    if (line != nullptr)
                    {
                        float* buffer = line->getBuffer();
                        float* to = new float[6]{ 0, 0, 0, 0, 0, 0};


                        // just use memcpy

                        int otherIdx = (xModifableIdx + 3) % 6;
                        int otherIdx2 = (yModifableIdx + 3) % 6;
                        to[otherIdx] =  buffer[otherIdx];
                        to[otherIdx2] =  buffer[otherIdx2];

                        to[xModifableIdx] = Translator::producePixelCoordinatesToGL(lastMouseUpX, model->getWidth());
                        to[yModifableIdx] = Translator::producePixelCoordinatesToGL(lastMouseUpY, model->getHeight());

                        std::cout << to[0] << " " << to[1] << " " << to[2] << std::endl;
                        std::cout << to[3] << " " << to[4] << " " << to[5] << std::endl;
                        line->updateBuffer(to);
                    }
                }
                isLineModifable = false;
        }

        
    }

    if (event.type == SDL_KEYDOWN)
    {
        if (model->getActiveNodeType() == ObjectType::POLYLINE && 
            mode == WorkModes::MODIFY)
        {   
            model->setMode(WorkModes::CREATE_POLYLINE);
        }
    }

    if (isMouseDown && mode != WorkModes::TRANSLATE)
    {
        float* newBuff = new float[6] {
            0, 0, 0,
            0, 0, 0
        };

        newBuff[0] = Translator::producePixelCoordinatesToGL(lastMouseDownX, model->getWidth());
        newBuff[1] = Translator::producePixelCoordinatesToGL(lastMouseDownY, model->getHeight());
        newBuff[3] = Translator::producePixelCoordinatesToGL(cursorX, model->getWidth());
        newBuff[4] = Translator::producePixelCoordinatesToGL(cursorY, model->getHeight());

        rubberThread->updateBuffer(newBuff);
        rubberDrawable = true;
    }
    else
    {
        rubberDrawable = false;
    }
    
    // process Controller's events.
}

    
