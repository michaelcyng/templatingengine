//
// Created by Cho Yiu Ng on 07.10.19.
//

#ifndef TEMPLATINGENGINE_PARAMETERSET_H
#define TEMPLATINGENGINE_PARAMETERSET_H

#include <unordered_map>

#include <dataTypes/ValueBase.h>

namespace templatingengine {

    typedef std::unordered_map<std::string, ValueBasePtr_t> ParameterSet;

}

#endif //TEMPLATINGENGINE_PARAMETERSET_H
