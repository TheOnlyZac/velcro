#pragma once
#include "types.h"
#include "opcodes.h"
#include <vector>
#include <array>

using byte = uint8_t;
using Bytes = std::vector<byte>;

using Operands = std::array<byte, 64>;

using Instruction = std::array<byte, 80>;

class CodeGenerator
{
public:
    /**
     * @brief Generates bytecode from the given AST.
     */
    Bytes generate(ASTNodePtr ast);

private:
    /**
     * @brief Outputs the bytecode for a single instruction (80-byte block).
     */
    Instruction createInstruction(Opcode opcode, bool flag, const Operands &operands);

    /**
     * @brief Encodes the opcode from a function name.
     */
    Opcode encodeOpcode(const std::string &functionName);

    /**
     * @brief Encodes the operands from a list of AST node pointers.
     *
     * Note: takes a vector of raw ASTNode* pointers to avoid copying unique_ptrs.
     */
    Operands encodeOperands(const std::vector<ASTNode *> &arguments);

    /**
     * @brief Generates the bytecode for single function call instruction represented by a ListNode.
     */
    Bytes generateInstruction(ListNode *functionCall);
};
