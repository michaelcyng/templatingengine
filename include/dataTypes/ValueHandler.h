//
// Created by Cho Yiu Ng on 12.10.19.
//

#ifndef TEMPLATINGENGINE_VALUEHANDLER_H
#define TEMPLATINGENGINE_VALUEHANDLER_H

#include <dataTypes/ValueBase.h>

namespace templatingengine {

    class ValueHandler {
    public:

        ValueHandler() = default;
        ValueHandler(const std::string& value); // NOLINT(google-explicit-constructor,hicpp-explicit-conversions)
        ValueHandler(ValueBasePtr_t valuePtr); // NOLINT(google-explicit-constructor,hicpp-explicit-conversions)

        ValueBasePtr_t&       operator->();
        const ValueBasePtr_t& operator->() const;
        ValueBase&            operator*();
        ValueHandler&         operator=(ValueBasePtr_t valuePtr);

        operator const ValueBasePtr_t&() const { return myValuePtr; } // NOLINT(google-explicit-constructor,hicpp-explicit-conversions)
        operator ValueBasePtr_t&() { return myValuePtr; } // NOLINT(google-explicit-constructor,hicpp-explicit-conversions)

    private:
        ValueBasePtr_t myValuePtr;
    };

}

#endif //TEMPLATINGENGINE_VALUEHANDLER_H
