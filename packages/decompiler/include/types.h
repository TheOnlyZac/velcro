#pragma once
#include <cstdint>
#include <string>
#include "common-types.h"

static constexpr int NUM_OPERAND_FIELDS = 16;

class Instruction
{
public:
    Opcode opcode;
    uint32_t timingFlag;
    uint32_t padding1;
    uint32_t padding2;
    uint32_t operandFields[NUM_OPERAND_FIELDS];

    std::string toString() const;

private:
    // Helper functions for operand formatting
    std::string operandAsInt(int index) const;
    std::string operandAsFloat(int index) const;
    std::string operandAsHex(int index, bool includePrefix = true) const;
};
