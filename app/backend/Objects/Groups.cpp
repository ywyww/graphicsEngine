#include "Groups.h"

Groups::Groups()
{
    groups = std::vector<Nodes>();
    names = std::vector<std::string>();
}

std::vector<Nodes> Groups::getGroups()
{
    return groups;
}

std::vector<std::string> Groups::getNames()
{
    return names;
}

int Groups::getSize()
{
    return groups.size();
}

void Groups::addGroupAndName(Nodes group, std::string name)
{
    groups.push_back(group);
    names.push_back(name);
}

bool Groups::deleteGroupAndName(int idx)
{
    bool deleted = false;
    int cnt = 0;
    for (auto it = groups.begin();
        cnt <= groups.size() && it != groups.end();
        it++)
    {
        if (cnt == idx)
        {
            groups.erase(it);
            deleted = true;
        }
        cnt++;
    }
    cnt = 0;
    for (auto itn = names.begin(); 
        cnt <= names.size() && itn != names.end(); 
        itn++)
    {
        if (cnt == idx)
        {
            names.erase(itn);
            deleted = true;
        }
        cnt++;
    }
    return deleted;
}

std::pair<Nodes&, std::string&> Groups::getGroup(int idx)
{
    return std::pair<Nodes&, std::string&>(
        groups[idx], names[idx]
    );
}
