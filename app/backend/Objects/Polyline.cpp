#include "Polyline.h"


void Polyline::addLine(float newDotX, float newDotY)  // if we have at least 2 points
{
    if (dots.size() > 1)
    {
        std::cout << "dots: " << newDotX << " " << newDotY << " " << previousDot.first << " " << previousDot.second << std::endl;
        Line* line = new Line(
            Translator::producePixelCoordinatesToGL(previousDot.first, 1400),
            Translator::producePixelCoordinatesToGL(previousDot.second, 800),
            0,
            Translator::producePixelCoordinatesToGL(newDotX, 1400),
            Translator::producePixelCoordinatesToGL(newDotY, 800),
            0
        );
        lines.push_back(line);
    }
}

Polyline::Polyline(): Object()
{
    dots = std::vector<std::pair<float, float>>();
    lines = std::vector<Line*>();
    previousDot = std::pair(.0f, .0f);
    transformation = glm::identity<glm::mat4x4>();

    projection = glm::mat4(1.0f);
    view = glm::mat4(1.0f);
}

void Polyline::addDot(float dotX, float dotY)      // plus add Line
{
    dots.push_back(std::pair(dotX, dotY));

    addLine(dotX, dotY);

    previousDot = std::pair(dotX, dotY);
}

void Polyline::updateMVP()
{
    for (int i = 0; i < lines.size(); i++)
    {
        lines[i]->setTransformation(transformation);
        lines[i]->setProjection(projection);
        lines[i]->setView(view);
    }
}

void Polyline::setTransformation(const glm::mat4x4& transformation)
{
    this->transformation = transformation;
    updateMVP();
}

void Polyline::setProjection(const glm::mat4& projection)
{
    this->projection = projection;
    updateMVP();
}

void Polyline::setView(const glm::mat4& view)
{
    this->view = view;
    updateMVP();
}

bool Polyline::isPointBelongs(const float& x, const float& y, const float z, const float& width, const float& height, bool coefficientTrim, float precision)  // only 2d
{
    for (int i = 0; i < lines.size(); i++)
    {
        if (lines[i]->isPointBelongs(x, y, z, width, height, coefficientTrim, precision))
            return true;
    }
    return false;
}

void Polyline::draw()
{
    for (int i = 0; i < lines.size(); i++)
    {
        lines[i]->draw();
    }
}
