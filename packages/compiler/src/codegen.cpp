#include "codegen.h"
#include <stdexcept>
#include <cstring>
#include <map>
#ifdef DEBUG
#include <iostream>
#endif

Bytes CodeGenerator::generate(ASTNodePtr ast)
{
    Bytes bytecode;

    if (!ast)
        return bytecode;

    // Top-level ListNode represents the program (a list of expressions).
    ListNode *listNode = dynamic_cast<ListNode *>(ast.get());
    if (!listNode)
        throw std::runtime_error("Top-level AST nodes must be ListNodes");

    // Iterate over each element and generate an instruction for each function call/list.
    const std::vector<ASTNodePtr> &elements = listNode->getElements();
    for (const ASTNodePtr &elem : elements)
    {
        if (!elem)
            continue;

        // Each top-level element should itself be a ListNode representing a function call.
        if (auto fnCall = dynamic_cast<ListNode *>(elem.get()))
        {
            Bytes instructions = generateInstruction(fnCall);
            bytecode.insert(bytecode.end(), instructions.begin(), instructions.end());
        }
        else
        {
            // Non-list top-level elements are not valid function calls.
            throw std::runtime_error("Top-level expressions must be function call lists");
        }
    }

    return bytecode;
}

template<size_t N>
void CodeGenerator::writeUint32(std::array<byte, N> &buffer, size_t offset, uint32_t value, bool bigEndian)
{
    if (offset + 4 > buffer.size())
        throw std::runtime_error("Operand offset out of bounds");

    if (bigEndian)
    {
        buffer[offset] = static_cast<byte>((value >> 24) & 0xFF);
        buffer[offset + 1] = static_cast<byte>((value >> 16) & 0xFF);
        buffer[offset + 2] = static_cast<byte>((value >> 8) & 0xFF);
        buffer[offset + 3] = static_cast<byte>(value & 0xFF);

#ifdef DEBUG
        std::cout << "Wrote big-endian value: " << value << " at offset " << offset << std::endl;
#endif
        return;
    }

    buffer[offset] = static_cast<byte>(value & 0xFF);
    buffer[offset + 1] = static_cast<byte>((value >> 8) & 0xFF);
    buffer[offset + 2] = static_cast<byte>((value >> 16) & 0xFF);
    buffer[offset + 3] = static_cast<byte>((value >> 24) & 0xFF);

#ifdef DEBUG
    std::cout << "Wrote little-endian value: " << value << " at offset " << offset << std::endl;
#endif
}

template<size_t N>
void CodeGenerator::writeInt32(std::array<byte, N> &buffer, size_t offset, int32_t value)
{
    writeUint32(buffer, offset, static_cast<uint32_t>(value));
}

template<size_t N>
void CodeGenerator::writeFloat(std::array<byte, N> &buffer, size_t offset, float value)
{
    if (offset + 4 > buffer.size())
        throw std::runtime_error("Operand offset out of bounds");

    uint32_t asInt;
    std::memcpy(&asInt, &value, sizeof(float));
    writeUint32(buffer, offset, asInt);
}

Instruction CodeGenerator::createInstruction(Opcode opcode, bool flag, const Operands &operands)
{
    Instruction inst = {};

    // Set opcode (4 bytes, little-endian)
    inst[0] = static_cast<byte>(opcode & 0xFF);
    inst[1] = static_cast<byte>((opcode >> 8) & 0xFF);
    inst[2] = static_cast<byte>((opcode >> 16) & 0xFF);
    inst[3] = static_cast<byte>((opcode >> 24) & 0xFF);

    // Set flag (1 byte)
    inst[4] = flag ? 1 : 0;

    // Copy operands (up to 64 bytes)
    for (size_t i = 0; i < operands.size(); ++i)
    {
        inst[16 + i] = operands[i];
    }

    return inst;
}

Opcode CodeGenerator::encodeOpcode(const std::string &functionName)
{
    static const std::map<std::string, Opcode> opcodeMap = {
        {"PushFocus", OP_PUSH_FOCUS},
        {"PopFocus", OP_POP_FOCUS},
        {"SetPoCur", OP_SET_PO_CUR},
        {"StartSplice", OP_START_SPLICE},
        {"StartMacro", OP_START_MACRO},
        {"ShowBlot", OP_SHOW_BLOT},
        {"SetClockRate", OP_SET_CLOCK_RATE},
        {"StartCinematic", OP_SHOW_LETTERBOXING},
        {"Teleport", OP_TELEPORT},
        {"ResetCm", OP_RESET_CM},
        {"UseObject", OP_USE_OBJECT},
        {"LootCutscene", OP_LOOT_CUTSCENE},
        {"Sleep", OP_SLEEP},
        {"PathToTarget", OP_PATH_TO_TARGET},
        {"JumpToTarget", OP_JUMP_TO_TARGET},
        {"SpawnEntity", OP_SPAWN_ENTITY},
        {"DespawnEntity", OP_DESPAWN_ENTITY},
        {"ToggleGeom", OP_TOGGLE_GEOM},
        {"Screenshake", OP_SCREENSHAKE},
        {"Rumble", OP_RUMBLE},
        {"StartSound", OP_START_SOUND},
        {"MarkCmStart", OP_MARK_CM_START},
        {"MarkCmEnd", OP_MARK_CM_END},
        {"SetupBinoc", OP_SETUP_BINOC},
        {"SetCmFocus", OP_SET_CM_FOCUS},
        {"SetupCm", OP_SETUP_CM},
        {"ShowButtonNote", OP_SHOW_BTN_NOTE},
        {"StartDialog", OP_START_DIALOG},
        {"SetPuppetMode", OP_SET_PUPPET_MODE},
        {"SetInfoboxString", OP_SET_INFOBOX_STRING}
    };

    auto it = opcodeMap.find(functionName);
    if (it != opcodeMap.end())
        return it->second;

    throw std::runtime_error(std::string("Unknown function name: ") + functionName);
}

