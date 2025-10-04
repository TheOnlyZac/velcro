#include <iostream>
#include <fstream>
#include "scanner.h"

void PrintUsage(const char *program)
{
    std::cout << "Usage:" << std::endl;
    std::cout << "  " << program << " <input_file> <output_file.vel>" << std::endl;
}

int main(int argc, const char *argv[])
{
    std::cout << "Velcro Deompiler v1.0.0" << std::endl;

    if (argc != 3)
    {
        PrintUsage(argv[0]);
        return 1;
    }

    const char *infileName = argv[1];
    const char *outfileName = argv[2];

    try
    {
        // Open input file for reading
        std::ifstream sourceFile(infileName);
        if (!sourceFile)
        {
            std::cerr << "Error: Could not open input file: " << infileName << std::endl;
            return 1;
        }

        // Read in bytecode from input file
        Bytes bytecode;
        byte b;
        while (sourceFile.read(reinterpret_cast<char*>(&b), sizeof(byte)))
        {
            bytecode.push_back(b);
        }
        sourceFile.close();

        // Scan the bytecode into structured instructions
        bool hasHeader = true;
        Scanner scanner(bytecode, hasHeader);
        std::vector<Instruction> instructions = scanner.scan();

        if (instructions.empty())
        {
            std::cerr << "Error: No instructions found in input file." << std::endl;
            return 1;
        }
        std::cout << "Scanned " << instructions.size() << " instructions." << std::endl;

        // Open output file fo writing
        std::ofstream outFile(outfileName);
        if (!outFile)
        {
            std::cerr << "Error: Could not open output file: " << outfileName << std::endl;
            return 1;
        }

        // Output the instructions as Velcro source code
        for (const Instruction &inst : instructions)
        {
            outFile << inst.toString() << std::endl;
        }
        outFile.close();

        std::cout << "Wrote " << outfileName << std::endl;
    }
    catch(const std::exception& e)
    {
        std::cerr << "Error: " << e.what() << '\n';
        return 1;
    }

    return 0;
}
