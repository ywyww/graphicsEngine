#include "backend/Controller.h"
#include <fstream>

#ifndef SAVER_H
#define SAVER_H


class Saver
{
public:
    Saver();

    bool saveIntoAFile(std::string filename, const Model* model, const Controller* controller);
    bool readFromFile(std::string filename, Model* model, Controller* controller);

};

#endif