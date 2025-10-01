#pragma once
#include "types.h"
#include <vector>
#include <cstdint>

using byte = uint8_t;
using Bytes = std::vector<byte>;

using Opcode = uint32_t;
using Flag = uint32_t;

using Operands = std::array<byte, 64>;
using Instruction = std::array<byte, 80>;

class CodeGenerator
{
public:
    Bytes generate(ASTNodePtr ast);

private:
    Instruction createInstruction(Opcode opcode, Flag flag, const Operands &operands);
    Opcode encodeOpcode(const std::string &functionName);
    Operands encodeOperands(const std::vector<ASTNode *> &arguments);
    Bytes generateInstruction(ListNode *functionCall);
};
