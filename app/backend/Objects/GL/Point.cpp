#include "Point.h"

void Point::Init()
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(0, sizeof(coords), glm::value_ptr(coords), GL_DYNAMIC_DRAW);   // fix size
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(coords), (void*)0);
        glEnableVertexAttribArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    shader = Shader("data/point.vertexshader", "data/point.fragmentshader");
}

Point::Point() : Object()
{
    transformation = glm::identity<glm::mat4x4>();
    color = glm::vec3(1.0f);
    coords = glm::vec3(0.0f);

    Init();
}

Point::Point(const float& x, const float& y, const float& z): Point()
{
    coords = glm::vec3(x, y, z);

    Init();
}

Point::Point(const glm::vec3& coords): Point()
{
    this->coords = coords;

    Init();
}

void Point::setPointSize(const float& size)
{
    this->pointSize = size;
}

const float& Point::getPointSize()
{
    return pointSize;
}

void Point::setColor(const glm::vec3& color)
{
    this->color = color;
}

const glm::vec3& Point::getColor()
{
    return color;
}

GLuint Point::getVAO()
{
    return VAO;
}

GLuint Point::getVBO()
{
    return VBO;
}

GLuint Point::getShaderID()
{
    return shader.getProgramID();
}

void Point::draw()
{
    glUseProgram(getShaderID());

    GLuint matrixName = glGetUniformLocation(getShaderID(), "MVP");
    glUniformMatrix4fv(matrixName, 1, GL_FALSE, glm::value_ptr(transformation));

    GLuint colorName = glGetUniformLocation(getShaderID(), "color");
    glUniform3fv(colorName, 1, glm::value_ptr(color));

    glBindVertexArray(VAO);
	glDrawArrays(GL_POINTS, 0, 1);
}