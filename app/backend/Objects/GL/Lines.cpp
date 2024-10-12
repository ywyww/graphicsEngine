#include "Lines.h"

Line::Line(const float x1, const float y1, const float x2, const float y2) {
        buffer = new float[6]{x1, y1, 0.5f, 
                              x2, y2, 1.0f};

        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);

            glGenBuffers(1, &VBO);
            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(buffer), buffer, GL_STATIC_DRAW);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
            glEnableVertexAttribArray(0);

            glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        shader = Shader("data/line.vertexshader", "data/line.fragmentshader");
        transformation = glm::mat4x4(1.0f);
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

void Line::setTransformation(const glm::mat4x4& other)
{
    transformation = other;
}

void Line::draw()
{
    glUseProgram(getShaderID());

	GLuint matrixName = glGetUniformLocation(getShaderID(), "MVP");
    glUniformMatrix4fv(matrixName, 1, GL_FALSE, glm::value_ptr(transformation));

    glBindVertexArray(getVAO());
	glDrawArrays(GL_LINES, 0, 2);
}

