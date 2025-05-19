#include <iostream>

#include <glm/glm.hpp>
#include "glm/gtc/matrix_transform.hpp"

#include "Types/ViewState.h"
#include "Types/ProjectionState.h"


#ifndef ROTATOR_H
#define ROTATOR_H

struct Rotator
{
    glm::mat4 viewXY;
    glm::mat4 viewXZ;
    glm::mat4 viewYZ;
    
    glm::mat4 view0;
    glm::mat4 view1;
    glm::mat4 view2;
    glm::mat4 view3;
    glm::mat4 view4;
    glm::mat4 view5;

    glm::mat4 projectionPerspective;
    glm::mat4 projectionOrtho;

    Rotator();

    const glm::mat4& getProjection(const ProjectionState& projectionState);

    const glm::mat4& getView(const ViewState& viewState);      // depends on viewState
    bool setView(const glm::mat4& view, const ViewState& viewState);
};



#endif