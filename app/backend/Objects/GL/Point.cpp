#include "Point.h"

void Point::Init()
{
    glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glBindVertexArray(VAO);

            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3, coords, GL_DYNAMIC_DRAW);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
            glEnableVertexAttribArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    shader = Shader("data/point.vertexshader", "data/point.fragmentshader");
}

Point::Point() : Object()
{
    transformation = glm::mat4(1.0f);
    color = glm::vec3(1.0f);
    coords = new float[3] {0, 0, 0};

    Init();
}

Point::Point(const float& x, const float& y, const float& z) : Object()
{
    transformation = glm::mat4(1.0f);
    color = glm::vec3(1.0f);
    coords = new float[3] {x, y, z};
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

void Point::updateBuffer(float* data)
{
    if (data != nullptr)
    {
        delete coords;
        coords = data;

        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3, data, GL_DYNAMIC_DRAW);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
            glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }
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