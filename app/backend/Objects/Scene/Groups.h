#include <string>
#include <vector>

#include <glm/glm.hpp>
#include "../../Interfaces/Object.h"
#include "../GL/Point.h"
#include "../GL/Line.h"
#include "../Polyline.h"

#include <boost/serialization/access.hpp>

#ifndef LINE_GROUP_H
#define LINE_GROUP_H


enum ObjectType {
    NULLTYPE,
    POINT,
    LINE,
    POLYLINE
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

// std::ostream& operator<<(std::ostream& os, NodeGroup& node)
// {
//     if (node.type == ObjectType::POINT)
//         os << *(dynamic_cast<Point*>(node.node)) << " " << node.name << " " << node.type << "\n";

//     else if (node.type == ObjectType::LINE)
//         os << *(dynamic_cast<Line*>(node.node)) << " " << node.name << " " << node.type << "\n";

//     else if (node.type == ObjectType::POLYLINE)
//         os << *(dynamic_cast<Polyline*>(node.node)) << " " << node.name << " " << node.type << "\n";
    
//     return os;
// }

// std::istream& operator>>(std::istream& is, NodeGroup& node)
// {
//     int type;
//     is >> *(node.node) >> node.name >> type;
//     node.type = static_cast<ObjectType>(type);

//     return is;
// }


// std::ostream& operator<<(std::ostream& os, Nodes& nodes)
// {
//     for (int i = 0; i< nodes.size(); i++)
//         os << nodes[i] << " ";

//     return os;
// }

#endif