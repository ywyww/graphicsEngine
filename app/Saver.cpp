#include "Saver.h"

Saver::Saver() {}

bool Saver::saveIntoAFile(std::string filename, const Model& model)
{
    try
    {
        std::ofstream ofs(filename, std::ios::out);
        boost::archive::text_oarchive oa(ofs);

        oa << model;

        ofs.close();
        return true;
    }
    catch (std::exception &e)
    {
        std::cout << e.what() << std::endl;
    }
    return false;
}

bool Saver::readFromAFile(std::string filename, Model& model)
{
    try
    {
        std::fstream ifs(filename, std::ios::in);
        boost::archive::text_iarchive ia(ifs);

        ia >> model;

        ifs.close();
        return true;
    }
    catch (std::exception &e)
    {
        std::cout << e.what() << std::endl;
    }
    return false;
}