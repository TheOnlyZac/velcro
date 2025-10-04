#include "scanner.h"
#include <vector>
#ifdef DEBUG
#include <iostream>
#endif

std::vector<Instruction> Scanner::scan()
{
    _current = _hasHeader ? 0xB0 : 0x00;

    std::vector<Instruction> instructions;
    while (!isAtEnd())
    {
        Instruction inst = scanInstruction();
        instructions.push_back(inst);
    }

    return instructions;
}

bool Scanner::isAtEnd() const
{
    return _current == _bytes.size();
}

byte Scanner::next()
{
    if (isAtEnd())
        return 0x00;

    return _bytes[_current++];
}

uint32_t Scanner::scanUint32()
{
    uint32_t value = 0;
    for (int i = 0; i < 4; i++)
    {
        value |= (static_cast<uint32_t>(next()) << (8 * i));
    }
#ifdef DEBUG
    std::cout << "Scanned uint32: " << value << std::endl;
#endif
    return value;
}

Instruction Scanner::scanInstruction()
{
    Instruction inst;
    inst.opcode = static_cast<Opcode>(scanUint32());
    inst.timingFlag = scanUint32();
    inst.padding1 = scanUint32();
    inst.padding2 = scanUint32();
    for (int i = 0; i < NUM_OPERAND_FIELDS; i++)
    {
        inst.operandFields[i] = scanUint32();
    }
#ifdef DEBUG
    std::cout << "Scanned instruction: " << inst.toString() << std::endl;
#endif
    return inst;
}
