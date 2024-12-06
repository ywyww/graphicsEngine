#include <GL/gl.h>
#include <GLES3/gl3.h>
#include <glm/mat4x4.hpp>
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

    glm::vec3 color;

    float* buffer;  // 6 floats

public:
    Line();
    Line(const float x1, const float y1, const float z1, const float x2, const float y2, const float z2);
    ~Line();

    GLuint getVAO();
    GLuint getVBO();
    GLuint getShaderID();

    void setColor(const glm::vec3& color);
    
    bool isGLPointBelongs(const float& x, const float& y, const float z = 0);  // only 2d  // point in gl coordinates
    bool isPointBelongs(const float& x, const float& y, const float z, const float& wWidth, const float& wHeight, bool coefficientTrim = true, float precision=0.03f);   // only 2d  // point in human coordinates

    void updateBuffer(float* data); // 6 floats
    float* getBuffer();

    void draw() override;
};


#endif