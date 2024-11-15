#include "Objects/Scene/Groups.h"
#include "Interfaces/Object.h"
#include "Types/Modes.h"
#include "Types/Data.h"
#include <map>

#ifndef CONTROLLER_H
#define CONTROLLER_H

    class Controller
    {   
        // make structure for this objects.
        Lines lines;
        std::string linesName;

        NodeGroup* activeNode;

        LineInputData* lineInputData;

        WorkModes mode;
        ObjectCreationModes creationMode;
    public:
        static std::map<WorkModes, const char*> modeMap; 
        static std::map<ObjectCreationModes, const char*> modeCreationMap;

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

        void setCreationMode(const ObjectCreationModes& mode);
        const ObjectCreationModes& getCreationMode();

        void addLine(Line*);
        bool deleteLine(int idx);
        void drawLines();
        
        float producePixelCoordinatesToGL(float coord, int dimension);
    };

#endif
