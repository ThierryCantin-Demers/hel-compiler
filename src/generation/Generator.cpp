//
// Created by thierrycd on 2023-11-16.
//

#include "Generator.h"
#include <iostream>
#include <utility>

Generator::Generator(NodeProgram program_) : m_program(std::move(program_))
{
    m_output = std::stringstream{};
    m_variables = {};
    m_stackSize = 0;
}

void Generator::generateTerm(NodeTerm term_)
{
    struct TermVisitor
    {
        Generator& gen;
        void operator()(const NodeTermIntLiteral& intLiteral_)
        {
            gen.m_output << "    mov rax, " << intLiteral_.int_literal.val.value() << "\n";
            gen.push("rax");
        }

        void operator()(const NodeTermIdentifier& identifier_)
        {
            auto varsIt = std::find_if(gen.m_variables.begin(),
                                       gen.m_variables.end(),
                                       [&](const Variable& var) { return var.name == identifier_.identifier.val.value(); });

            if (varsIt == gen.m_variables.end())
            {
                std::cerr << "Variable `" << identifier_.identifier.val.value() << "` has not been defined yet." << std::endl;
                exit(EXIT_FAILURE);
            }

            std::stringstream offset;
            offset << " QWORD [rsp + " << (gen.m_stackSize - varsIt->stackLocation - 1) * 8 << "]";
            gen.push(offset.str());
        }
    };

    TermVisitor visitor{.gen = *this};
    std::visit(visitor, term_.var);
}

void Generator::generateExpression(NodeExpression expression_)
{
    struct ExpressionVisitor
    {
        Generator& gen;
        void operator()(const NodeTerm& expressionTerm)
        {
            gen.generateTerm(expressionTerm);
        }
    };

    ExpressionVisitor visitor{.gen = *this};
    std::visit(visitor, expression_.var);
}

void Generator::generateStatement(NodeStatement statement_)
{
    struct StatementVisitor
    {
        Generator& gen;
        void operator()(const NodeStatementKill& statementKill)
        {
            gen.generateExpression(statementKill.expression);
            gen.m_output << "    mov rax, 60\n";
            gen.m_output << "    pop rdi\n";
            gen.m_output << "    syscall\n";
        }

        void operator()(const NodeStatementAssign& statementAssign)
        {
            auto varIt = std::find_if(gen.m_variables.begin(),
                                      gen.m_variables.end(),
                                      [&](const Variable& var) { return var.name == statementAssign.identifier.val.value(); });

            if (varIt != gen.m_variables.end())
            {
                std::cerr << "Variable `" << statementAssign.identifier.val.value() << "` was already declared." << std::endl;
                exit(EXIT_FAILURE);
            }

            Variable var = {.name=statementAssign.identifier.val.value(), .stackLocation=gen.m_stackSize};
            gen.m_variables.push_back(var);
            gen.generateExpression(statementAssign.expression);
        }
    };

    StatementVisitor visitor{.gen = *this};
    std::visit(visitor, statement_.var);
}

std::string Generator::generateProgram()
{
    m_output << "global _start\n";
    m_output << "_start:\n";

    for (const NodeStatement& statement: m_program.statements)
    {
        generateStatement(statement);
    }

    m_output << "    mov rax, 60\n";
    m_output << "    mov rdi, 0\n";
    m_output << "    syscall\n";

    return m_output.str();
}

void Generator::push(const std::string& reg_)
{
    m_output << "    push " << reg_ << "\n";
    m_stackSize++;
}


void Generator::pop(const std::string& reg_)
{
    m_output << "    pop " << reg_ << "\n";
    m_stackSize--;
}