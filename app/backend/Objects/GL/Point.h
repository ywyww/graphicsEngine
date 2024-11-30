#include <GL/gl.h>
#include <GLES3/gl3.h>
#include <glm/mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"
#include "../../Interfaces/Object.h"

#ifndef POINT_H
#define POINT_H

class Point: public Object {
    
    GLuint VAO;
    GLuint VBO;
    Shader shader;

    float* coords;      // 3 float.
    glm::vec3 color;

    float pointSize;

private:
    void Init();

public:
    Point();
    Point(const float& x, const float& y, const float& z);

    void setPointSize(const float& size);
    const float& getPointSize();

    void setColor(const glm::vec3& color);
    const glm::vec3& getColor();

    GLuint getVAO();
    GLuint getVBO();
    GLuint getShaderID();

    void updateBuffer(float* buff);     // 3 floats

    void draw() override;
};

#endif