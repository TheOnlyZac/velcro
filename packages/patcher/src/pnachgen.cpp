#include "pnachgen.h"
#ifdef DEBUG
#include <iostream>
#endif

std::string PnachGenerator::generate(std::string title, std::string author, std::string comment)
{
    std::string pnach = generateHeader(title, author, comment);

    // Iterate over bytes in 4-byte chunks
    for (size_t i = 0; i < _bytes.size(); i += 4)
    {
        uint32_t value = 0;
        // Combine up to 4 bytes into a single 32-bit value (little-endian)
        for (size_t j = 0; j < 4; ++j)
        {
            if (i + j < _bytes.size())
            {
                value |= static_cast<uint32_t>(_bytes[i + j]) << (8 * j);
            }
        }
#ifdef DEBUG
        std::cout << "Patching address: " << std::hex << (_address + i) << " with value: " << std::hex << value << std::endl;
#endif
        pnach += generate32bitWrite(_address + i, value);
    }

    return pnach;
}

std::string PnachGenerator::generateHeader(std::string title, std::string author, std::string comment)
{
    std::string header = "[" + title + "]\n";
    header += "author=" + author + "\n";
    if (!comment.empty())
    {
        header += "comment=" + comment + "\n";
    }
    header += "\n";
    return header;
}

std::string PnachGenerator::generate32bitWrite(uintptr_t address, uint32_t value)
{
    char buffer[64];
    snprintf(buffer, sizeof(buffer), "patch=1,EE,2%07X,extended,%08X\n", static_cast<unsigned int>(address), value);
    std::string patch = buffer;
    return patch;
}
