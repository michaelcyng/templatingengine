//
// Created by Cho Yiu Ng on 11.10.19.
//

#ifndef TEMPLATINGENGINE_ESCAPESTREAM_H
#define TEMPLATINGENGINE_ESCAPESTREAM_H

#include <istream>
#include <unordered_set>

namespace templatingengine {

    class EscapeStream {

        class Char {
        public:

            Char(char ch, bool escaped);

            char get() const noexcept;
            bool isEscaped() const noexcept;

        private:

            char myCh;
            bool myIsEscaped;

        };

    public:

        explicit EscapeStream(std::istream& stream);

        Char getChar();
        bool isEof() const noexcept;

    private:

        static const char                     ourEscapeChar;
        static const std::unordered_set<char> ourEscapedChars;

        std::istream& myStream;

    };

}

#endif //TEMPLATINGENGINE_ESCAPESTREAM_H
