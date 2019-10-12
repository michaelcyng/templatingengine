//
// Created by Cho Yiu Ng on 07.10.19.
//

#ifndef TEMPLATINGENGINE_STRINGVALUE_H
#define TEMPLATINGENGINE_STRINGVALUE_H

#include <string>

#include <dataTypes/SingleValueBase.h>

namespace templatingengine {

    class StringValue : public SingleValueBase {
    public:

        explicit StringValue(std::string value);

        const std::string& getValue()                const noexcept;
        void               printTo(std::ostream& os) const override;

    private:

        std::string myValue;

    };

}

#endif //TEMPLATINGENGINE_STRINGVALUE_H
