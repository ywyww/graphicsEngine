#include "Objects/Scene/Groups.h"
#include "Interfaces/Object.h"

#ifndef CONTROLLER_H
#define CONTROLLER_H

    struct LineInputData
    {
        float* coordinates; // 6 (x, y, z) * 2

        float* translation; // 3 x y z
        float* rotation; // 3 x y z
        float angle;

    public:
        LineInputData();
        ~LineInputData();
    };

    class Controller
    {   
        // make structure for this objects.
        Lines lines;
        std::string linesName;

        NodeGroup<Object>* activeNode;

        LineInputData* lineInputData; 

    public:
        Controller();
        ~Controller();

        Lines& getLines();
        LineInputData* getLineInput();
        
        void addLine(Line*);
        void drawLines();
        
        float producePixelCoordinatesToGL(float coord, int dimension);

    };

#endif
