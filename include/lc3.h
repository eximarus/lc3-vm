#pragma once
#include "instructions.h"
#include <cpu.h>
#include <mem.h>
#include <string>

struct Lc3 final {
    Cpu cpu{};
    Mem mem{};
    void setup();
    void load_image(const std::string& image_path);
    void run();

    void op_add(OperateInstruction instruction);
    void op_and(OperateInstruction instruction);
    void op_not(OperateInstruction instruction);

    // Load
    void op_ld(DataMovementInstruction instruction);
    void op_ldi(DataMovementInstruction instruction);
    void op_ldr(DataMovementInstruction instruction);

    // store
    void op_st(DataMovementInstruction instruction);
    void op_str(DataMovementInstruction instruction);
    void op_sti(DataMovementInstruction instruction);

    // Load effective address
    void op_lea(DataMovementInstruction instruction);

    void op_br(ControlInstruction instruction);
    void op_jsr(ControlInstruction instruction);
    void op_jmp(ControlInstruction instruction);
    void op_trap(ControlInstruction instruction);
} __attribute__((packed)) __attribute__((aligned(128)));
