#include <GL/gl.h>
#include <GLES3/gl3.h>
#include <glm/mat4x4.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"
#include "../../Interfaces/Object.h"

#include <boost/serialization/access.hpp>
#include <boost/serialization/split_member.hpp>

#ifndef LINES_H
#define LINES_H

class Line: public Object
{
    friend class boost::serialization::access;
    
    template <class Archive>
    void save(Archive& ar, const unsigned int version) const
    {
        ar & boost::serialization::base_object<Object>(*this);
        
        for (int i = 0; i < 6; i++)
            ar & buffer[i];

        for (int i = 0; i < 3; i++)
            ar & color[i];

    }

    template <class Archive>
    void load(Archive& ar, const unsigned int version)
    {
        ar & boost::serialization::base_object<Object>(*this);
        
        for (int i = 0; i < 6; i++)
            ar & buffer[i];

        for (int i = 0; i < 3; i++)
            ar & color[i];

        updateBuffer(buffer);
    }

    BOOST_SERIALIZATION_SPLIT_MEMBER();

    GLuint VAO;
    GLuint VBO;
    Shader shader;

    glm::mat4 projection;
    glm::mat4 view;

    glm::vec3 color;

    float* buffer;  // 6 floats

public:
    Line();
    Line(const float x1, const float y1, const float z1, const float x2, const float y2, const float z2);
    Line(const Line& other);
    ~Line();

    GLuint getVAO();
    GLuint getVBO();
    GLuint getShaderID();

    void setColor(const glm::vec3& color);

    void setProjection(const glm::mat4& projection);
    void setView(const glm::mat4& view);
    
    bool isGLPointBelongs(const float& x, const float& y, const float z = 0);  // only 2d  // point in gl coordinates
    bool isPointBelongs(const float& x, const float& y, const float z, const float& wWidth, const float& wHeight, bool coefficientTrim = true, float precision=0.03f);   // only 2d  // point in human coordinates

    void updateBuffer(float* data); // 6 floats
    float* getBuffer();

    void draw() override;

    Line& operator=(const Line& other)
    {
        std::cout << "OPERATOR= OF LINE. " << std::endl;
        if (buffer != nullptr)
        {
            delete[] buffer;
            buffer = new float[6]{};
        }

        for (int i = 0; i < 6; i++)
            buffer[i] = other.buffer[i];

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
        transformation = other.transformation;
        color = other.color;

        projection = other.projection;
        view = other.view;

        std::cout << "OPERATOR= OF LINE. END " << std::endl;
    
        return *this;
    }
};


#endif