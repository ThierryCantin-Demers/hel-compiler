//
// Created by thierrycd on 2023-11-16.
//

#pragma once

#include "Node.h"
#include "token/Token.h"

#include <vector>


class Parser
{
public:
    explicit Parser(std::vector<Token> tokens_);

    std::optional<NodeTerm> parseTerm();
    std::optional<NodeExpression> parseExpression();
    std::optional<NodeStatement> parseStatement();
    std::optional<NodeProgram> parseProgram();

private:
    inline Token& consume();
    inline std::optional<Token> peek(int offset_ = 0);

    std::vector<Token> m_tokens;
    size_t m_currentIndex;
};
