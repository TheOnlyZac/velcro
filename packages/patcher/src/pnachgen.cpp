#include "pnachgen.h"
#ifdef DEBUG
#include <iostream>
#endif

std::string PnachGenerator::generate()
{
    std::string pnach = generateHeader();

    // Iterate over bytes in 4-byte chunks
    for (const auto& [bytes, address] : _patches)
    {
        for (size_t i = 0; i < bytes.size(); i += 4)
        {
            uint32_t value = 0;
            // Combine up to 4 bytes into a single 32-bit value (little-endian)
            for (size_t j = 0; j < 4; ++j)
            {
                if (i + j < bytes.size())
                {
                    value |= static_cast<uint32_t>(bytes[i + j]) << (8 * j);
                }
            }
            pnach += generate32bitWrite(address + i, value);
        }
    }

    return pnach;
}

void PnachGenerator::addBytes(const Bytes& bytes, uintptr_t address)
{
    _patches[bytes] = static_cast<uint32_t>(address);
}

std::string PnachGenerator::generateHeader()
{
    std::string header = "[" + _title + "]\n";
    header += "author=" + _author + "\n";
    if (!_comment.empty())
    {
        header += "comment=" + _comment + "\n";
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
