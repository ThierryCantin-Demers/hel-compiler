//
// Created by thierrycd on 2023-11-16.
//

#pragma once

#include "token/Token.h"
#include <variant>
#include <vector>


struct NodeTermIntLiteral
{
    Token int_literal;
};

struct NodeTermIdentifier
{
    Token identifier;
};

struct NodeTerm
{
    std::variant<NodeTermIntLiteral /*, NodeTermIdentifier*/> var;
};

struct NodeExpression
{
    std::variant<NodeTerm> var;
};

struct NodeStatementKill
{
    NodeExpression expression;
};

struct NodeStatement
{
    std::variant<NodeStatementKill> var;
};

struct NodeProgram
{
    std::vector<NodeStatement> statements;
};
