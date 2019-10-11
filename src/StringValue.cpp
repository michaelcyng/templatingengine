//
// Created by Cho Yiu Ng on 07.10.19.
//

#include <StringValue.h>

using namespace templatingengine;

StringValue::StringValue(std::string value) : myValue(std::move(value)) {
}

const std::string& StringValue::getValue() const noexcept {
    return myValue;
}

void StringValue::printTo(std::ostream &os) const {
    os << myValue;
}
