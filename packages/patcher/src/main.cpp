#include <iostream>
#include <fstream>
#include "types.h"
#include "pnachgen.h"

#define INSTRUCTION_SIZE 80

void PrintUsage(const char *program)
{
    std::cout << "Usage:" << std::endl;
    std::cout << "  " << program << " <input_file.s2m> <output_file.pnach> <address>" << std::endl;
}


int main(int argc, char* argv[]) {
    std::cout << "Velcro Patcher v1.0.0" << std::endl;

    if (argc != 4)
    {
        PrintUsage(argv[0]);
        return 1;
    }

    const char *infileName = argv[1];
    const char *outfileName = argv[2];
    const char *addressStr = argv[3];
    unsigned int address = std::stoul(addressStr, nullptr, 16);

    try
    {
        // Read in bytes from input file
        std::ifstream bytecodeFile(infileName, std::ios::binary);
        if (!bytecodeFile)
        {
            std::cerr << "Error: Could not open input file: " << infileName << std::endl;
            return 1;
        }

        Bytes bytecode((std::istreambuf_iterator<char>(bytecodeFile)), std::istreambuf_iterator<char>());
        bytecodeFile.close();

        // Generate and clear macro structure
        Macro macro;
        macro.init(static_cast<unsigned int>(bytecode.size() / INSTRUCTION_SIZE), address + 0xB0); // Instructions array immediately follows macro structure (0xB0 bytes)

#ifdef DEBUG
        // Print relevant macro fields for debugging
        std::cout << "Macro struct size: " << sizeof(Macro) << " bytes" << std::endl;

        std::cout << "Macro.pvtMacro: " << std::hex << macro.pvtMacro << std::endl;
        std::cout << "Macro.pfkList: " << std::hex << macro.pfkList << std::endl;
        std::cout << "Macro.cinstructions: " << macro.cinstructions << std::endl;
        std::cout << "Macro.painstructions: " << std::hex << static_cast<unsigned int>(macro.painstructions) << std::endl;
#endif

        PnachGenerator generator;

        // Add null byte at address + 0x0
        Bytes nullBytes(4, 0);
        generator.addBytes(nullBytes, address + 0x0);

        // Add cinstructions bytes at address + 0x50
        Bytes cinstructionsBytes(reinterpret_cast<byte*>(&macro.cinstructions), reinterpret_cast<byte*>(&macro.cinstructions) + sizeof(macro.cinstructions));
        generator.addBytes(cinstructionsBytes, address + 0x50);

        // Add bytecode bytes at address + 0xB0
        Bytes bytecodeBytes(bytecode.data(), bytecode.data() + bytecode.size());

#ifdef DEBUG
        std::cout << "Bytecode size: " << bytecode.size() << " bytes" << std::endl;
        std::cout << "Bytecode address: " << std::hex << (address + 0xB0) << std::endl;

        std::cout << "First 16 bytes of bytecode:" << std::endl;
        for (size_t i = 0; i < 16 && i < bytecode.size(); ++i)
        {
            std::cout << std::hex << static_cast<unsigned int>(bytecode[i]) << " ";
        }
        std::cout << std::endl;
#endif
        generator.addBytes(bytecodeBytes, address + 0xB0);

        std::string pnach = generator.generate();

        // Write generated pnach to output file
        std::ofstream pnachFile(outfileName);
        if (!pnachFile)
        {
            std::cerr << "Error: Could not open output file: " << outfileName << std::endl;
            return 1;
        }
        pnachFile << pnach;
        pnachFile.close();

        std::cout << "Wrote " << outfileName << std::endl;
    }
    catch(const std::exception& e)
    {
        std::cerr << "Error: " << e.what() << '\n';
        return 1;
    }

    return 0;
}
