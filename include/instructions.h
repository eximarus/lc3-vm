#pragma once
#include <cpu.h>

enum TrapCode {
    Getc = 0x20, /* get character from keyboard, not echoed onto the terminal */
    Out = 0x21,  /* output a character */
    Puts = 0x22, /* output a word string */
    In = 0x23,   /* get character from keyboard, echoed onto the terminal */
    Putsp = 0x24, /* output a byte string */
    Halt = 0x25   /* halt the program */
};

struct ControlInstruction final {
    union {
        struct {
            bool n;
            bool z;
            bool p;
        } __attribute__((aligned(4)));
        bool r_flag;
    };
    union {
        uint16_t pc_offset_9;
        uint16_t pc_offset_11;
        uint8_t base_r;
        uint8_t trapvect_8;
    };

    static ControlInstruction br(uint16_t instruction);
    static ControlInstruction jmp(uint16_t instruction);
    static ControlInstruction jsr(uint16_t instruction);
    static ControlInstruction trap(uint16_t instruction);
} __attribute__((aligned(8)));

struct DataMovementInstruction final {
    uint8_t dr;
    union {
        uint16_t pc_offset_9;
        struct {
            uint8_t base_r;
            uint8_t pc_offset_6;
        } __attribute__((aligned(2)));
    };

    static DataMovementInstruction with_offset_9(uint16_t instruction);
    static DataMovementInstruction with_offset_6(uint16_t instruction);
} __attribute__((aligned(4)));

struct OperateInstruction final {
    uint16_t dr;
    uint16_t sr1;
    uint16_t imm_flag;
    union {
        uint16_t sr2;
        uint16_t imm5;
    };

    explicit OperateInstruction(uint16_t instruction);

} __attribute__((aligned(8)));
