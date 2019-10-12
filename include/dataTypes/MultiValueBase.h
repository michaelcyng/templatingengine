//
// Created by Cho Yiu Ng on 09.10.19.
//

#ifndef TEMPLATINGENGINE_MULTIVALUEBASE_H
#define TEMPLATINGENGINE_MULTIVALUEBASE_H

#include <dataTypes/ValueBase.h>

namespace templatingengine {

    class MultiValueBase : public ValueBase {
    public:

        bool isMultiValueType() const override { return true; }

    private:
    };
}

#endif //TEMPLATINGENGINE_MULTIVALUEBASE_H
