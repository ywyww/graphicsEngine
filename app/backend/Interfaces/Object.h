#include <glm/glm.hpp>
#include <boost/serialization/access.hpp>
#include <boost/serialization/base_object.hpp>
#include <boost/serialization/split_member.hpp>
#include <boost/serialization/assume_abstract.hpp>

#ifndef OBJECT_H
#define OBJECT_H

class Object
{
    friend class boost::serialization::access;
    
    template <class Archive>
    void serialize(Archive& ar, const unsigned int version)
    {
        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                ar & transformation[i][j];
            }
        }
    }

    BOOST_SERIALIZATION_ASSUME_ABSTRACT( Object );

protected:
    glm::mat4x4 transformation;

public:
    virtual ~Object() { }

    virtual void setTransformation(const glm::mat4x4& transformation)
    {
        this->transformation = transformation;
    }

    virtual glm::mat4x4& getTransformation()
    {
        return transformation;
    };

    virtual void draw() = 0;
};



#endif