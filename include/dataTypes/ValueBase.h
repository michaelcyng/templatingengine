//
// Created by Cho Yiu Ng on 07.10.19.
//

#ifndef TEMPLATINGENGINE_VALUEBASE_H
#define TEMPLATINGENGINE_VALUEBASE_H

#include <memory>
#include <ostream>

namespace templatingengine {

    class ValueBase {
    public:

        virtual bool isMultiValueType() const = 0;
        virtual void printTo(std::ostream& os) const = 0;

    private:
    };

    typedef std::shared_ptr<ValueBase> ValueBasePtr_t;

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

std::ostream& operator<<(std::ostream&, const templatingengine::ValueBase& value);

#endif //TEMPLATINGENGINE_VALUEBASE_H
