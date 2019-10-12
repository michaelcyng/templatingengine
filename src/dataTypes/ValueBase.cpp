//
// Created by Cho Yiu Ng on 07.10.19.
//

#include <dataTypes/ValueBase.h>

using namespace templatingengine;

std::ostream& operator<<(std::ostream& os, const ValueBase& value) {
    value.printTo(os);
    return os;
}
