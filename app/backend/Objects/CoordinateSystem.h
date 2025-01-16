#include "GL/Line.h"
// #include "../Interfaces/Object.h"

#ifndef COORDINATE_SYSTEM_H
#define COORDINATE_SYSTEM_H

class CoordinateSystem
{
    Line X;
    Line Y;
    Line Z;

    glm::mat4 view;
    glm::mat4 projection;

public:
    CoordinateSystem();
    CoordinateSystem(const CoordinateSystem& other);
    ~CoordinateSystem();

    void setView(const glm::mat4& view);
    void setProjection(const glm::mat4& projection);

    void draw();

};







#endif