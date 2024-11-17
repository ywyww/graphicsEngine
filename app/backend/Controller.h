#include "Objects/Scene/Groups.h"
#include "Interfaces/Object.h"
#include "Types/Modes.h"
#include "Types/Data.h"
#include "Translator.h"
#include "Objects/Polyline.h"
#include <map>

#ifndef CONTROLLER_H
#define CONTROLLER_H

    class Controller
    {   
        // make structure for this objects.
        Nodes lines;
        Nodes polyLines;

        NodeGroup* activeNode;

        LineInputData* lineInputData;

        WorkModes mode;
        ObjectCreationModes creationMode;
    public:
        static std::map<WorkModes, const char*> modeMap; 
        static std::map<ObjectCreationModes, const char*> modeCreationMap;

    protected:
        void addLine(Line*);
        void addPolyLine(Polyline*);

    public:
        Controller();
        ~Controller();

        Nodes& getLines();
        Nodes& getPolylines();

        LineInputData* getLineInput();

        NodeGroup* getActiveNode();
        void setActiveNode(NodeGroup* object);
    
        void setMode(const WorkModes& mode);
        const WorkModes& getMode();

        void setCreationMode(const ObjectCreationModes& mode);
        const ObjectCreationModes& getCreationMode();

        bool deleteLine(int idx);
        bool deletePolyLine(int idx);


        NodeGroup* isObjectInSpace(const float& x, const float& y, const float& width, const float& height);    // check if object in space

        void trySetActiveNode(float lastClickedX, float lastClickedY, const float& wWidth, const float& wHeight);

        void translateObject(float relX, float relY, const float& wWidth, const float& wHeight);      // now only lines, then all objects.
        void rotateObject(float relX, float relY, const float& wWidth, const float& wHeight);
        void createObject(const float& x1, const float& y1, const float& x2, const float& y2, const float& wWidth, const float& wHeight);
        
        void createLine(const float& x1, const float& y1, const float& x2, const float& y2, const float& wWidth, const float& wHeight);
        Polyline* createPolyline();

        void addLineInPolyline(Polyline* polyline, const float& x1, const float& y1);

        void drawLines();
        void drawPolyLines();
    };

#endif
