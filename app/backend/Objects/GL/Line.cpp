#include "Line.h"


Line::Line(): Object()
{
    buffer = new float[6] {0.0, 0.0, 0.0,
                           0.0, 0.0, 0.0};

        //if (x1 == x2 && y1 == y2 && z1 == z2)
        //    throw std::invalid_argument("Line consist of two point. Not one.");


        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glBindVertexArray(VAO);

            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6, buffer, GL_DYNAMIC_DRAW);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
            glEnableVertexAttribArray(0);

            glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

    shader = Shader("data/line.vertexshader", "data/line.fragmentshader");
    transformation = glm::identity<glm::mat4x4>();
    color = glm::vec3(1.0f, 0.5f, 0.2f);
}

Line::Line(const float x1, const float y1, const float z1, const float x2, const float y2, const float z2) : Object() {
        buffer = new float[6] {x1, y1, z1, 
                         x2, y2, z2};

        //if (x1 == x2 && y1 == y2 && z1 == z2)
        //    throw std::invalid_argument("Line consist of two point. Not one.");


        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glBindVertexArray(VAO);

            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6, buffer, GL_DYNAMIC_DRAW);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
            glEnableVertexAttribArray(0);

            glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        shader = Shader("data/line.vertexshader", "data/line.fragmentshader");
        transformation = glm::identity<glm::mat4x4>();
        color = glm::vec3(1.0f, 0.5f, 0.2f);
}

Line::~Line()
{
    delete[] buffer;
}


GLuint Line::getVAO() {
    return VAO;
}

GLuint Line::getVBO() {
    return VBO;
}

GLuint Line::getShaderID() {
    return shader.getProgramID();
}

float* Line::getBuffer()
{
    return buffer;
}

void Line::setColor(const glm::vec3& color)
{
    this->color = color;
}

void Line::updateBuffer(float* data)    // 6 floats
{
    if (data != nullptr)
    {
        delete buffer;
        buffer = data;

        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6, data, GL_DYNAMIC_DRAW);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
            glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

    }
}


bool Line::isGLPointBelongs(const float& x, const float& y, const float z)    // only 2d
{
    return false;
}

bool Line::isPointBelongs(const float& x, const float& y, const float z, const float& width, const float& height, bool coefficientTrim, float precision)  // only 2d
{
    // x(t) = x1 + (x2 - x1) * t, 0 <= t <= 1

    glm::vec4 transformedP1 = transformation * glm::vec4(buffer[0], buffer[1], 1, 1);
    glm::vec4 transformedP2 = transformation * glm::vec4(buffer[3], buffer[4], 1, 1);

    float x1Normal = width * (transformedP1[0] + 1) / 2;
    float y1Normal = height * (transformedP1[1] + 1) / 2;
     
    float x2Normal = width * (transformedP2[0] + 1) / 2;
    float y2Normal = height * (transformedP2[1] + 1) / 2;
        

    float xCoefficient = (x - x1Normal) / (x2Normal - x1Normal);
    float yCoefficient = (y - y1Normal) / (y2Normal - y1Normal);
    
    if (coefficientTrim)
    {
        if (xCoefficient > 1 || xCoefficient < -1 || yCoefficient > 1 || yCoefficient < -1)
            return false;
    }


    if (xCoefficient >= yCoefficient)
        return xCoefficient - yCoefficient < precision;
    else 
        return yCoefficient - xCoefficient < precision;
    
    // make z coodinate
}

void Line::draw()
{
    glUseProgram(getShaderID());

	GLuint matrixName = glGetUniformLocation(getShaderID(), "MVP");
    glUniformMatrix4fv(matrixName, 1, GL_FALSE, glm::value_ptr(transformation));

    GLuint colorName = glGetUniformLocation(getShaderID(), "color");
    glUniform3fv(colorName, 1, glm::value_ptr(color));

    glBindVertexArray(VAO);
	glDrawArrays(GL_LINES, 0, 2);
}


