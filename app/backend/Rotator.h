#include <iostream>

#include <glm/glm.hpp>

#include "Types/ViewState.h"


#ifndef ROTATOR_H
#define ROTATOR_H

struct Rotator
{
    glm::mat4 baseView;
    glm::mat4 cameraView;
    glm::mat4 settedView;

    ViewState viewState;

    Rotator();

    const glm::mat4& getCurrent();
    bool setCurrent(const glm::mat4& view);
};



#endif