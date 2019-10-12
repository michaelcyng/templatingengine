//
// Created by Cho Yiu Ng on 11.10.19.
//

#include <sstream>

#include <utils/EscapeStream.h>
#include <utils/TemplateTokenizer.h>

using namespace templatingengine;

void TemplateTokenizer::tokenize(std::istream &inputStream, std::list<std::string> &tokenTextList) {
    EscapeStream escapedInputStream(inputStream);

    std::stringstream buffer;
    while (!escapedInputStream.isEof()) {
        auto ch = escapedInputStream.getChar();

        // Escaped character
        if (ch.isEscaped()) {
            buffer << ch.get();
            continue;
        }

        // Not { so just push into buffer
        if (ch.get() != '{') {
            buffer << ch.get();
            continue;
        }

        // Now, we have a {. Look forward and check if the next one is also a {
        if (escapedInputStream.isEof()) {
            buffer << ch.get();
            break;
        }
        auto nextCh = escapedInputStream.getChar();
        if (nextCh.get() != '{') {
            buffer << ch.get() << nextCh.get();
        }
        else {
            std::stringstream commandBuffer;
            commandBuffer << "{{";
            int closingCount = 0;
            while (!escapedInputStream.isEof() && closingCount < 2) {
                auto ch = escapedInputStream.getChar();
                commandBuffer << ch.get();
                if (ch.isEscaped() || ch.get() != '}') {
                    closingCount = 0;
                }
                else {
                    ++closingCount;
                }
            }

            if (closingCount == 2) {
                if (buffer.str().length() > 0) {
                    tokenTextList.emplace_back(buffer.str());
                    buffer.str("");
                }
                tokenTextList.emplace_back(commandBuffer.str());
            }
            else {
                buffer << commandBuffer.str();
                tokenTextList.emplace_back(buffer.str());
                buffer.str("");
            }
        }
    }

    if (buffer.str().length() > 0) {
        tokenTextList.emplace_back(buffer.str());
    }
}
