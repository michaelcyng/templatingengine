//
// Created by Cho Yiu Ng on 11.10.19.
//

#include <utils/EscapeStream.h>

using namespace templatingengine;

const char EscapeStream::ourEscapeChar = '\\';
const std::unordered_set<char> EscapeStream::ourEscapedChars({EscapeStream::ourEscapeChar, '{'});

EscapeStream::Char::Char(char ch, bool escaped): myCh(ch), myIsEscaped(escaped) {
}

char EscapeStream::Char::get() const noexcept {
    return myCh;
}

bool EscapeStream::Char::isEscaped() const noexcept {
    return myIsEscaped;
}

EscapeStream::EscapeStream(std::istream &stream) : myStream(stream) {
}

EscapeStream::Char EscapeStream::getChar() {
    char ch;
    if (myStream.get(ch)) {

        //Ordinary characters
        if (ch != ourEscapeChar) {
            return {ch, false};
        }

        // Not escaping character
        if (myStream.eof() || ourEscapedChars.find(myStream.peek()) == ourEscapedChars.end()) {
            return {ch, false};
        }

        // Escaping character
        myStream.get(ch);
        return {ch, true};
    }
    return {std::char_traits<char>::eof(), false};
}

bool EscapeStream::isEof() const noexcept {
    return (myStream.eof() || myStream.peek() == std::char_traits<char>::eof());
}
