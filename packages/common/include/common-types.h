
#include <cstdint>
#include <vector>
#include <string>
#include <map>

using byte = uint8_t;
using Bytes = std::vector<byte>;

/**
 * @brief Opcode values for each bytecode instructions.
 */
enum Opcode : uint32_t
{
    OP_PUSH_FOCUS = 0x0,
    OP_POP_FOCUS = 0x1,
    OP_SET_PLAYER = 0x7,
    OP_START_SPLICE = 0x9,
    OP_START_MACRO = 0xB,
    OP_SHOW_BLOT = 0xE,
    OP_SET_CLOCK_RATE = 0x10,
    OP_START_CINEMATIC = 0x12,
    OP_TELEPORT = 0x16,
    OP_RESET_CM = 0x17,
    OP_USE_OBJECT = 0x1A,
    OP_LOOT_CUTSCENE = 0x22,
    OP_SLEEP = 0x23,
    OP_PATH_TO_TARGET = 0x24,
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
    OP_SHOW_BTN_NOTE = 0x42,
    OP_START_DIALOG = 0x45,
    OP_SET_PUPPET_MODE = 0x47,
    OP_SET_INFOBOX_STRING = 0x54,
};

/**
 * @brief Maps name of built-in functions to opcodes.
 */
static const std::map<std::string, Opcode> opcodeMap = {
    {"PushFocus", OP_PUSH_FOCUS},
    {"PopFocus", OP_POP_FOCUS},
    {"SetPlayer", OP_SET_PLAYER},
    {"StartSplice", OP_START_SPLICE},
    {"StartMacro", OP_START_MACRO},
    {"ShowBlot", OP_SHOW_BLOT},
    {"SetClockRate", OP_SET_CLOCK_RATE},
    {"StartCinematic", OP_START_CINEMATIC},
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
