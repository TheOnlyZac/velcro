#pragma once
#include "types.h"
#include <string>

class PnachGenerator
{
public:
    PnachGenerator(Bytes bytes, unsigned int address)
        : _bytes(std::move(bytes)), _address(address) {}

    /**
     * @brief Generates a pnach that writes the bytes to the specified address.
     *
     * @param title The title of the pnach.
     * @param author The author of the pnach.
     * @param comment An optional comment to show in PCSX2.
     *
     * @return The generated pnach as a string.
     */
    std::string generate(std::string title, std::string author, std::string comment);

private:
    Bytes _bytes; // The bytes to write
    unsigned int _address; // The address to patch the bytes over

    /**
     * @brief Generates the header for the pnach.
     *
     * @param title The title of the pnach.
     * @param author The author of the pnach.
     * @param comment An optional comment to show in PCSX2.
     *
     * @return The generated header as a string.
     */
    std::string generateHeader(std::string title, std::string author, std::string comment);

    /**
     * @brief Generates a 32-bit write patch.
     *
     * Writes the 32-bit value V to the address A.
     * patch=1,EE,2AAAAAAA,extended,VVVVVVVV
     */
    std::string generate32bitWrite(uintptr_t address, uint32_t value);
};
