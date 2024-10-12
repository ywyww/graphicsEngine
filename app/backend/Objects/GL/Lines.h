#include <GL/gl.h>
#include <GLES3/gl3.h>
#include <glm/mat4x4.hpp>

#include "Shader.h"

#ifndef LINES_H
#define LINES_H

class Line
{
    GLuint VAO;
    GLuint VBO;
    Shader shader;

    float* buffer;

public:
    Line(float x1, float y1, float x2, float y2);

    GLuint getVAO();
    GLuint getVBO();
    GLuint getShaderID();
};


#endif