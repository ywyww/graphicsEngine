#include "Rotator.h"


Rotator::Rotator()
{
    viewState = ViewState::XY;

    viewXY = glm::mat4(1.0f);
    viewXZ = glm::mat4(1.0f);
    viewYZ = glm::mat4(1.0f);

    viewXZ = glm::rotate(viewXZ, glm::radians(-90.0f), glm::vec3(1, 0, 0));
    viewYZ = glm::rotate(viewYZ, glm::radians(90.0f), glm::vec3(0, 1, 0));

    
    view0 = glm::mat4(1.0f);
    view1 = glm::mat4(1.0f);
    view2 = glm::mat4(1.0f);
    view3 = glm::mat4(1.0f);
    view4 = glm::mat4(1.0f);
    view5 = glm::mat4(1.0f);
}

const glm::mat4& Rotator::getCurrent()
{
    switch (viewState)
    {
        case ViewState::XY:
        {
            return viewXY;
        }
        case ViewState::XZ:
        {
            return viewXZ;
        }
        case ViewState::YZ:
        {
            return viewYZ;
        }
        case ViewState::VIEW_0:
        {
            return view0;
        }
        case ViewState::VIEW_1:
        {
            return view1;
        }
        case ViewState::VIEW_2:
        {
            return view2;
        }
        case ViewState::VIEW_3:
        {
            return view3;
        }
        case ViewState::VIEW_4:
        {
            return view4;
        }
        case ViewState::VIEW_5:
        {
            return view5;
        }
    }
    return viewXY;
}

bool Rotator::setCurrent(const glm::mat4& view)
{
    bool isSetted = true;
    switch (viewState)
    {
        case ViewState::VIEW_0:
        {
            view0 = view;
        }
        case ViewState::VIEW_1:
        {
            view1 = view;
        }
        case ViewState::VIEW_2:
        {
            view2 = view;
        }
        case ViewState::VIEW_3:
        {
            view3 = view;
        }
        case ViewState::VIEW_4:
        {
            view4 = view;
        }
        case ViewState::VIEW_5:
        {
            view5 = view;
        }
        default:
            std::cout << "VIEW NOT SETTED.";
            isSetted = false;
    }
    return isSetted;
}