#include "CoordinateSystem.h"

CoordinateSystem::CoordinateSystem(): Object()
{
    X = new Line(0, 0, 0, 1, 0, 0);
    Y = new Line(0, 0, 0, 0, 1, 0);
    Z = new Line(0, 0, 0, 0, 0, 1);

    X->setColor(glm::vec3(1, 0, 0));
    Y->setColor(glm::vec3(0, 1, 0));
    Z->setColor(glm::vec3(0, 0, 1));
}

CoordinateSystem::~CoordinateSystem()
{
    // delete X;
    // delete Y;
    // delete Z;
}

void CoordinateSystem::setProjection(const glm::mat4& projection)
{
    X->setProjection(projection);
    Y->setProjection(projection);
    Z->setProjection(projection);
}

void CoordinateSystem::setView(const glm::mat4& view)
{
    X->setView(view);
    Y->setView(view);
    Z->setView(view);
}

void CoordinateSystem::draw()
{
    X->draw();
    Y->draw();
    Z->draw();
}