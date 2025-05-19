#include <vector>

#include "Polyline.h"
#include "Scene/Nodes.h"

#ifndef GROUP_H
#define GROUP_H

class Groups
{
    std::vector<Nodes> groups;
    std::vector<std::string> names;

public:
    Groups();
    
    std::vector<Nodes> getGroups();
    std::vector<std::string> getNames();
    int getSize();

    void addGroupAndName(Nodes group, std::string name);
    bool deleteGroupAndName(int idx);

    std::pair<Nodes&, std::string&> getGroup(int idx);
};

#endif