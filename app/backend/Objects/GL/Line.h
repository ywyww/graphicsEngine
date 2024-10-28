#include <GL/gl.h>
#include <GLES3/gl3.h>
#include <glm/mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"
#include "../../Interfaces/Object.h"

#ifndef LINES_H
#define LINES_H

class Line: public Object
{
    GLuint VAO;
    GLuint VBO;
    Shader shader;

    float* buffer;  // 6 floats

public:
    Line(const float x1, const float y1, const float z1, const float x2, const float y2, const float z2);
    ~Line();

    GLuint getVAO();
    GLuint getVBO();
    GLuint getShaderID();

    bool isGLPointBelongs(const float x, const float y, const float z);  // only 2d  // point in gl coordinates
    bool isPointBelongs(const float x, const float y, const float z);   // only 2d  // point in human coordinates

    float* getBuffer();

    void draw() override;
};


#endif