Operands CodeGenerator::encodeOperands(const std::vector<ASTNode *> &arguments)
{
    Operands ops = {};
    size_t offset = 0;

    for (const auto &arg : arguments)
    {
        if (!arg)
            throw std::runtime_error("Null ASTNode pointer in operands");

        auto atom = dynamic_cast<AtomNode *>(arg);
        if (!atom)
            throw std::runtime_error("Only atom nodes are supported as operands");

        const Token &token = atom->getToken();
        if (token.type == INTEGER)
        {
            int value = std::stoi(token.image);
            writeInt32(ops, offset, value);
            offset += 4;
        }
        else if (token.type == FLOAT)
        {
            float value = std::stof(token.image);
            writeFloat(ops, offset, value);
            offset += 4;
        }
        else
        {
            throw std::runtime_error("Unsupported operand type");
        }
    }
    return ops;
}

Bytes CodeGenerator::generateInstruction(ListNode *functionCall)
{
    Bytes bytecode;

    // Ensure functionCall is not empty
    const std::vector<ASTNodePtr> &elements = functionCall->getElements();
    if (elements.empty())
        throw std::runtime_error("Empty function call");

    // Ensure first element is an AtomNode (should be function name identifier)
    auto funcAtom = dynamic_cast<AtomNode *>(elements[0].get());
    if (!funcAtom)
        throw std::runtime_error("Function name must be an atom");

    // Get function name and encode opcode
    std::string functionName = funcAtom->toString();
    Opcode opcode = encodeOpcode(functionName);

    // Handle arguments
    Operands operands = {};
    switch (opcode)
    {
        case OP_SLEEP:
        {
            if (elements.size() != 2)
                throw std::runtime_error("Sleep requires exactly one argument");

            auto sleepDurToken = dynamic_cast<AtomNode *>(elements[1].get());
            if (!sleepDurToken)
                throw std::runtime_error("Sleep argument must be an atom");

            // Write sleep duration as float operand
            const Token &token = sleepDurToken->getToken();
            if (token.type != FLOAT)
                throw std::runtime_error("Sleep duration must be a float");

            // Convert float to 32-bit int representation
            float sleepDurValue = std::stof(token.image);
            writeFloat(operands, 0, sleepDurValue);

            // Set 2nd and 3rd to 0xFFFFFFFF
            writeUint32(operands, 4, 0xFFFFFFFF);
            writeUint32(operands, 8, 0xFFFFFFFF);
            break;
        }
        case OP_START_SOUND:
        {
            if (elements.size() != 2)
                throw std::runtime_error("StartSound requires exactly one argument");

            auto soundIdToken = dynamic_cast<AtomNode *>(elements[1].get());
            if (!soundIdToken)
                throw std::runtime_error("StartSound argument must be an atom");

            // Write sound ID as integer operand
            const Token &token = soundIdToken->getToken();
            if (token.type != INTEGER)
                throw std::runtime_error("StartSound ID must be an integer");

            int soundIdValue = std::stoi(token.image);
            writeInt32(operands, 0, soundIdValue);
            writeUint32(operands, 4, 0xFFFFFFFF);
            writeUint32(operands, 8, 0x0000012C);
            writeUint32(operands, 12, 0x00000BB8);
            writeUint32(operands, 24, 0x3F800000);
            break;
        }
        case OP_SHOW_LETTERBOXING:
        {
            if (elements.size() != 1)
                throw std::runtime_error("StartCinematic requires zero arguments");

            writeInt32(operands, 0, 1);
            writeUint32(operands, 4, 3);
            break;
        }
        default:
        {
            std::vector<ASTNode *> args;
            for (size_t i = 1; i < elements.size(); ++i)
            {
                // Use raw pointer to avoid copying unique_ptr
                args.push_back(elements[i].get());
            }
            operands = encodeOperands(args);
            break;
        }
    }


    // Add special flag if needed
    bool flag = false;
    if (opcode == OP_PUSH_FOCUS || opcode == OP_POP_FOCUS
        || opcode == OP_SHOW_LETTERBOXING || opcode == OP_START_SOUND
        || opcode == OP_PATH_TO_TARGET || opcode == OP_SHOW_BTN_NOTE)
    {
        flag = true;
    }

    Instruction inst = createInstruction(opcode, flag, operands);
    bytecode.insert(bytecode.end(), inst.begin(), inst.end());

    return bytecode;
}
