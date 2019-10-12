//
// Created by Cho Yiu Ng on 12.10.19.
//

#ifndef TEMPLATINGENGINE_VALUEHANDLER_H
#define TEMPLATINGENGINE_VALUEHANDLER_H

#include <list>

#include <dataTypes/ValueBase.h>
#include <dataTypes/ValueList.h>

namespace templatingengine {

    class ValueHandler {
    public:

        ValueHandler() = default;
        ValueHandler(const char* value); // NOLINT(google-explicit-constructor,hicpp-explicit-conversions)
        ValueHandler(const std::string& value); // NOLINT(google-explicit-constructor,hicpp-explicit-conversions)
        ValueHandler(ValueBasePtr_t valuePtr); // NOLINT(google-explicit-constructor,hicpp-explicit-conversions)

        // Convert from container type to ValueHandler which contains a ValueList shared pointer
        template <typename Container,
                std::enable_if_t<std::is_convertible<typename Container::value_type, ValueHandler>::value, int> = 0>
        ValueHandler(const Container& valueContainer):
            myValuePtr(std::static_pointer_cast<ValueBase>(std::make_shared<ValueList>(
                    std::list<ValueHandler>(valueContainer.begin(), valueContainer.end())))) {
        }

        // Support for conversion from initializer list as it cannot be inferred as template argument
        template <typename T, std::enable_if_t<std::is_convertible<T, ValueHandler>::value, int> = 0 >
        ValueHandler(const std::initializer_list<T>& valueList):
                myValuePtr(std::static_pointer_cast<ValueBase>(std::make_shared<ValueList>(
                        std::list<ValueHandler>(valueList.begin(), valueList.end())))) {
        }

        ValueBasePtr_t&       operator->();
        const ValueBasePtr_t& operator->() const;
        ValueBase&            operator*();

        ValueHandler& operator=(const ValueHandler& value);

        operator const ValueBasePtr_t&() const { return myValuePtr; } // NOLINT(google-explicit-constructor,hicpp-explicit-conversions)
        operator ValueBasePtr_t&() { return myValuePtr; } // NOLINT(google-explicit-constructor,hicpp-explicit-conversions)

    private:
        ValueBasePtr_t myValuePtr;
    };

}

#endif //TEMPLATINGENGINE_VALUEHANDLER_H
