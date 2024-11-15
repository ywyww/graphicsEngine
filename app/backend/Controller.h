#include "Objects/Scene/Groups.h"
#include "Interfaces/Object.h"
#include <map>

#ifndef CONTROLLER_H
#define CONTROLLER_H

    enum WorkModes
    {
        POINTER,

        DRAW_POINT,
        DRAW_LINE,

        ROTATE,
        TRANSLATE,
    };

    enum ObjectCreation
    {
        POINT,
        LINE
    }

    struct LineInputData
    {
        float* coordinates; // 6 (x, y, z) * 2
        float* color;       // r g b

        float* translation; // 3 x y z
        float* rotation; // 3 x y z
        float angle;

        char* lineName;
        size_t lineNameSize = 36;

    public:
        LineInputData();
        ~LineInputData();
    };

    class Controller
    {   
        // make structure for this objects.
        Lines lines;
        std::string linesName;

        NodeGroup* activeNode;

        LineInputData* lineInputData;   // упала

        WorkModes mode;
    public:
        std::map<WorkModes, const char*> modeMap; 

    public:
        Controller();
        ~Controller();

        Lines& getLines();
        LineInputData* getLineInput();

        NodeGroup* getActiveNode();
        void setActiveNode(NodeGroup* object);
    
        NodeGroup* isObjectInSpace(const float& x, const float& y, const float& width, const float& height);    // check if object in space

        void translateObject(float relX, float relY, const float& wWidth, const float& wHeight);
        void rotateObject(float relX, float relY, const float& wWidth, const float& wHeight);

        void setMode(const WorkModes& mode);
        const WorkModes& getMode();

        void addLine(Line*);
        bool deleteLine(int idx);
        void drawLines();
        
        float producePixelCoordinatesToGL(float coord, int dimension);
    };

#endif
