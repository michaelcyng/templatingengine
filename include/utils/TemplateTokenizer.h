//
// Created by Cho Yiu Ng on 11.10.19.
//

#ifndef TEMPLATINGENGINE_TEMPLATETOKENIZER_H
#define TEMPLATINGENGINE_TEMPLATETOKENIZER_H

#include <istream>
#include <list>

namespace templatingengine {

    class TemplateTokenizer {
    public:

        typedef std::list<std::string> TokenTextList_t;

        static void tokenize(std::istream& inputStream, TokenTextList_t& tokenTextList);
    };

}

#endif //TEMPLATINGENGINE_TEMPLATETOKENIZER_H
