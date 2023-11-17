//
// Created by thierrycd on 2023-11-16.
//

#include "Generator.h"
#include <utility>

Generator::Generator(NodeProgram program_) : m_program(std::move(program_))
{
    m_output = std::stringstream{};
}

void Generator::generateTerm(NodeTerm term_)
{
    struct TermVisitor
    {
        Generator& gen;
        void operator()(const NodeTermIntLiteral& intLiteral_)
        {
            gen.m_output << "    mov rax, " << intLiteral_.int_literal.val.value() << "\n";
            gen.m_output << "    push rax\n";
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
