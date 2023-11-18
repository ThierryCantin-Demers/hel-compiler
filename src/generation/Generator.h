//
// Created by thierrycd on 2023-11-16.
//

#pragma once

#include "parsing/Node.h"
#include <sstream>


class Generator
{
public:
    explicit Generator(NodeProgram program_);

    void generateTerm(NodeTerm term_);
    void generateExpression(NodeExpression expression_);
    void generateStatement(NodeStatement statement_);
    std::string generateProgram();

private:
    void pop(const std::string& reg_);
    void push(const std::string& reg_);

    struct Variable{
        std::string name;
        size_t stackLocation;
    };

    NodeProgram m_program;
    std::stringstream m_output;

    std::vector<Variable> m_variables;
    size_t m_stackSize;
};
