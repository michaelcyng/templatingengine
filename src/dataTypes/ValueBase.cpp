//
// Created by Cho Yiu Ng on 07.10.19.
//

#include <dataTypes/ValueBase.h>
#include <dataTypes/StringValue.h>

using namespace templatingengine;

std::ostream& operator<<(std::ostream& os, const ValueBase& value) {
    value.printTo(os);
    return os;
}

ValueHandler::ValueHandler(const std::string& value): myValuePtr(std::make_shared<StringValue>(value)) {
}

ValueHandler::ValueHandler(ValueBasePtr_t valuePtr): myValuePtr(std::move(valuePtr)) {
}

ValueBasePtr_t& ValueHandler::operator->() {
    return myValuePtr;
}

const ValueBasePtr_t& ValueHandler::operator->() const {
    return myValuePtr;
}

ValueBase& ValueHandler::operator*() {
    return *myValuePtr;
}

ValueHandler& ValueHandler::operator=(templatingengine::ValueBasePtr_t valuePtr) {
    myValuePtr = std::move(valuePtr);
    return *this;
}
