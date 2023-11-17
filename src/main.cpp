//
// Created by thierrycd on 2023-11-16.
//

#include "generation/Generator.h"
#include "parsing/Node.h"
#include "parsing/Parser.h"
#include "token/Tokenizer.h"


#include <fstream>
#include <iostream>
#include <sstream>

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        std::cerr << "Incorrect usage. Correct usage is..." << std::endl;
        std::cerr << "hel <input.hl>" << std::endl;
        return EXIT_FAILURE;
    }

    std::string fileContents;
    {
        std::stringstream contentsStream;
        std::ifstream file{argv[1]};
        contentsStream << file.rdbuf();
        fileContents = contentsStream.str();
    }

    Tokenizer tokenizer{fileContents};
    std::vector<Token> tokens = tokenizer.tokenize();

    Parser parser{tokens};
    std::optional<NodeProgram> program = parser.parseProgram();

    if (!program.has_value())
    {
        std::cerr << "Program could not be parsed." << std::endl;
        exit(EXIT_FAILURE);
    }

    Generator assemblyGenerator{program.value()};
    std::string output = assemblyGenerator.generateProgram();
    {
        std::ofstream file{"test.asm"};
        file << output;
    }

    system("nasm -felf64 test.asm");
    system("ld -o test test.o");

    return EXIT_SUCCESS;
}
