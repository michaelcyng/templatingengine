//
// Created by Cho Yiu Ng on 07.10.19.
//

#include <dataTypes/ValueHandler.h>
#include <dataTypes/ValueList.h>

using namespace templatingengine;

ValueList::ValueList(const ValueHandler &valueHandler):
ValueList(std::static_pointer_cast<ValueList>(static_cast<ValueBasePtr_t>(valueHandler))->myValueList) {
}

ValueBasePtrListIter_t ValueList::begin() noexcept {
    return myValueList.begin();
}

ValueBasePtrListIter_t ValueList::end() noexcept {
    return myValueList.end();
}

bool ValueList::empty() const noexcept {
    return myValueList.empty();
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
