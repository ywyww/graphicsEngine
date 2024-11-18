
#ifndef TRANSLATOR_H
#define TRANSLATOR_H

class Translator
{
public:
    static float producePixelCoordinatesToGL(float coord, int dimension)
    {
        return coord * 2 / (dimension) - 1;
    }
};



#endif