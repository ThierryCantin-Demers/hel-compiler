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
    NodeProgram m_program;
    std::stringstream m_output;
    void generateStatement();
};
