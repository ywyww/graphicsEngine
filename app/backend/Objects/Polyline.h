#include <vector>

#include "GL/Line.h"
#include "../Interfaces/Object.h"
#include "../Translator.h"

#ifndef POLYLINE_H
#define POLYLINE_H

class Polyline: public Object
{
    std::vector<std::pair<float, float>> dots;
    std::vector<Line*> lines;
    
    std::pair<float, float> previousDot;

private:
   void addLine(float newDotX, float newDotY);  // if we have at least 2 points
public:
    Polyline();

    void addDot(float dotX, float dotY);

    // set transformation for current lines and for all 

    void draw() override;
};

#endif