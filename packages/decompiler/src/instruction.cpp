#include "types.h"
#include <sstream>
#include <cstring>
#include <iomanip>

std::string Instruction::operandAsInt(int index) const
{
    if (index < 0 || index >= NUM_OPERAND_FIELDS)
        return "";
    return std::to_string(operandFields[index]);
}

std::string Instruction::operandAsFloat(int index) const
{
    if (index < 0 || index >= NUM_OPERAND_FIELDS)
        return "";

    float value;
    static_assert(sizeof(uint32_t) == sizeof(float), "Size mismatch between uint32_t and float");
    std::memcpy(&value, &operandFields[index], sizeof(float));

    std::ostringstream oss;
    oss << std::fixed << std::setprecision(6) << value;
    std::string str = oss.str();

    // Trim trailing zeros and possibly the decimal point
    str.erase(str.find_last_not_of('0') + 1, std::string::npos);
    if (!str.empty() && str.back() == '.')
        str.push_back('0'); // Ensure at least one digit after decimal

    return str;
}

std::string Instruction::operandAsHex(int index, bool includePrefix) const
{
    if (index < 0 || index >= NUM_OPERAND_FIELDS)
        return "";

    std::ostringstream oss;
    if (includePrefix)
        oss << "0x";
    oss << std::hex << operandFields[index];
    return oss.str();
}

std::string Instruction::toString() const
{
    std::ostringstream oss;

    oss << "(";

    // Get function name from opcodeMap by searching for the Opcode value
    std::string funcName;
    for (const auto& pair : opcodeMap)
    {
        if (pair.second == (opcode))
        {
            funcName = pair.first;
            break;
        }
    }
    if (!funcName.empty())
        oss << funcName;
    else
        oss << "#" << std::hex << opcode << std::dec;

    // Helper lambda to append operands with spaces
    auto appendOperands = [&](auto... args) {
        int dummy[] = {0, (oss << " " << args, 0)...};
        (void)dummy;
    };

    switch (opcode)
    {
        // One ID operand
        case OP_PUSH_FOCUS:
        case OP_START_SPLICE:
        case OP_START_MACRO:
        case OP_SHOW_BLOT:
        case OP_SET_CM_FOCUS:
        case OP_SHOW_BTN_NOTE:
        case OP_START_DIALOG:
        case OP_SET_INFOBOX_STRING:
            appendOperands(operandAsHex(0));
            break;

        // One float operand
        case OP_SET_CLOCK_RATE:
        case OP_SLEEP:
            appendOperands(operandAsFloat(0));
            break;

        // One ID and one float operand
        case OP_SPAWN_ENTITY:
        case OP_DESPAWN_ENTITY:
            appendOperands(operandAsHex(0), operandAsFloat(1));
            break;

        // Two int operands
        case OP_START_CINEMATIC:
            appendOperands(operandAsInt(0), operandAsInt(1));
            break;

        // Special cases
        case OP_TELEPORT:
            // Hex, Int, Hex, Int
            appendOperands(operandAsHex(0), operandAsInt(1), operandAsHex(2), operandAsInt(3));
            break;

        case OP_USE_OBJECT:
            // Hex, Hex
            appendOperands(operandAsHex(0), operandAsHex(1));
            break;

        case OP_LOOT_CUTSCENE:
            // Hex, Hex, Int, Hex
            appendOperands(operandAsHex(0), operandAsHex(1), operandAsInt(2), operandAsHex(3));
            break;

        case OP_PATH_TO_TARGET:
            // Hex, Float, Float, Hex, Float, Int
            appendOperands(
                operandAsHex(0), operandAsFloat(1), operandAsFloat(2), operandAsHex(3),
                operandAsHex(4), operandAsInt(5));
            break;

        case OP_JUMP_TO_TARGET:
            // Hex, Int, Hex, Hex
            appendOperands(operandAsHex(0), operandAsInt(1), operandAsHex(2), operandAsHex(3));
            break;

        case OP_TOGGLE_GEOM:
            // Hex, Int
            appendOperands(operandAsHex(0), operandAsInt(1));
            break;

        case OP_START_SOUND:
            // Hex, Float, Float
            appendOperands(operandAsHex(0), operandAsFloat(1), operandAsFloat(2));
            break;

        case OP_MARK_CM_START:
        case OP_MARK_CM_END:
            // Float, Float, Float, Float, Float, Float, Float, Float, Float
            appendOperands(
                operandAsFloat(0), operandAsFloat(1), operandAsFloat(2), operandAsFloat(3),
                operandAsFloat(4), operandAsFloat(5), operandAsFloat(6), operandAsFloat(7),
                operandAsFloat(8)
            );
            break;

        case OP_SETUP_BINOC:
            // Hex, Hex, Hex
            appendOperands(operandAsHex(0), operandAsHex(1), operandAsHex(2));
            break;

        case OP_SETUP_CM:
            // Unknown
            break;

        case OP_SET_PUPPET_MODE:
            // Hex, Hex, int
            appendOperands(operandAsHex(0), operandAsHex(1), operandAsInt(2));
            break;

        // No operands
        case OP_POP_FOCUS:
        case OP_RESET_CM:
        case OP_SCREENSHAKE:
        case OP_RUMBLE:
            break;

        default:
        {
            for (int i = 0; i < NUM_OPERAND_FIELDS; i++)
            {
                appendOperands(operandAsHex(i, false));
            }
            break;
        }
    }

    oss << ")" << std::dec;
    return oss.str();
}
