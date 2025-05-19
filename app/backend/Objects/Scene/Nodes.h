#include <string>
#include <vector>

#include <glm/glm.hpp>

#include "../../Interfaces/Object.h"
#include "../GL/Point.h"
#include "../GL/Line.h"
#include "../Polyline.h"
#include "../../Types/ObjectType.h"

#include <boost/serialization/access.hpp>

#ifndef NODE_GROUP_H
#define NODE_GROUP_H

struct Node
{
    friend class boost::serialization::access;

    template <class Archive>
    void save(Archive& ar, const unsigned int version) const
    {
        ar.template register_type<Point>();
        ar.template register_type<Line>();
        ar.template register_type<Polyline>();

        ar & node;
        ar & name;
        ar & type;
    }

    template <class Archive>
    void load(Archive& ar, const unsigned int version)
    {
        ar.template register_type<Point>();
        ar.template register_type<Line>();
        ar.template register_type<Polyline>();

        ar & node;
        ar & name;
        ar & type;
    }

    BOOST_SERIALIZATION_SPLIT_MEMBER();


    Object* node;
    std::string name;
    ObjectType type;

    Node()
    {
        node = nullptr;
        name = "";
        type = NULLTYPE;
    }
    
    Node& operator=(const Node other){
        if (this == &other)
            return *this;

        this->node = other.node;
        this->name = other.name;
        this->type = other.type;

        return *this;
    }
    virtual ~Node() = default;

};

typedef std::vector<Node> Nodes;

#endif