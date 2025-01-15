#include <GL/gl.h>
#include <GLES3/gl3.h>
#include <glm/mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"
#include "../../Interfaces/Object.h"

#include <boost/serialization/access.hpp>

#include <boost/archive/tmpdir.hpp>

#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>

#include <boost/serialization/base_object.hpp>
#include <boost/serialization/utility.hpp>
#include <boost/serialization/list.hpp>
#include <boost/serialization/assume_abstract.hpp>

#ifndef POINT_H
#define POINT_H

class Point: public Object {
    
    friend class boost::serialization::access;

    template <class Archive>
    void save(Archive& ar, const unsigned int version) const
    {
        ar & boost::serialization::base_object<Object>(*this);

        for (int i = 0; i < 3; i++)
            ar & coords[i];

        for (int i = 0; i < 3; i++)
            ar & color[i];

        ar & pointSize;

    }

    template <class Archive>
    void load(Archive& ar, const unsigned int version)
    {
        ar & boost::serialization::base_object<Object>(*this);

        for (int i = 0; i < 3; i++)
            ar & coords[i];

        for (int i = 0; i < 3; i++)
            ar & color[i];

        ar & pointSize;

        updateBuffer(coords);
    }
    
    BOOST_SERIALIZATION_SPLIT_MEMBER();

    GLuint VAO;
    GLuint VBO;
    Shader shader;

    float* coords;      // 3 float.
    glm::vec3 color;

    glm::mat4 projection;
    glm::mat4 view;

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

    void setProjection(const glm::mat4& projection);
    void setView(const glm::mat4& view);

    GLuint getVAO();
    GLuint getVBO();
    GLuint getShaderID();

    void updateBuffer(float* buff);     // 3 floats
    float* getBuffer();

    void draw() override;
};

#endif