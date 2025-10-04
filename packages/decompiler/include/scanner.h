#pragma once
#include "types.h"
#include <vector>
#include <cstdint>

/**
 * @brief Scanner class reads the binary data for a macro and outputs the instructions as structured data.
 */
class Scanner
{
public:
    Scanner(Bytes bytes, bool hasHeader = true)
        : _bytes(bytes), _hasHeader(hasHeader) {}

    /**
     * @brief Scans the bytes and converts them into a vector of structured instructions.
     */
    std::vector<Instruction> scan();

private:
    Bytes _bytes; // Byte source vector
    bool _hasHeader; // Flag indicating if the bytecode has a header
    size_t _current; // Current byte index

    /**
     * @brief Checks whether the current byte is the last byte.
     *
     * @returns True if at the end, otherwise false.
     */
    bool isAtEnd() const;

    /**
     * @brief Gets the next byte and advances the current index.
     */
    byte next();

    /**
     * @brief Scans a 32-bit unsigned integer.
     */
    uint32_t scanUint32();

    /**
     * @brief Scans a single instruction from the byte source.
     */
    Instruction scanInstruction();
};
