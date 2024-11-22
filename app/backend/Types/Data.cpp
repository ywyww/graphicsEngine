#include "Data.h"
LineInputData::LineInputData()
{
    lineNameSize = 36;
    lineName = new char[lineNameSize] {};
    color = new float[3] {.5f, .5f, .5f};
    coordinates = new float[6] {.0f, .0f, .0f, .0f, .0f, .0f};
    translation = new float[3] {.0f, .0f, .0f};
    rotation = new float[3] {.0f, .0f, .0f};
    angle = 0.0f;
}
LineInputData::~LineInputData()
{
    delete[] coordinates;
    delete[] translation;
    delete[] rotation;
}
