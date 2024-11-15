#include "Objects/Scene/Groups.h"
#include "Interfaces/Object.h"
#include "Types/Modes.h"
#include "Types/Data.h"
#include "Translator.h"
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

        void setMode(const WorkModes& mode);
        const WorkModes& getMode();

        void setCreationMode(const ObjectCreationModes& mode);
        const ObjectCreationModes& getCreationMode();

        void addLine(Line*);
        bool deleteLine(int idx);


        void trySetActiveNode(float lastClickedX, float lastClickedY, const float& wWidth, const float& wHeight);
        void translateObject(float relX, float relY, const float& wWidth, const float& wHeight);      // now only lines, then all objects.
        void rotateObject(float relX, float relY, const float& wWidth, const float& wHeight);
        void createObject(const float& x1, const float& y1, const float& x2, const float& y2, const float& wWidth, const float& wHeight);


        void drawLines();
    };

#endif
