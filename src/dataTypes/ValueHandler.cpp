//
// Created by Cho Yiu Ng on 12.10.19.
//

#include <dataTypes/StringValue.h>
#include <dataTypes/ValueHandler.h>

using namespace templatingengine;

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