//
// Created by thierrycd on 2023-11-16.
//

#include "Tokenizer.h"
#include <iostream>
#include <sstream>
#include <utility>


Tokenizer::Tokenizer(std::string data_) : m_data(std::move(data_))
{
    m_currentIndex = 0;
}

std::vector<Token> Tokenizer::tokenize()
{
    std::vector<Token> tokens{};

    while (peek().has_value())
    {
        char c = consume();
        std::stringstream buffer;

        if (std::isdigit(c))
        {
            buffer << c;

            while (peek().has_value() && std::isdigit(peek().value()))
            {
                buffer << consume();
            }

            tokens.emplace_back(TokenType::int_lit, buffer.str());
            buffer.clear();
        }
        else if (std::isalpha(c))
        {
            buffer << c;

            while (peek().has_value() && std::isalnum(peek().value()))
            {
                buffer << consume();
            }

            std::string out = buffer.str();

            if (out == "kill")
            {
                tokens.emplace_back(TokenType::kill);
            }
        }
        else if (c == ';')
        {
            tokens.emplace_back(TokenType::endl);
        }
        else if (std::isspace(c))
        {
            continue;
        }
        else
        {
            std::cerr << "Character does not correspond to any valid character: " << c << std::endl;
        }
    }

    return tokens;
}


char Tokenizer::consume()
{
    return m_data.at(m_currentIndex++);
}

std::optional<char> Tokenizer::peek(int offset_)
{
    if (m_currentIndex + offset_ < m_data.size())
    {
        return m_data.at(m_currentIndex + offset_);
    }

    return std::nullopt;
}
