#include <cstddef>
#ifndef DATA_H
#define DATA_H

struct LineInputData
{
    float* coordinates; // 6 (x, y, z) * 2
    float* color;       // r g b

    float* translation; // 3 x y z
    float* rotation; // 3 x y z
    float angle;

    char* lineName;
    size_t lineNameSize;

public:
    LineInputData();
    ~LineInputData();
};

#endif