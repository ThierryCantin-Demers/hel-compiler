//
// Created by thierrycd on 2023-11-16.
//

#pragma once

#include "Token.h"

#include <optional>
#include <string>
#include <vector>

class Tokenizer
{
public:
    explicit Tokenizer(std::string data_);

    std::vector<Token> tokenize();

private:
    inline char consume();
    inline std::optional<char> peek(int offset_ = 0);

    std::string m_data;
    size_t m_currentIndex;
};
