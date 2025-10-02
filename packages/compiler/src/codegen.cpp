#include "codegen.h"
#include <stdexcept>
#include <cstring>
#include <map>
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wreturn-type"
#pragma GCC diagnostic ignored "-Wunused-parameter"

Bytes CodeGenerator::generate(ASTNodePtr ast)
{
    Bytes bytecode;

    if (!ast)
        return bytecode;

    // Top-level ListNode which represents the program (a list of expressions).
    ListNode *listNode = dynamic_cast<ListNode *>(ast.get());
    if (!listNode)
        throw std::runtime_error("Top-level AST nodes must be ListNodes");

    // Iterate each element and generate an instruction for each function call/list.
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

Instruction CodeGenerator::createInstruction(Opcode opcode, bool flag, const Operands &operands)
{
    Instruction inst = {};

    // Set opcode (4 bytes, little-endian)
    inst[0] = static_cast<byte>((opcode >> 24) & 0xFF);
    inst[1] = static_cast<byte>((opcode >> 16) & 0xFF);
    inst[2] = static_cast<byte>((opcode >> 8) & 0xFF);
    inst[3] = static_cast<byte>(opcode & 0xFF);

    // Set flag (1 byte)
    inst[7] = flag ? 1 : 0;

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
        {"ShowLetterboxing", OP_SHOW_LETTERBOXING},
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
        {
            throw std::runtime_error("Null ASTNode pointer in operands");
        }

        if (auto atom = dynamic_cast<AtomNode *>(arg))
        {
            const Token &token = atom->getToken();
            if (token.type == INTEGER)
            {
                int value = std::stoi(token.image);
                if (offset + 4 > ops.size())
                {
                    // Raise error: Too many operands
                    throw std::runtime_error("Too many operands for instruction");
                }
                ops[offset] = static_cast<byte>((value >> 24) & 0xFF);
                ops[offset + 1] = static_cast<byte>((value >> 16) & 0xFF);
                ops[offset + 2] = static_cast<byte>((value >> 8) & 0xFF);
                ops[offset + 3] = static_cast<byte>(value & 0xFF);
                offset += 4;
            }
            else if (token.type == FLOAT)
            {
                float value = std::stof(token.image);
                if (offset + 4 > ops.size())
                {
                    // Raise error: Too many operands
                    throw std::runtime_error("Too many operands for instruction");
                }
                uint32_t asInt;
                std::memcpy(&asInt, &value, sizeof(float));
                ops[offset] = static_cast<byte>((asInt >> 24) & 0xFF);
                ops[offset + 1] = static_cast<byte>((asInt >> 16) & 0xFF);
                ops[offset + 2] = static_cast<byte>((asInt >> 8) & 0xFF);
                ops[offset + 3] = static_cast<byte>(asInt & 0xFF);
                offset += 4;
            }
            else
            {
                // Raise error: Unsupported operand type
                throw std::runtime_error("Unsupported operand type");
            }
        }
        else
        {
            // Raise error: Only atom nodes are supported as operands
            throw std::runtime_error("Only atom nodes are supported as operands");
        }
    }
    return ops;
}

Bytes CodeGenerator::generateInstruction(ListNode *functionCall)
{
    Bytes bytecode;

    const std::vector<ASTNodePtr> &elements = functionCall->getElements();
    if (elements.empty())
    {
        // Raise error: Empty function call
        throw std::runtime_error("Empty function call");
    }

    // First element should be the function name (AtomNode)
    auto funcAtom = dynamic_cast<AtomNode *>(elements[0].get());
    if (!funcAtom)
    {
        // Raise error: Function name must be an atom
        throw std::runtime_error("Function name must be an atom");
    }

    std::string functionName = funcAtom->toString();
    Opcode opcode = encodeOpcode(functionName);

    // Remaining elements are arguments
    std::vector<ASTNode *> args;
    for (size_t i = 1; i < elements.size(); ++i)
    {
        // Use raw pointer to avoid copying unique_ptr
        args.push_back(elements[i].get());
    }

    Operands operands = encodeOperands(args);

    // Determine if special flag is needed
    bool flag = false;
    if (opcode == OP_PUSH_FOCUS || opcode == OP_POP_FOCUS
        || opcode == OP_PATH_TO_TARGET || opcode == OP_SHOW_BTN_NOTE)
    {
        flag = true;
    }

    Instruction inst = createInstruction(opcode, flag, operands);
    bytecode.insert(bytecode.end(), inst.begin(), inst.end());

    return bytecode;
}

#pragma GCC diagnostic pop
