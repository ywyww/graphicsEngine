#include "backend/Controller.h"
#include <fstream>

#ifndef SAVER_H
#define SAVER_H


class Saver
{
public:
    Saver();

    bool saveIntoAFile(std::string filename, const Controller* controller);
    bool readFromFile(std::string filename, Controller* controller);

};

#endif