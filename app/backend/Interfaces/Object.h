#include <glm/glm.hpp>

#ifndef OBJECT_H
#define OBJECT_H

class Object
{
protected:
    glm::mat4x4 transformation;

public:
    virtual void setTransformation(const glm::mat4x4& transformation)
    {
        this->transformation = transformation;
    }

    virtual glm::mat4x4& getTransformation()
    {
        return transformation;
    };

    virtual void draw() = 0;
};



#endif