#include "backend/Controller.h"
#include <fstream>

#ifndef SAVER_H
#define SAVER_H


class Saver
{
public:
    Saver();

    static bool saveIntoAFile(std::string filename, const Model* model);
    static bool readFromFile(std::string filename, Model* model);

};

#endif