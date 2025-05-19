#ifndef MATOR_H
#define MATOR_H


class Mathor
{
public:
    static float trimInBorders(float value, float leftBorder, float rightBorder)
    {
        if (value > rightBorder)
            return rightBorder;

        else if (value < leftBorder)
            return leftBorder;
        
        return value;
    }
};


#endif