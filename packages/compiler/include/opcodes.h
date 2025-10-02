#pragma once
#include <cstdint>

/**
 * @brief Enumeration of opcodes for the macro bytecode instructions.
 */
enum Opcode : uint32_t
{
    OP_PUSH_FOCUS = 0x0, // Uses special flag
    OP_POP_FOCUS = 0x1, // Uses special flag
    OP_SET_PO_CUR = 0x7,
    OP_START_SPLICE = 0x9,
    OP_START_MACRO = 0xB,
    OP_SHOW_BLOT = 0xE,
    OP_SET_CLOCK_RATE = 0x10,
    OP_SHOW_LETTERBOXING = 0x12,
    OP_TELEPORT = 0x16,
    OP_RESET_CM = 0x17,
    OP_USE_OBJECT = 0x1A,
    OP_LOOT_CUTSCENE = 0x22,
    OP_SLEEP = 0x23,
    OP_PATH_TO_TARGET = 0x24, // Uses special flag
    OP_JUMP_TO_TARGET = 0x2A,
    OP_SPAWN_ENTITY = 0x2C,
    OP_DESPAWN_ENTITY = 0x2D,
    OP_TOGGLE_GEOM = 0x2E,
    OP_SCREENSHAKE = 0x32,
    OP_RUMBLE = 0x33,
    OP_START_SOUND = 0x36,
    OP_MARK_CM_START = 0x38,
    OP_MARK_CM_END = 0x39,
    OP_SETUP_BINOC = 0x3B,
    OP_SET_CM_FOCUS = 0x3E,
    OP_SETUP_CM = 0x3F,
    OP_SHOW_BTN_NOTE = 0x42, // Uses special flag
    OP_START_DIALOG = 0x45,
    OP_SET_PUPPET_MODE = 0x47,
    OP_SET_INFOBOX_STRING = 0x54,
};
