#include <fstream>

#include "backend/Model.h"

#include <boost/archive/tmpdir.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>

#include <boost/serialization/base_object.hpp>
#include <boost/serialization/utility.hpp>
#include <boost/serialization/list.hpp>
#include <boost/serialization/assume_abstract.hpp>
#include <boost/serialization/vector.hpp>


#ifndef SAVER_H
#define SAVER_H


class Saver
{
public:
    Saver();

    static bool saveIntoAFile(std::string filename, const Model& model);
    static bool readFromAFile(std::string filename, Model& model);
};

#endif