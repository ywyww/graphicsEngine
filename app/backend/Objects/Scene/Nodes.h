#include <string>
#include <vector>

#include <glm/glm.hpp>
#include "../../Interfaces/Object.h"
#include "../GL/Point.h"
#include "../GL/Line.h"
#include "../Polyline.h"

#include <boost/serialization/access.hpp>

#ifndef NODE_GROUP_H
#define NODE_GROUP_H


enum ObjectType {
    NULLTYPE,
    POINT,
    LINE,
    POLYLINE
    GROUPMODE
};

struct NodeGroup
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

    NodeGroup()
    {
        node = nullptr;
        name = "";
        type = NULLTYPE;
    }
    
    NodeGroup& operator=(const NodeGroup other){
        if (this == &other)
            return *this;

        this->node = other.node;
        this->name = other.name;
        this->type = other.type;

        return *this;
    }
    virtual ~NodeGroup() = default;

};

typedef std::vector<NodeGroup> Nodes;

#endif