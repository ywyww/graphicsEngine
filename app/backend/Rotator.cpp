#include "Rotator.h"


Rotator::Rotator()
{
    viewState = ViewState::BASE;
    baseView = glm::mat4(1.0f);
    cameraView = glm::mat4(1.0f);
    settedView = glm::mat4(1.0f);
}

const glm::mat4& Rotator::getCurrent()
{
    switch (viewState)
    {
        case ViewState::BASE:
        {
            return baseView;
        }
        case ViewState::CAMERA:
        {
            return cameraView;
        }
        case ViewState::SETTED:
        {
            return settedView;
        }
    }
    return baseView;
}

bool Rotator::setCurrent(const glm::mat4& view)
{
    bool isSetted = false;
    switch (viewState)
    {
        case ViewState::CAMERA:
        {
            cameraView = view;
            isSetted = true;
            break;
        }
        case ViewState::SETTED:
        {
            settedView = view;
            isSetted = true;
            break;
        }
        default:
            std::cout << "VIEW NOT SETTED.";
    }
    return isSetted;
}