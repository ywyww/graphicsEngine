#include <SDL2/SDL_rect.h>
#include <map>

#include "Types/Modes.h"
#include "Types/Data.h"

#include "Objects/Scene/Nodes.h"
#include "Objects/GL/Point.h"
#include "Objects/GL/Line.h"
#include "Objects/Polyline.h"
#include "Objects/Groups.h"

#include <boost/serialization/access.hpp>

#ifndef MODEL_H
#define MODEL_H


class Model
{
    friend class boost::serialization::access;

    template <class Archive>
    void serialize(Archive& ar, const unsigned int version)
    {
        ar & renderRect.w;
        ar & renderRect.h;
        ar & renderRect.x;
        ar & renderRect.y;

        ar & shiftX;
        ar & shiftY;

        ar & windowWidth;
        ar & windowHeight;
        
        ar & cursorX;
        ar & cursorY;

        ar & centerX;
        ar & centerY;

        ar & activeNodeType;
        ar & mode;

        ar & points;

        ar & lines;

        ar & polyLines;
    }

    // doubling data.
    SDL_Rect renderRect;
    float shiftX;
    float shiftY;
    float windowWidth;    // viewport dimensions.
    float windowHeight;   // viewport dimensions.

    float cursorX;
    float cursorY;

    float centerX;
    float centerY;


    // [START]
    // take it into a scene.

    Nodes points;
    Nodes lines;
    Nodes polyLines;
    Groups groups;

    // [END]

    NodeGroup* activeNode;  // make a massive
    ObjectType activeNodeType;


    WorkModes mode;
    
    public:
        static std::map<WorkModes, const char*> modeMap; 

    public:
        Model(const SDL_Rect& renderArea);
        ~Model();

        const SDL_Rect& getRenderRect();

        const float& getCursorX();
        void setCursorX(const float& cursorX);

        const float& getCursorY();
        void setCursorY(const float& cursorY);

        const float& getCenterX();
        void setCenterX(const float& x);

        const float& getCenterY();
        void setCenterY(const float& y);

        const float& getWidth();
        const float& getHeight();

        void setMode(const WorkModes& mode);
        const WorkModes& getMode();

        Nodes& getPoints();
        Nodes& getLines();
        Nodes& getPolylines();
        Groups& getGroups();

        NodeGroup* getActiveNode();
        void setActiveNode(NodeGroup* object);
        
        const ObjectType& getActiveNodeType();
    
        void addPoint(Point* point);
        bool deletePoint(int idx);
        
        void addLine(Line*);
        bool deleteLine(int idx);
        
        void addPolyLine(Polyline*);
        bool deletePolyLine(int idx);

        void addGroup(Nodes group);
        bool deleteGroup(int idx);

        //friend std::ostream& operator<<(std::ostream& os, const Model& model);
        //friend std::istream& operator>>(std::istream& is, Model& model);
};


#endif