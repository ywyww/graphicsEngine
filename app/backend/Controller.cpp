#include "Controller.h"

Node* Controller::isPointInSpace(const float& x, const float& y)
{
    // implement
    return nullptr;
}

Node* Controller::isLineInSpace(const float& x, const float& y)
{
    Node* current = new Node();
    Nodes lines = model.getLines();
    for (int i = 0; i < lines.size(); i++)
    {
        Line* line = dynamic_cast<Line*>(lines.operator[](i).node);
        if (line != nullptr && line->isPointBelongs(x, y, 0, model.getWidth(), model.getHeight(), true, 0.07))
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

Node* Controller::isPolylineInSpace(const float& x, const float& y)
{
    Node* current = new Node();
    Nodes polylines = model.getPolylines();
    for (int i = 0; i < polylines.size(); i++)
    {
        Polyline* polyline = dynamic_cast<Polyline*>(polylines.operator[](i).node);
        if (polyline != nullptr && polyline->isPointBelongs(x, y, 0, model.getWidth(), model.getHeight(), true, 0.07))
        {
            current->node = polylines[i].node;
            current->name = polylines[i].name;
            current->type = polylines[i].type;
        }
    }
    if (current->node == nullptr)
    {
        delete current;
        current = nullptr;
    }
    return current;
}



Point* Controller::createPoint(const float& x, const float& y)
{
    float width = model.getWidth();
    float height = model.getHeight();
    Point* point = new Point(
        Translator::producePixelCoordinatesToGL(x, width),
        Translator::producePixelCoordinatesToGL(y, height),
        0
    );
    return point;
}



Line* Controller::createLine(const EditState& editState, const float& x1, const float& y1, const float& x2, const float& y2)
{
    float width = model.getWidth();
    float height = model.getHeight();

    Line* line;


    glm::vec4 first = glm::vec4(Translator::producePixelCoordinatesToGL(x1, width), 
                                    Translator::producePixelCoordinatesToGL(y1, height), 0, 1);

    glm::vec4 second = glm::vec4(Translator::producePixelCoordinatesToGL(x2, width), 
                                        Translator::producePixelCoordinatesToGL(y2, height), 0, 1);


    switch (editState)          // подумать над std::bind вместо этой конструкции
    {
        case EditState::XY:
        {
            line = new Line(
                    first[0],
                    first[1],
                    first[2],
                    second[0],
                    second[1],
                    second[2]
                );
            break;
        }
        case EditState::XZ:
        {
            line = new Line(
                    Translator::producePixelCoordinatesToGL(x1, width),
                    0,
                    Translator::producePixelCoordinatesToGL(y1, height),
                    Translator::producePixelCoordinatesToGL(x2, width),
                    0,
                    Translator::producePixelCoordinatesToGL(y2, height)
                );
            break;
        }
        case EditState::YZ:
        {
            line = new Line(
                    0,
                    Translator::producePixelCoordinatesToGL(y1, height),
                    Translator::producePixelCoordinatesToGL(x1, width),
                    0,
                    Translator::producePixelCoordinatesToGL(y2, height),
                    Translator::producePixelCoordinatesToGL(x2, width)
                );
            break;
        }
        default:
            line = nullptr;
    }
    return line;
}

Polyline* Controller::createPolyline(const float& x0, const float& y0)
{
    Polyline* polyline = new Polyline();
    polyline->addDot(x0, y0);
    return polyline;       
}

Controller::Controller(Model& model): model(model)
{
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


    buildingGroup = Nodes();

    isPolylineCreationMode = false;
    scaleX = scaleY = 0.0f;

    centerPoint = Point(0, 0, 0);


    coordinateSystem = CoordinateSystem();

}

Controller::~Controller()
{
}

glm::mat4 Controller::computeTrimetricMatrix(float angleX, float angleY, float position)
{
    glm::mat4 trimetricMatrix = glm::mat4(0.0f);
    trimetricMatrix[0] = glm::vec4(cos(angleX), sin(angleX) * sin(angleY), 0, sin(angleX) * cos(angleY) / position);
    trimetricMatrix[1] = glm::vec4(0, cos(angleY), 0, -sin(angleY) / position);
    trimetricMatrix[2] = glm::vec4(sin(angleX), -cos(angleX) * sin(angleY), 0, -cos(angleX) * cos(angleY) / position);
    trimetricMatrix[3] = glm::vec4(0, 0, 0, 1);
    return trimetricMatrix;
}


void Controller::readFromFile(std::string filename)
{
    if (Saver::readFromAFile(filename, model))
        std::cout << "Sucess to read" << std::endl;
    else
        std::cout << "Raised problem" << std::endl;
}

void Controller::saveIntoFile(std::string filename)
{
    if (Saver::saveIntoAFile(filename, model))
        std::cout << "Sucess to save" << std::endl;
    else
        std::cout << "Raised problem" << std::endl;
}

Node* Controller::isObjectInSpace(const float& x, const float& y)      // can be problems here.
{
    Node* obj = isPointInSpace(x, y);
    if (obj != nullptr)
       return obj;
    
    obj = isLineInSpace(x, y);
    if (obj != nullptr)
        return obj;

    obj = isPolylineInSpace(x, y);
    if (obj != nullptr)
        return obj;
    
    return nullptr;
}

void Controller::translateObject(Node* object, float relX, float relY, float relZ)
{
    if (object == nullptr)
        throw std::invalid_argument("Object is nullptr (translate object function)");

    float border = 50;

    relX = Mathor::trimInBorders(relX, -border, border);
    relY = Mathor::trimInBorders(relY, -border, border);
    relZ = Mathor::trimInBorders(relZ, -border, border);

    float glXRel = 2 * relX / model.getWidth();
    float glYRel = 2 * relY / model.getHeight();
    float glZRel = 2 * relZ / model.getHeight();

    glm::mat4 transformation = object->node->getTransformation();
    transformation = glm::translate(transformation, glm::vec3(glXRel, glYRel, glZRel));
    object->node->setTransformation(transformation);
}

void Controller::rotateObject(Node* object, float relX, float relY, float relZ)
{
    if (object == nullptr)
        throw std::invalid_argument("Object is nullptr (rotate object function)");

    float border = 50;

    relX = Mathor::trimInBorders(relX, -border, border);
    relY = Mathor::trimInBorders(relY, -border, border);
    relZ = Mathor::trimInBorders(relZ, -border, border);

    float glXRel = 2 * relX / model.getWidth();
    float glYRel = 2 * relY / model.getHeight();
    float glZRel = 2 * relZ / model.getHeight();

    // bool: 1 - glXRel: if (Z < 0) and (X and Y > 0): glm::vec3(0, 0, 1);

    bool xActive = (glXRel != 0);
    bool yActive = (glYRel != 0);
    bool zActive = (glZRel != 0);

    glm::mat4 transformation = object->node->getTransformation();
    transformation = glm::rotate(transformation, glm::radians(1.0f), glm::vec3(1 - (float)xActive, 1 - (float)yActive, 1 - (float)zActive));
    object->node->setTransformation(transformation);
}

void Controller::scaleObject(Node* object, float relX, float relY, float relZ)      // fix on each scaleCoeff
{
    if (object == nullptr)
        throw std::invalid_argument("Object is nullptr (scale object function)");

    float glXRel = 2 * relX / model.getWidth();
    float glYRel = 2 * relY / model.getHeight();
    float glZRel = 2 * relZ / model.getHeight();

    float scale = 1.0f;
    float value = 1 + sqrt(pow(glXRel, 2) + pow(glXRel, 2) + pow(glZRel, 2));
    value *= scale;
    if (glXRel < 0)
    {
        value = 1 / value;
    }

    bool xActive = (glXRel != 0);
    bool yActive = (glYRel != 0);
    bool zActive = (glZRel != 0);

    float scaleX = xActive ? value : 1.0f;
    float scaleY = yActive ? value : 1.0f;
    float scaleZ = zActive ? value : 1.0f;

    glm::mat4 transformation = object->node->getTransformation();
    transformation = glm::scale(transformation, glm::vec3(scaleX, scaleY, scaleZ));
    object->node->setTransformation(transformation);
}

void Controller::mirrorObject(Node* object, float lastUpX, float lastUpY, float lastUpZ)
{
    if (object == nullptr)
        throw std::invalid_argument("Object is nullptr (mirror object function)");
   
    glm::vec2 vecToMirror = glm::vec2(lastUpX - model.getCenterX(), lastUpY - model.getCenterY());
        
    float glXRel = 2 * vecToMirror.x / model.getWidth();
    float glYRel = 2 * vecToMirror.y / model.getHeight();
    float glZRel = 2 * vecToMirror.y / model.getHeight();

    float scaleValX;
    float scaleValY;
    float scaleValZ;
    scaleValX = scaleValY = scaleValZ = 1;

    if (glXRel < 0)
        scaleValX = -1;
    if (glYRel < 0)
        scaleValY = -1;
    if (glZRel < 0)
        scaleValZ = -1;

    bool xActive = (glXRel != 0);
    bool yActive = (glYRel != 0);
    bool zActive = (glZRel != 0);

    float scaleX = xActive ? scaleValX : 1.0f;
    float scaleY = yActive ? scaleValY : 1.0f;
    float scaleZ = zActive ? scaleValZ : 1.0f;
    
    glm::mat4 transformation = object->node->getTransformation();
    transformation = glm::scale(transformation, glm::vec3(scaleX, scaleY, scaleZ));
    object->node->setTransformation(transformation);
}

void Controller::trySetActiveNode(float lastClickedX, float lastClickedY)
{
    Node* node = isObjectInSpace(lastClickedX, lastClickedY);
    model.setActiveNode(node);
}

void Controller::addPoint(const float& x, const float& y)
{
    Point* point = createPoint(x, y);
    model.addPoint(point);
}

void Controller::addLine(const float& x1, const float& y1, const float& x2, const float& y2)
{
    Line* line = createLine(model.getEditState(), x1, y1, x2, y2);

    if (line == nullptr)
        std::cout << "Failed to create line. Check line creation function." << std::endl;
    else
        model.addLine(line);
}

void Controller::addPolyline(const float& x0, const float& y0)
{
    Polyline* line = createPolyline(x0, y0);
    model.addPolyLine(line); 
}

void Controller::addDotInActivePolyline(const float& x1, const float& y1)
{
    Node* activeNode = model.getActiveNode();
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


bool Controller::setIfLineModifable(const float& precision) // if we on 
{
    bool answer = false;
    Node* active = model.getActiveNode();
    if (active != nullptr)
    {
        Line* line = dynamic_cast<Line*>(active->node);
        if (line != nullptr)
        {
            float* buffer = line->getBuffer();
            float height = model.getHeight();
            float width = model.getWidth();

            float x1 = Translator::produceGLCoordinatesToPixel(buffer[0], width);
            float y1 = Translator::produceGLCoordinatesToPixel(buffer[1], height);
            float x2 = Translator::produceGLCoordinatesToPixel(buffer[3], width);
            float y2 = Translator::produceGLCoordinatesToPixel(buffer[4], height);

            if (std::fabs(lastMouseDownX - x1) <= precision && 
                std::fabs(lastMouseDownY - y1) <= precision)
            {
                answer = true;
                xModifableIdx = 0;         // position of modifable
                yModifableIdx = 1;
            }
            if (std::fabs(lastMouseDownX - x2) <= precision && 
                std::fabs(lastMouseDownY - y2) <= precision)
            {
                answer = true;
                xModifableIdx = 3;         // position of modifable
                yModifableIdx = 4;
            }
        }
    }
    return answer;
}

void Controller::modifyLine()
{
    Node* active = model.getActiveNode();
    if (active != nullptr && active->type == ObjectType::LINE)
    {
        Line* line = dynamic_cast<Line*>(active->node);
        glm::mat4 transformation = line->getTransformation();
        if (line != nullptr)
        {
            float* buffer = line->getBuffer();
            float* to = new float[6]{ 0, 0, 0, 0, 0, 0};

            int otherIdx = (xModifableIdx + 3) % 6;
            int otherIdx2 = (yModifableIdx + 3) % 6;

            to[otherIdx] =  buffer[otherIdx];
            to[otherIdx2] =  buffer[otherIdx2];

            to[xModifableIdx] = Translator::producePixelCoordinatesToGL(lastMouseUpX, model.getWidth());
            to[yModifableIdx] = Translator::producePixelCoordinatesToGL(lastMouseUpY, model.getHeight());

            line->updateBuffer(to);
        }
    }
}

void Controller::addNodeInBuildingGroup(Node* possibleNode)
{
    if (possibleNode != nullptr)
    {
        Node addingNode = Node();
        addingNode.name = possibleNode->name;
        addingNode.node = possibleNode->node;
        addingNode.type = possibleNode->type;

        if (addingNode.node == nullptr)
            std::cout << "TROUBLE IN CONTROLLER:: ADD_NODE_IN_BUILDING_GROUP FUNCTION: node->node nullptr" << std::endl;
        buildingGroup.push_back(addingNode);
    }
    else
        std::cout << "CONTROLLER::ADD_NODE_IN_BUILDING_GROUP TROUBLE" << std::endl;
}

void Controller::addNodeInBuildingGroup(const float& x, const float& y)
{
    Node* possibleNode = isObjectInSpace(x, y);

    if (possibleNode != nullptr)
    {
        Node addingNode = Node();
        addingNode.name = possibleNode->name;
        addingNode.node = possibleNode->node;
        addingNode.type = possibleNode->type;

        if (addingNode.node == nullptr)
            std::cout << "TROUBLE IN CONTROLLER:: ADD_NODE_IN_BUILDING_GROUP FUNCTION: node->node nullptr" << std::endl;
        buildingGroup.push_back(addingNode);
    }
    else
        std::cout << "CONTROLLER::ADD_NODE_IN_BUILDING_GROUP TROUBLE" << std::endl;
}

void Controller::clearBuildingGroup()
{
    buildingGroup.clear();
}

void Controller::processRubberThread()
{
    if (isMouseDown && model.getMode() != WorkModes::TRANSLATE)
    {
        float* newBuff = new float[6] {
            0, 0, 0,
            0, 0, 0
        };

        newBuff[0] = Translator::producePixelCoordinatesToGL(lastMouseDownX, model.getWidth());
        newBuff[1] = Translator::producePixelCoordinatesToGL(lastMouseDownY, model.getHeight());
        newBuff[3] = Translator::producePixelCoordinatesToGL(cursorX, model.getWidth());
        newBuff[4] = Translator::producePixelCoordinatesToGL(cursorY, model.getHeight());

        rubberThread->updateBuffer(newBuff);
        rubberDrawable = true;
    }
    else
    {
        rubberDrawable = false;
    }
}

void Controller::processObjectTranslation(const EditState& editState, Node* object, float relA, float relB)
{
    if (editState == EditState::XY)
    {
        translateObject(object, relA, relB, 0.0f);
    }
    else if (editState == EditState::XZ)
    {
        translateObject(object, relA, 0.0f, relB);
    }
    else if (editState == EditState::YZ)
    {
        translateObject(object, 0.0f, relB, relA);
    }
}

void Controller::processObjectRotation(const EditState& editState, Node* object, float relA, float relB)
{
    if (editState == EditState::XY)
    {
        rotateObject(object, relA, relB, 0.0f);
    }
    else if (editState == EditState::XZ)
    {
        rotateObject(object, relA, 0.0f, relB);
    }
    else if (editState == EditState::YZ)
    {
        rotateObject(object, 0.0f, relB, relA);
    }
}

void Controller::processObjectScaling(const EditState& editState, Node* object, float relA, float relB)
{
    if (editState == EditState::XY)
    {
        scaleObject(object, relA, relB, 0.0f);
    }
    else if (editState == EditState::XZ)
    {
        scaleObject(object, relA, 0.0f, relB);
    }
    else if (editState == EditState::YZ)
    {
        scaleObject(object, 0.0f, relB, relA);
    }
}

void Controller::processObjectMirroring(const EditState& editState, Node* object, float relA, float relB)
{
    if (editState == EditState::XY)
    {
        mirrorObject(object, relA, relB, 0.0f);
    }
    else if (editState == EditState::XZ)
    {
        mirrorObject(object, relA, 0.0f, relB);
    }
    else if (editState == EditState::YZ)
    {
        mirrorObject(object, 0.0f, relB, relA);
    }
}

void Controller::processGroupOperation(const EditState& editState, std::function<void(Node*,float,float,float)> operation, Nodes* objects, float relA, float relB)
{
    if (objects == nullptr)
        throw std::invalid_argument("Objects is nullptr. (operation on group function)");
    
    for (int i = 0; i < objects->size(); i++)
    {
        Node* object = &(objects->operator[](i));

        if (object == nullptr)
            throw std::bad_cast();          // bad object cast: do operation on group
                
        if (editState == EditState::XY)
        {
            operation(object, relA, relB, 0.0f);
        }
        else if (editState == EditState::XZ)
        {
            operation(object, relA, 0.0f, relB);
        }
        else if (editState == EditState::YZ)
        {
            operation(object, 0.0f, relB, relA);
        }
    }
}


void Controller::processEvent(SDL_Event& event, const float& wWidth, const float& wHeight)      // get full model data in 1 structure
{
    SDL_Rect glRenderArea = model.getRenderRect();
    WorkModes mode = model.getMode();
    ObjectType objectType = model.getActiveNodeType();
    EditState editState = model.getEditState();
   
    // get active node 

    float* centerPointBuff = new float[3] {
        Translator::producePixelCoordinatesToGL(model.getCenterX(), glRenderArea.w), 
        Translator::producePixelCoordinatesToGL(model.getCenterY(), glRenderArea.h), 
        0.0f};

    centerPoint.updateBuffer(centerPointBuff);

    if (event.type == SDL_MOUSEMOTION)
    {
        cursorAbsX = event.motion.x;
        cursorAbsY = event.motion.y;

        cursorX = cursorAbsX - glRenderArea.x;
        cursorY = wHeight - glRenderArea.y - cursorAbsY;

        model.setCursorX(cursorX);
        model.setCursorY(cursorY);        

        if (cursorX <= glRenderArea.w && cursorY <= glRenderArea.h)
            isCursorInRenderArea = true;
        else
            isCursorInRenderArea = false;

        if (isMouseDown)
        {
            float xRel = event.motion.xrel;
            float yRel = event.motion.yrel;
            
            if (mode == WorkModes::TRANSLATE)
            {
                if (objectType == ObjectType::GROUPMODE)
                {
                    std::function<void(Node*, float, float, float)> operation = std::bind(&Controller::translateObject, this, 
                                std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);
                    
                    processGroupOperation(editState, operation, model.getActiveGroup(), xRel, -yRel);
                }
                else
                {
                    processObjectTranslation(editState, model.getActiveNode(), xRel, -yRel);
                }
            }

            else if (mode == WorkModes::ROTATE)
            {
                if (objectType == ObjectType::GROUPMODE)
                {
                    std::function<void(Node*, float, float, float)> operation = std::bind(&Controller::rotateObject, this, 
                                std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);
                    
                    processGroupOperation(editState, operation, model.getActiveGroup(), xRel, -yRel);
                }
                else
                {
                    processObjectRotation(editState, model.getActiveNode(), xRel, -yRel);
                }
            }
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

        if (mode == WorkModes::CREATE_POINT)
        {
            addPoint(lastMouseDownX, lastMouseDownY);
        }

        if (mode == WorkModes::CREATE_POLYLINE)
        {
            if (isPolylineCreationMode == true)
            {
                addDotInActivePolyline(lastMouseDownX, lastMouseDownY);
            }
            else
            {
                addPolyline(lastMouseDownX, lastMouseDownY);
                Nodes massive = model.getPolylines();
                int size = massive.size();
                if (size >= 1)
                {
                    Node* newActiveNode = new Node();
                    newActiveNode->node = massive[size - 1].node;
                    newActiveNode->name = massive[size - 1].name;
                    newActiveNode->type = massive[size - 1].type;

                    model.setActiveNode(newActiveNode);
                    isPolylineCreationMode = true;
                }

            }
        }

        if (mode == WorkModes::CREATE_GROUP)
        {
            if (isGroupCreationMode)
            {
                addNodeInBuildingGroup(lastMouseDownX, lastMouseDownY);
            }

        }

        if (objectType == ObjectType::LINE && 
            mode == MODIFY)
        { 
            isLineModifable = setIfLineModifable(5.0f);
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

        if (objectType == ObjectType::LINE &&
            mode == WorkModes::MODIFY && 
            isLineModifable)
        {
            modifyLine();
            isLineModifable = false;
        }

        if (mode == WorkModes::SCALE)
        {
            float aX = lastMouseDownX - lastMouseUpX;
            float aY = lastMouseDownY - lastMouseUpY;

            if (objectType == ObjectType::GROUPMODE)
            {
                std::function<void(Node*, float, float, float)> operation = std::bind(&Controller::scaleObject, this, 
                                std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);
                    
                processGroupOperation(editState, operation, model.getActiveGroup(), -aX, aY);
            }
            else 
            {
                processObjectScaling(editState, model.getActiveNode(), -aX, aY);
            }
        }

        if (mode == WorkModes::MIRROR)
        {
            if (objectType == ObjectType::GROUPMODE)
            {
                std::function<void(Node*, float, float, float)> operation = std::bind(&Controller::mirrorObject, this, 
                                std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);
                    
                processGroupOperation(editState, operation, model.getActiveGroup(), lastMouseUpX, lastMouseUpY);
            }
            else
            {
                processObjectMirroring(editState, model.getActiveNode(), lastMouseUpX, lastMouseUpY);
            }
        }      
    }

    if (event.type == SDL_KEYDOWN)              // fix for current situation: polyline mode, group mode
    {
        isPolylineCreationMode = false;
        isGroupCreationMode = false;
    }

    if (!isGroupCreationMode)
    {
        if (buildingGroup.size() != 0)
        {
            model.addGroup(buildingGroup);                 // what ptr.
            buildingGroup = Nodes();
        }
        else
        {
            std::cout << "Trouble: buildingGroup size == 0" << std::endl;
        }
        isGroupCreationMode = true;
    }

    processRubberThread();
    // process Controller's events.
}
