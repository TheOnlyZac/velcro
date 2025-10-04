#pragma once
#include <cstdint>
#include <cstring>
#include "common-types.h"

struct Macro {
    uint32_t psplice; // 0x0
    uint32_t unk_0x4; // 0x4
    uint32_t unk_0x8; // 0x8
    uint32_t pvtMacro; // REQUIRED: Pointer to macro vtable (0xC)

    uint32_t unk_0x10; // 0x10
    uint32_t unk_0x14; // 0x14
    uint32_t oid; // Object ID (0x18)
    uint32_t unk_0x1C; // 0x1C

    uint32_t pmacroNext; // 0x20
    uint32_t pentityNext; // 0x24
    uint32_t pentityPrev; // 0x28
    uint32_t unk_0x2C; // 0x2C

    uint32_t unk_0x30; // 0x30
    uint32_t unk_0x34; // 0x34
    uint32_t unk_0x38; // 0x38
    uint32_t pfkList; // REQUIRED: Pointer to file key list (0x3C)

    uint32_t unk_0x40; // 0x40
    uint32_t unk_0x44; // 0x44
    uint32_t unk_0x48; // 0x48
    uint32_t unk_0x4C; // 0x4C

    unsigned int cinstructions; // REQUIRED: Count of instructions (0x50)
    uint32_t painstructions; // REQUIRED: Pointer to array of instructions (0x54)
    uint32_t unk_0x58; // 0x58
    uint32_t unk_0x5C; // 0x5C

    uint32_t unk_0x60; // 0x60
    uint32_t unk_0x64; // 0x64
    uint32_t unk_0x68; // 0x68
    uint32_t unk_0x6C; // 0x6C

    uint32_t unk_0x70; // 0x70
    uint32_t unk_0x74; // 0x74
    uint32_t unk_0x78; // 0x78
    uint32_t unk_0x7C; // 0x7C

    uint32_t unk_0x80; // 0x80
    uint32_t unk_0x84; // 0x84
    uint32_t unk_0x88; // 0x88
    uint32_t unk_0x8C; // 0x8C

    uint32_t unk_0x90; // 0x90
    uint32_t unk_0x94; // 0x94
    uint32_t unk_0x98; // 0x98
    uint32_t unk_0x9C; // 0x9C

    uint32_t unk_0xA0; // 0xA0
    uint32_t unk_0xA4; // 0xA4
    uint32_t unk_0xA8; // 0xA8
    uint32_t unk_0xAC; // 0xAC

    void init(unsigned int cinstructions, uint32_t painstructions)
    {
        std::memset(this, 0, sizeof(Macro));
        this->psplice = 0x00F950D8;
        this->pvtMacro = 0x002D8C48;

        this->unk_0x14 = 0x00E9C300; // prefixed
        this->oid = 0x00006A74; //
        this->pmacroNext = 0x0140D730; // prefixed

        this->pfkList = 0x00487A00;
        this->cinstructions = cinstructions;
        this->painstructions = painstructions;
    }
};
