#include "CoordinateSystem.h"

CoordinateSystem::CoordinateSystem()
{
    std::cout << "COORDINATE SYSTEM CONSTRUCTOR BEGIN" << std::endl;
    X = Line(0, 0, 0, 1, 0, 0);
    Y = Line(0, 0, 0, 0, 1, 0);
    Z = Line(0, 0, 0, 0, 0, 1);

    X.setColor(glm::vec3(1, 0, 0));
    Y.setColor(glm::vec3(0, 1, 0));
    Z.setColor(glm::vec3(0, 0, 1));
    std::cout << "COORDINATE SYSTEM CONSTRUCTOR END" << std::endl;
}

CoordinateSystem::CoordinateSystem(const CoordinateSystem& other)
{
    std::cout << "COORDINATE SYSTEM COPYING CONSTRUCTOR BEGIN" << std::endl;
    this->X = other.X;
    this->Y = other.Y;
    this->Z = other.Z;
    std::cout << "COORDINATE SYSTEM COPYING CONSTRUCTOR END" << std::endl;
}

CoordinateSystem::~CoordinateSystem()
{
    std::cout << "COORDINATE SYSTEM DESTRUCTOR BEGIN" << std::endl;
    std::cout << "COORDINATE SYSTEM DESTRUCTOR END" << std::endl;
}

void CoordinateSystem::setProjection(const glm::mat4& projection)
{
    X.setProjection(projection);
    Y.setProjection(projection);
    Z.setProjection(projection);
}

void CoordinateSystem::setView(const glm::mat4& view)
{
    X.setView(view);
    Y.setView(view);
    Z.setView(view);
}

void CoordinateSystem::draw()
{
    X.draw();
    Y.draw();
    Z.draw();
}