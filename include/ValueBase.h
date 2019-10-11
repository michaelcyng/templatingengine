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
}

std::ostream& operator<<(std::ostream&, const templatingengine::ValueBase& value);

#endif //TEMPLATINGENGINE_VALUEBASE_H
