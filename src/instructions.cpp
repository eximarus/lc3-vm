#include <array>
#include <fmt/core.h>
#include <gsl/gsl>
#include <instructions.h>
#include <lc3.h>

static uint16_t sext(uint16_t x, const int bit_count) {
    if (((uint16_t)(x >> (bit_count - 1U)) & 1U) != 0) {
        x |= (0xFFFFU << (uint16_t)bit_count);
    }
    return x;
}

ControlInstruction ControlInstruction::br(const uint16_t instruction) {
    return ControlInstruction{
        .n = (bool)(((uint8_t)(instruction >> 11U) & 0x1U)),
        .z = (bool)(((uint8_t)(instruction >> 10U) & 0x1U)),
        .p = (bool)(((uint8_t)(instruction >> 9U) & 0x1U)),
        .pc_offset_9 = (uint16_t)(instruction & 0x1FFU),
    };
}

ControlInstruction ControlInstruction::jmp(const uint16_t instruction) {
    return ControlInstruction{
        .base_r = (uint8_t)((uint8_t)(instruction >> 6U) & 0x7U),
    };
}

ControlInstruction ControlInstruction::jsr(const uint16_t instruction) {
    auto r_flag = (bool)((uint8_t)(instruction >> 11U) & 0x1U);
    if (r_flag) {
        return ControlInstruction{
            .r_flag = true,
            .pc_offset_11 = (uint16_t)(instruction & 0x7FFU),
        };
    }

    return ControlInstruction{
        .r_flag = false,
        .base_r = (uint8_t)((uint8_t)(instruction >> 6U) & 0x7U),
    };
}

ControlInstruction ControlInstruction::trap(const uint16_t instruction) {
    return ControlInstruction{
        .trapvect_8 = (uint8_t)(instruction & 0xFFU),
    };
}

OperateInstruction::OperateInstruction(const uint16_t instruction)
    : dr((uint16_t)(instruction >> 9U) & 0x7U),
      sr1((uint16_t)(instruction >> 6U) & 0x7U),
      imm_flag((uint16_t)(instruction >> 5U) & 0x1U) {

    if (imm_flag == 0) {
        sr2 = instruction & 0x7U;
    } else {
        imm5 = sext(instruction & 0x1FU, 5U);
    }
}

DataMovementInstruction
DataMovementInstruction::with_offset_9(const uint16_t instruction) {
    return DataMovementInstruction{
        .dr = (uint8_t)(((uint8_t)(instruction >> 9U) & 0x7U)),
        .pc_offset_9 = (uint16_t)(instruction & 0x1FFU),
    };
}

DataMovementInstruction
DataMovementInstruction::with_offset_6(const uint16_t instruction) {
    return DataMovementInstruction{
        .dr = (uint8_t)(((uint8_t)(instruction >> 9U) & 0x7U)),
        .base_r = (uint8_t)((uint16_t)(instruction >> 6U) & 0x7U),
        .pc_offset_6 = (uint8_t)(instruction & 0x3FU),
    };
}

void Lc3::op_ld(const DataMovementInstruction instruction) {
    uint16_t pc = cpu.reg[R_PC];
    uint16_t pc_offset = sext(instruction.pc_offset_9, 9);
    uint16_t mem_addr = pc + pc_offset;

    gsl::at(cpu.reg, instruction.dr) = gsl::at(mem, mem_addr);
    cpu.setcc(instruction.dr);
}

void Lc3::op_ldi(const DataMovementInstruction instruction) {
    uint16_t pc = cpu.reg[R_PC];
    uint16_t pc_offset = sext(instruction.pc_offset_9, 9);
    uint16_t mem_addr = pc + pc_offset;

    gsl::at(cpu.reg, instruction.dr) = gsl::at(mem, gsl::at(mem, mem_addr));
    cpu.setcc(instruction.dr);
}

void Lc3::op_ldr(const DataMovementInstruction instruction) {
    uint16_t pc_offset = sext(instruction.pc_offset_6, 6);
    uint16_t base_addr = gsl::at(cpu.reg, instruction.base_r);
    uint16_t mem_addr = base_addr + pc_offset;

    gsl::at(cpu.reg, instruction.dr) = gsl::at(mem, mem_addr);
    cpu.setcc(instruction.dr);
}

void Lc3::op_add(const OperateInstruction instruction) {
    if (instruction.imm_flag != 0) {
        gsl::at(cpu.reg, instruction.dr) =
            gsl::at(cpu.reg, instruction.sr1) + instruction.imm5;
    } else {
        gsl::at(cpu.reg, instruction.dr) = gsl::at(cpu.reg, instruction.sr1) +
                                           gsl::at(cpu.reg, instruction.sr2);
    }

    cpu.setcc(instruction.dr);
}

