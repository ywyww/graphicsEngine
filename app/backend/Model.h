#include "Types/Modes.h"
#include "Types/Data.h"
#include "Objects/Scene/Groups.h"
#include "Objects/Polyline.h"
#include "SDL2/SDL_rect.h"
#include <map>

#ifndef MODEL_H
#define MODEL_H


class Model
{
    // doubling data.
    const SDL_Rect renderRect;
    const float shiftX;
    const float shiftY;
    const float windowWidth;    // viewport dimensions.
    const float windowHeight;   // viewport dimensions.

    float cursorX;
    float cursorY;


    // [START]
    // take it into a scene.

    Nodes lines;
    Nodes polyLines;

    // [END]

    NodeGroup* activeNode;  // make a massive


    WorkModes mode;
    ObjectCreationModes creationMode;
    
    public:
        static std::map<WorkModes, const char*> modeMap; 
        static std::map<ObjectCreationModes, const char*> modeCreationMap;

    public:
        Model(const SDL_Rect& renderArea);
        ~Model();

        const SDL_Rect& getRenderRect();

        const float& getCursorX();
        void setCursorX(const float& cursorX);

        const float& getCursorY();
        void setCursorY(const float& cursorY);

        const float& getWidth();
        const float& getHeight();

        Nodes& getLines();
        Nodes& getPolylines();

        NodeGroup* getActiveNode();
        void setActiveNode(NodeGroup* object);
    
        void setMode(const WorkModes& mode);
        const WorkModes& getMode();

        void setCreationMode(const ObjectCreationModes& mode);
        const ObjectCreationModes& getCreationMode();

        bool deleteLine(int idx);
        bool deletePolyLine(int idx);
        
        void addLine(Line*);
        void addPolyLine(Polyline*);
};


#endif