//
// Created by Cho Yiu Ng on 07.10.19.
//

#include <dataTypes/ValueList.h>

using namespace templatingengine;

ValueBasePtrListIter_t ValueList::begin() noexcept {
    return myValueList.begin();
}

ValueBasePtrListIter_t ValueList::end() noexcept {
    return myValueList.end();
}

void ValueList::printTo(std::ostream &os) const {
    os << "[";
    bool firstFlag = true;
    for (const auto& value: myValueList) {
        if (!firstFlag) {
            os << ", ";
        }
        value->printTo(os);
        firstFlag = false;
    }
    os << "]";
}
