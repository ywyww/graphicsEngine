#include <vector>

#include "GL/Line.h"
#include "../Interfaces/Object.h"
#include "../Helpers/Translator.h"

#include <boost/serialization/access.hpp>

#ifndef POLYLINE_H
#define POLYLINE_H

class Polyline: public Object
{
    friend class boost::serialization::access;

    template <class Archive>
    void serialize(Archive& ar, const unsigned int version)
    {
        ar & boost::serialization::base_object<Object>(*this);

        ar & dots;
        ar & lines; 
        ar & previousDot;
    }

    std::vector<std::pair<float, float>> dots;
    std::vector<Line*> lines;
    
    std::pair<float, float> previousDot;

private:
   void addLine(float newDotX, float newDotY);  // if we have at least 2 points
public:
    Polyline();

    void addDot(float dotX, float dotY);

    void setTransformation(const glm::mat4x4& transformation) override;

    bool isPointBelongs(const float& x, const float& y, const float z, const float& wWidth, const float& wHeight, bool coefficientTrim = true, float precision=0.03f);   // only 2d  // point in human coordinates

    void draw() override;
};

#endif