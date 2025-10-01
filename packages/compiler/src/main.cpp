#include <iostream>
#include <fstream>
#include <string>
#include "lexer.h"
#include "parser.h"
#include "codegen.h"

void PrintUsage(const char *program)
{
    std::cout << "Usage:" << std::endl;
    std::cout << "  " << program << " <input_file.cro> <output_file.s2m>" << std::endl;
}

int main(int argc, const char *argv[])
{
    std::cout << "Velcro Compiler v1.0.0" << std::endl;

    if (argc != 3)
    {
        PrintUsage(argv[0]);
        return 1;
    }

    const char *infileName = argv[1];
    const char *outfileName = argv[2];

    try
    {
        std::string sourceText;

        std::ifstream infile(infileName);
        std::string line;
        while (std::getline(infile, line))
        {
            sourceText += line;
            sourceText.push_back('\n');
        }
        infile.close();

        std::cout << sourceText << std::endl;

        Lexer lexer(sourceText);
        std::vector<Token> tokens = lexer.tokenize();

        Parser parser(tokens);
        ASTNodePtr ast = parser.parse();

        // Todo: Report any syntax errors

        CodeGenerator codegen;
        Bytes bytecode = codegen.generate(std::move(ast));

        // Todo: Report any compilation errors

        std::ofstream outfile(outfileName);
        outfile.write(reinterpret_cast<const char *>(bytecode.data()), bytecode.size());
        outfile.close();

        std::cout << "Wrote " << outfileName << std::endl;
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        return 1;
    }

    return 0;
}
