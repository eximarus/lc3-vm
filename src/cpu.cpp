#include <cpu.h>
#include <gsl/gsl>

void Cpu::setcc(const uint16_t dr) {
    if (gsl::at(reg, dr) == 0) {
        reg[R_COND] = ConditionCode::ZRO;
    }
    /* a 1 in the left-most bit indicates negative */
    else if ((gsl::at(reg, dr) >> 15U) == 1) {
        reg[R_COND] = ConditionCode::NEG;
    } else {
        reg[R_COND] = ConditionCode::POS;
    }
}
