#include <vector>

#include "GL/Line.h"
#include "../Interfaces/Object.h"

#ifndef POLYLINE_H
#define POLYLINE_H

class Polyline: public Object
{
    std::vector<std::pair<float, float>> dots;
    std::vector<Line*> lines;
    
    std::pair<float, float> previousDot;

private:
   void addLine(float newDotX, float newDotY)  // if we have at least 2 points
    {
        if (dots.size() > 1)
        {
            Line* line = new Line(previousDot.first, previousDot.second, 0, 
                                  newDotX, newDotY, 0);
            lines.push_back(line);
        }
    }
public:
    Polyline()
    {
        dots = std::vector<std::pair<float, float>>();
        lines = std::vector<Line*>();
        previousDot = std::pair(.0f, .0f);
    }

    void addDot(float dotX, float dotY)      // plus add Line
    {
        dots.push_back(std::pair(dotX, dotY));

        addLine(dotX, dotY);

        previousDot = std::pair(dotX, dotY);
    }

    // set transformation for current lines and for all 

    void draw() override
    {
        for (int i = 0; i < lines.size(); i++)
        {
            lines[i]->draw();
        }
    }

};



#endif