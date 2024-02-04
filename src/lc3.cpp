#include <array>
#include <filesystem>
#include <fmt/core.h>
#include <fmt/format.h>
#include <fstream>
#include <instructions.h>
#include <lc3.h>
#include <string>
#include <vector>
#define PC_START 0x3000

void Lc3::setup() {
    cpu.reg[R_COND] = ConditionCode::ZRO;
    cpu.reg[R_PC] = PC_START;
}

static uint16_t to_big_endian(const uint16_t bin) {
    return (uint16_t)(bin << 8U) | (uint16_t)(bin >> 8U);
}

void Lc3::load_image(const std::string& image_path) {
    const auto file_size = std::filesystem::file_size(image_path);
    const auto user_program_max_size = (UserProgramsEnd - UserProgramsStart);

    if (file_size > user_program_max_size) {
    }

    std::ifstream file(image_path, std::ios::binary | std::ios::in);

    if (!file || file.fail()) {
        throw std::invalid_argument("failed to load image file");
    };

    uint16_t base_address = 0;
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
    file.read(reinterpret_cast<char*>(&base_address), sizeof(uint16_t));
    base_address = to_big_endian(base_address);

    if (base_address < UserProgramsStart || base_address > UserProgramsEnd) {
        throw std::invalid_argument(
            fmt::format("image has invalid base address: {}", base_address));
    }

    while (!file.eof()) {
        uint16_t instruction = 0;

        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
        file.read(reinterpret_cast<char*>(&instruction), sizeof(uint16_t));
        mem[base_address++] = to_big_endian(instruction);
    }
    file.close();
}

void Lc3::run() {
    auto pc = cpu.reg[R_PC]++;
    auto instruction = mem[pc];
    uint16_t opcode = instruction >> 12U;

    switch (opcode) {
    case Opcode::BR:
        op_br(ControlInstruction::br(instruction));
        break;
    case Opcode::ADD:
        op_add(OperateInstruction(instruction));
        break;
    case Opcode::LD:
        op_ld(DataMovementInstruction::with_offset_9(instruction));
        break;
    case Opcode::ST:
        op_st(DataMovementInstruction::with_offset_9(instruction));
        break;
    case Opcode::JSR:
        op_jsr(ControlInstruction::jsr(instruction));
        break;
    case Opcode::AND:
        op_and(OperateInstruction(instruction));
        break;
    case Opcode::LDR:
        op_ldr(DataMovementInstruction::with_offset_6(instruction));
        break;
    case Opcode::STR:
        op_str(DataMovementInstruction::with_offset_6(instruction));
        break;
    case Opcode::NOT:
        op_not(OperateInstruction(instruction));
        break;
    case Opcode::LDI:
        op_ldi(DataMovementInstruction::with_offset_9(instruction));
        break;
    case Opcode::STI:
        op_sti(DataMovementInstruction::with_offset_9(instruction));
        break;
    case Opcode::JMP:
        op_jmp(ControlInstruction::jmp(instruction));
        break;
    case Opcode::LEA:
        op_lea(DataMovementInstruction::with_offset_9(instruction));
        break;
    case Opcode::TRAP:
        op_trap(ControlInstruction::trap(instruction));
        break;
    case Opcode::RES:
    case Opcode::RTI:
        fmt::print("Unsupported instruction");
        break;
    default:
        fmt::print("Unknown Instruction");
        return;
    }
}
