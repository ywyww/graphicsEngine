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
}

void Polyline::addDot(float dotX, float dotY)      // plus add Line
{
    dots.push_back(std::pair(dotX, dotY));

    addLine(dotX, dotY);

    previousDot = std::pair(dotX, dotY);
}

void Polyline::setTransformation(const glm::mat4x4& transformation)
{
    this->transformation = transformation;
    for (int i = 0; i < lines.size(); i++)
    {
        lines[i]->setTransformation(transformation);
    }
}

void Polyline::draw()
{
    for (int i = 0; i < lines.size(); i++)
    {
        lines[i]->draw();
    }
}
