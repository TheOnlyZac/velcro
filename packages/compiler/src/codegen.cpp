#include "codegen.h"
#include <stdexcept>
#include <cstring>
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

    // Handle arguments using specialized helper functions
    Operands operands = generateOperandsForOpcode(opcode, elements, functionName);

    // Add special flag if needed
    bool flag = (opcode == OP_PUSH_FOCUS || opcode == OP_POP_FOCUS
        || opcode == OP_START_CINEMATIC || opcode == OP_START_SOUND
        || opcode == OP_PATH_TO_TARGET || opcode == OP_SHOW_BTN_NOTE);

    Instruction inst = createInstruction(opcode, flag, operands);
    bytecode.insert(bytecode.end(), inst.begin(), inst.end());

    return bytecode;
}

Operands CodeGenerator::generateOperandsForOpcode(Opcode opcode, const std::vector<ASTNodePtr> &elements, const std::string &functionName)
{
    Operands operands = {};

    switch (opcode)
    {
        case OP_PUSH_FOCUS:
        {
            ensureArgumentCount(elements, 1, functionName);
            int focus = getIntegerArgument(elements, 1, functionName, "focus");
            writeInt32(operands, 0, focus);
            break;
        }
        case OP_POP_FOCUS:
        {
            ensureArgumentCount(elements, 0, functionName);
            break;
        }
        case OP_SET_PLAYER:
        {
            ensureArgumentCount(elements, 1, functionName);
            int oid = getIntegerArgument(elements, 1, functionName, "character");
            writeInt32(operands, 0, oid);
            break;
        }
        case OP_SET_CLOCK_RATE:
        {
            ensureArgumentCount(elements, 1, functionName);
            float rate = getFloatArgument(elements, 1, functionName, "rate");
            writeFloat(operands, 0, rate);
            break;
        }
        case OP_START_CINEMATIC:
        {
            ensureArgumentCount(elements, 0, functionName);
            writeInt32(operands, 0, 1);
            writeUint32(operands, 4, 3);
            break;
        }
        case OP_RESET_CM:
        {
            ensureArgumentCount(elements, 0, functionName);
            break;
        }
        case OP_SLEEP:
        {
            ensureArgumentCount(elements, 1, functionName);
            float dur = getFloatArgument(elements, 1, functionName, "seconds");
            writeFloat(operands, 0, dur);
            // Set 2nd and 3rd to 0xFFFFFFFF
            writeUint32(operands, 4, 0xFFFFFFFF);
            writeUint32(operands, 8, 0xFFFFFFFF);
            break;
        }
        case OP_JUMP_TO_TARGET:
        {
            ensureArgumentCount(elements, 4, functionName);
            int targetOid = getIntegerArgument(elements, 1, functionName, "target");
            int doubleJump = getIntegerArgument(elements, 2, functionName, "double-jump");
            int unknown = getIntegerArgument(elements, 3, functionName, "unknown");
            int overrideOid = getIntegerArgument(elements, 4, functionName, "override");
            writeInt32(operands, 0, targetOid);
            writeInt32(operands, 4, doubleJump);
            writeInt32(operands, 8, unknown);
            // Todo: Override should be optional and default to 0xFFFFFFFF
            writeInt32(operands, 12, overrideOid);
            break;
        }
        case OP_SPAWN_ENTITY:
        {
            ensureArgumentCount(elements, 2, functionName);
            int oid = getIntegerArgument(elements, 1, functionName, "oid");
            float delay = getFloatArgument(elements, 2, functionName, "delay");
            writeInt32(operands, 0, oid);
            writeFloat(operands, 4, delay);
            break;
        }
        case OP_DESPAWN_ENTITY:
        {
            ensureArgumentCount(elements, 2, functionName);
            int oid = getIntegerArgument(elements, 1, functionName, "oid");
            float delay = getFloatArgument(elements, 2, functionName, "delay");
            writeInt32(operands, 0, oid);
            writeFloat(operands, 4, delay);
            break;
        }
        case OP_SCREENSHAKE:
        case OP_RUMBLE:
        {
            ensureArgumentCount(elements, 0, functionName);
            break;
        }
        case OP_START_SOUND:
        {
            ensureArgumentCount(elements, 1, functionName);
            int sfxid = getIntegerArgument(elements, 1, functionName, "sfxid");
            writeInt32(operands, 0, sfxid);
            writeUint32(operands, 4, 0xFFFFFFFF);
            writeUint32(operands, 8, 0x0000012C);
            writeUint32(operands, 12, 0x00000BB8);
            writeUint32(operands, 24, 0x3F800000);
            break;
        }
        case OP_START_DIALOG:
        {
            ensureArgumentCount(elements, 1, functionName);
            int dialogId = getIntegerArgument(elements, 1, functionName, "dialogid");
            writeInt32(operands, 0, dialogId);
            break;
        }
        case OP_SHOW_BTN_NOTE:
        {
            ensureArgumentCount(elements, 1, functionName);
            int btn = getIntegerArgument(elements, 1, functionName, "btn");
            writeInt32(operands, 0, btn);
            break;
        }
        case OP_SET_PUPPET_MODE:
        {
            ensureArgumentCount(elements, 2, functionName);
            int oid = getIntegerArgument(elements, 1, functionName, "entity");
            int mode = getIntegerArgument(elements, 2, functionName, "mode");
            writeInt32(operands, 0, oid);
            writeInt32(operands, 4, mode);
            // The 3rd argument is unknown but ususally 1
            writeInt32(operands, 8, 1);
            break;
        }
        default:
        {
#ifdef DEBUG
            std::cout << "Warning: Emitting unsupported opcode " << functionName << " with generic operand encoding." << std::endl;
            std::vector<ASTNode *> args;
            for (size_t i = 1; i < elements.size(); ++i)
            {
                args.push_back(elements[i].get());
            }
            operands = encodeOperands(args);
            break;
#endif
            throw std::runtime_error("Unsupported opcode: " + functionName);
        }
    }

    return operands;
}

// Helper functions for argument validation and extraction
void CodeGenerator::ensureArgumentCount(const std::vector<ASTNodePtr> &elements, size_t expected, const std::string &functionName)
{
    if (elements.size() != expected + 1)
    {
        throw std::runtime_error(functionName + " requires exactly " +
            std::to_string(expected) + " argument(s)");
    }
}

int CodeGenerator::getIntegerArgument(const std::vector<ASTNodePtr> &elements, size_t index, const std::string &functionName, const std::string &paramName)
{
    auto atomToken = dynamic_cast<AtomNode *>(elements[index].get());
    if (!atomToken)
        throw std::runtime_error(functionName + " " + paramName + " must be an atom");

    const Token &token = atomToken->getToken();
    if (token.type != INTEGER)
        throw std::runtime_error(functionName + " " + paramName + " must be an integer");

    return std::stoi(token.image);
}

float CodeGenerator::getFloatArgument(const std::vector<ASTNodePtr> &elements, size_t index, const std::string &functionName, const std::string &paramName)
{
    auto atomToken = dynamic_cast<AtomNode *>(elements[index].get());
    if (!atomToken)
        throw std::runtime_error(functionName + " " + paramName + " must be an atom");

    const Token &token = atomToken->getToken();
    if (token.type != FLOAT)
        throw std::runtime_error(functionName + " " + paramName + " must be a float");

    return std::stof(token.image);
}
