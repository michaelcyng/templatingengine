//
// Created by Cho Yiu Ng on 12.10.19.
//

#ifndef TEMPLATINGENGINE_VALUEHANDLER_H
#define TEMPLATINGENGINE_VALUEHANDLER_H

#include <list>

#include <dataTypes/ValueBase.h>

namespace templatingengine {

    class ValueList;

    class ValueHandler {
    public:

        ValueHandler() = default;
        ValueHandler(const std::string& value); // NOLINT(google-explicit-constructor,hicpp-explicit-conversions)
        ValueHandler(ValueBasePtr_t valuePtr); // NOLINT(google-explicit-constructor,hicpp-explicit-conversions)

        ValueBasePtr_t&       operator->();
        const ValueBasePtr_t& operator->() const;
        ValueBase&            operator*();

        template <typename Container>
        ValueHandler& operator=(const Container& valueContainer) {
            std::list<ValueHandler> valueHandlerList;
            for (const auto& value: valueContainer) {
                valueHandlerList.emplace_back(value);
            }
            myValuePtr = std::static_pointer_cast<ValueBase>(std::make_shared<ValueList>(valueHandlerList));
            return *this;
        }

        operator const ValueBasePtr_t&() const { return myValuePtr; } // NOLINT(google-explicit-constructor,hicpp-explicit-conversions)
        operator ValueBasePtr_t&() { return myValuePtr; } // NOLINT(google-explicit-constructor,hicpp-explicit-conversions)

    private:
        ValueBasePtr_t myValuePtr;
    };

}

#endif //TEMPLATINGENGINE_VALUEHANDLER_H
