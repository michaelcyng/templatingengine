//
// Created by Cho Yiu Ng on 07.10.19.
//

#ifndef TEMPLATINGENGINE_STRINGLIST_H
#define TEMPLATINGENGINE_STRINGLIST_H

#include <list>

#include <dataTypes/MultiValueBase.h>

namespace templatingengine {


    typedef std::list<ValueBasePtr_t>    ValueBasePtrList_t;
    typedef ValueBasePtrList_t::iterator ValueBasePtrListIter_t;

    class ValueList : public MultiValueBase {
    public:

        ValueList() = default;

        template <typename ValueBaseContainer>
        explicit ValueList(const ValueBaseContainer& container) : myValueList(container.begin(), container.end()) {
        }

        ValueBasePtrListIter_t begin() noexcept;
        ValueBasePtrListIter_t end()   noexcept;
        void                   printTo(std::ostream& os) const override;

    private:

        ValueBasePtrList_t myValueList;

    };

}

#endif //TEMPLATINGENGINE_STRINGLIST_H
