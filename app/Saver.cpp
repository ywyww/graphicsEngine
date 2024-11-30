#include "Saver.h"

Saver::Saver() {}

bool Saver::saveIntoAFile(std::string filename, const Model* model)
{
    try
    {
        std::ofstream filestream = std::ofstream(filename, std::ios_base::out);

        filestream << model;

        filestream.close();

        return true;
    }
    catch (std::exception &e)
    {
        std::cout << e.what() << std::endl;
    }
    return false;
}

bool Saver::readFromFile(std::string filename, Model* model)
{
    // try
    // {
    //     std::fstream filestream = std::fstream(filename);

    //     while (!filestream.eof())
    //         filestream >> model;
    //     filestream.close();
    
    //     return true;
    // }
    // catch (std::exception &e)
    // {
    //     std::cout << e.what() << std::endl;
    // }

    return false;
}