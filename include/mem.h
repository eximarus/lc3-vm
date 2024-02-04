#pragma once
#include <array>
#include <cstdint>
#define MEMORY_MAX (1U << 16U)

enum {
    TrapVectorTableStart = 0x0000,
    TrapVectorTableEnd = 0x00FF,
    InterruptVectorTableStart = 0x0100,
    InterruptVectorTableEnd = 0x01FF,
    OsAndSupervisorStackStart = 0x0200,
    OsAndSupervisorStackEnd = 0x02FFF,
    UserProgramsStart = 0x3000,
    UserProgramsEnd = 0xFDFF,
    DeviceRegisterAddrStart = 0xFE00,
    DeviceRegisterAddrEnd = 0xFFFF,
};

using Mem = std::array<uint16_t, MEMORY_MAX>;