void Lc3::op_and(const OperateInstruction instruction) {
    if (instruction.imm_flag != 0) {
        gsl::at(cpu.reg, instruction.dr) =
            gsl::at(cpu.reg, instruction.sr1) & instruction.imm5;
    } else {
        gsl::at(cpu.reg, instruction.dr) = gsl::at(cpu.reg, instruction.sr1) &
                                           gsl::at(cpu.reg, instruction.sr2);
    }

    cpu.setcc(instruction.dr);
}

void Lc3::op_not(const OperateInstruction instruction) {
    gsl::at(cpu.reg, instruction.dr) = ~gsl::at(cpu.reg, instruction.sr1);
    cpu.setcc(instruction.dr);
}

void Lc3::op_st(const DataMovementInstruction instruction) {
    uint16_t pc = cpu.reg[R_PC];
    uint16_t pc_offset = sext(instruction.pc_offset_9, 9);
    uint16_t mem_addr = pc + pc_offset;
    gsl::at(mem, mem_addr) = gsl::at(cpu.reg, instruction.dr);
}

void Lc3::op_str(const DataMovementInstruction instruction) {
    uint16_t pc_offset = sext(instruction.pc_offset_6, 6);
    uint16_t mem_addr = gsl::at(cpu.reg, instruction.base_r) + pc_offset;

    gsl::at(mem, mem_addr) = gsl::at(cpu.reg, instruction.dr);
    cpu.setcc(instruction.dr);
}

void Lc3::op_sti(const DataMovementInstruction instruction) {
    uint16_t pc = cpu.reg[R_PC];
    uint16_t pc_offset = sext(instruction.pc_offset_9, 9);
    uint16_t mem_addr = pc + pc_offset;

    gsl::at(mem, gsl::at(mem, mem_addr)) = gsl::at(cpu.reg, instruction.dr);
}

void Lc3::op_lea(const DataMovementInstruction instruction) {
    uint16_t pc = cpu.reg[R_PC];
    uint16_t pc_offset = sext(instruction.pc_offset_9, 9);
    uint16_t mem_addr = pc + pc_offset;

    gsl::at(cpu.reg, instruction.dr) = mem_addr;
    cpu.setcc(instruction.dr);
}

void Lc3::op_br(const ControlInstruction instruction) {
    uint16_t pc_offset = sext(instruction.pc_offset_9, 9);
    uint16_t r_cond = cpu.reg[R_COND];

    if ((instruction.n && r_cond == ConditionCode::NEG) ||
        (instruction.z && r_cond == ConditionCode::ZRO) ||
        (instruction.p && r_cond == ConditionCode::POS)) {

        cpu.reg[R_PC] += pc_offset;
    }
}

void Lc3::op_jmp(const ControlInstruction instruction) {
    cpu.reg[R_PC] = cpu.reg[instruction.base_r];
}

void Lc3::op_jsr(const ControlInstruction instruction) {
    cpu.reg[R_R7] = cpu.reg[R_PC];
    if (!instruction.r_flag) {
        cpu.reg[R_PC] = cpu.reg[instruction.base_r];
    } else {
        uint16_t pc_offset = sext(instruction.pc_offset_11, 11);
        cpu.reg[R_PC] += pc_offset;
    }
}

void Lc3::op_trap(const ControlInstruction instruction) {
    cpu.reg[R_R7] = cpu.reg[R_PC];
    switch (instruction.trapvect_8) {
    case TrapCode::Getc:
        cpu.reg[R_R0] = (uint16_t)getchar();
        break;
    case TrapCode::Out:
        static_cast<void>(putc((char)cpu.reg[R_R0], stdout));
        static_cast<void>(fflush(stdout));
        break;
    case TrapCode::Puts: {
        auto i = cpu.reg[R_R0];
        auto c = mem[i];
        while (c != 0x0000) {
            static_cast<void>(putc((char)c, stdout));
            c = mem[++i];
        }
        static_cast<void>(fflush(stdout));
        break;
    }
    case TrapCode::In:
        cpu.reg[R_R0] = (uint16_t)getchar();
        static_cast<void>(putc((char)cpu.reg[R_R0], stdout));
        static_cast<void>(fflush(stdout));
        break;
    case TrapCode::Putsp: {
        auto i = cpu.reg[R_R0];
        auto c = mem[i];
        while (c != 0x0000) {
            static_cast<void>(putc((char)(c & 0xFFU), stdout));
            static_cast<void>(putc((char)(c >> 8U), stdout));
            c = mem[++i];
        }
        static_cast<void>(fflush(stdout));
        break;
    }
    case TrapCode::Halt:
        fmt::print("HALT detected\n");
        static_cast<void>(fflush(stdout));
        // NOLINTNEXTLINE
        exit(1);
        break;
    default:
        fmt::print("Unknown trap. defaulting to exit.\n");
        // NOLINTNEXTLINE
        exit(1);
        break;
    }
}
