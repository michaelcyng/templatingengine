//
// Created by Cho Yiu Ng on 09.10.19.
//

#ifndef TEMPLATINGENGINE_SINGLEVALUEBASE_H
#define TEMPLATINGENGINE_SINGLEVALUEBASE_H

#include <dataTypes/ValueBase.h>

namespace templatingengine {

    class SingleValueBase : public ValueBase {
    public:

        bool isMultiValueType() const override { return false; }

    private:
    };
}

#endif //TEMPLATINGENGINE_SINGLEVALUEBASE_H
