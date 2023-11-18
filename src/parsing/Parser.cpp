//
// Created by thierrycd on 2023-11-16.
//

#include "Parser.h"
#include <iostream>
#include <utility>


Parser::Parser(std::vector<Token> tokens_) : m_tokens(std::move(tokens_))
{
    m_currentIndex = 0;
}

std::optional<NodeTerm> Parser::parseTerm()
{
    if (peek().has_value() && peek().value().type == TokenType::int_lit)
    {
        Token intLiteral = consume();
        NodeTerm term = NodeTerm({.var = NodeTermIntLiteral{.int_literal = intLiteral}});

        return term;
    }
    else if (peek().has_value() && peek().value().type == TokenType::ident)
    {
        Token ident = consume();
        NodeTerm term = NodeTerm({.var = NodeTermIdentifier{.identifier = ident}});

        return term;
    }

    return std::nullopt;
}

std::optional<NodeExpression> Parser::parseExpression()
{
    std::optional<NodeTerm> term = parseTerm();
    if (term.has_value())
    {
        return NodeExpression{.var = term.value()};
    }

    return std::nullopt;
}

std::optional<NodeStatement> Parser::parseStatement()
{
    if (peek().has_value() && peek().value().type == TokenType::kill)
    {
        // kill
        consume();

        if (std::optional<NodeExpression> exp = parseExpression())
        {
            NodeStatementKill statementKill = NodeStatementKill({.expression = exp.value()});
            NodeStatement statement = NodeStatement({.var = statementKill});

            // ^
            consume();

            return statement;
        }
        else
        {
            std::cerr << "Invalid expression" << std::endl;
            exit(EXIT_FAILURE);
        }
    }
    else if (peek().has_value() && peek().value().type == TokenType::variable &&
             peek(1).has_value() && peek(1).value().type == TokenType::ident &&
             peek(2).has_value() && peek(2).value().type == TokenType::assign)
    {
        // thing keyword
        consume();

        // identifier
        Token identifier = consume();

        // =
        consume();

        if (auto expr = parseExpression())
        {
            NodeStatementAssign statementAssign = NodeStatementAssign({.identifier = identifier, .expression= expr.value()});
            NodeStatement statement = NodeStatement({.var=statementAssign});

            // ^
            consume();

            return statement;
        }
        else
        {
            std::cerr << "Invalid expression" << std::endl;
            exit(EXIT_FAILURE);
        }
    }

    return std::nullopt;
}

std::optional<NodeProgram> Parser::parseProgram()
{
    NodeProgram root{};

    while (peek().has_value())
    {
        if (std::optional<NodeStatement> statement = parseStatement())
        {
            root.statements.push_back(statement.value());
        }
        else
        {
            std::cerr << "Invalid statement" << std::endl;
            return std::nullopt;
        }
    }

    return root;
}

Token& Parser::consume()
{
    return m_tokens.at(m_currentIndex++);
}

std::optional<Token> Parser::peek(int offset_)
{
    if (m_currentIndex + offset_ < m_tokens.size())
    {
        return m_tokens.at(m_currentIndex + offset_);
    }

    return {};
}
