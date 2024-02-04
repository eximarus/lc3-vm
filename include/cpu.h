#pragma once
#include <array>
#include <cstdint>

enum {
    R_R0 = 0U,
    R_R1,
    R_R2,
    R_R3,
    R_R4,
    R_R5,
    R_R6, // SP
    R_R7,
    R_PC,
    R_COND,
    R_COUNT
};

enum Opcode : uint16_t {
    BR = 0U, /* branch */
    ADD,     /* add  */
    LD,      /* load */
    ST,      /* store */
    JSR,     /* jump register */
    AND,     /* bitwise and */
    LDR,     /* load register */
    STR,     /* store register */
    RTI,     /* unused */
    NOT,     /* bitwise not */
    LDI,     /* load indirect */
    STI,     /* store indirect */
    JMP,     /* jump */
    RES,     /* reserved (unused) */
    LEA,     /* load effective address */
    TRAP     /* execute trap */
};

enum ConditionCode {
    POS = 1U << 0U, /* P */
    ZRO = 1U << 1U, /* Z */
    NEG = 1U << 2U, /* N */
};

using Registers = std::array<uint16_t, R_COUNT>;
struct Cpu final {
    Registers reg{};
    void setcc(uint16_t dr);
} __attribute__((packed)) __attribute__((aligned(32)));
