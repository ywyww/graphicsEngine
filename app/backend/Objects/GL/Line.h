#include <GL/gl.h>
#include <GLES3/gl3.h>
#include <glm/mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"

#ifndef LINES_H
#define LINES_H

class Line
{
    GLuint VAO;
    GLuint VBO;
    Shader shader;

    glm::mat4x4 transformation; // transformation matrix

    float buffer[6];  // 6 floats

public:
    Line(const float x1, const float y1, const float z1, const float x2, const float y2, const float z2);
    ~Line();

    GLuint getVAO();
    GLuint getVBO();
    GLuint getShaderID();

    void setTransformation(const glm::mat4x4&);
    glm::mat4x4& getTransformation();

    float* getBuffer();

    void draw();
};


#endif