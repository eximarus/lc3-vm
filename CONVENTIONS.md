xNumber The number in hexadecimal notation.

\#Number The number in decimal notation.

A[l:r] The field delimited by bit [l] on the left and bit [r] on the right, of the datum A. For
example, if PC contains 0011001100111111, then PC[15:9] is 0011001. PC[2:2]
is 1. If l and r are the same bit number, the notation is usually abbreviated PC[2].

BaseR Base Register; one of R0..R7, used in conjunction with a six-bit offset to compute
Base+offset addresses.

DR Destination Register; one of R0..R7, which specifies which register the result of an
instruction should be written to.

imm5 A 5-bit immediate value; bits [4:0] of an instruction when used as a literal
(immediate) value. Taken as a 5-bit, 2’s complement integer, it is sign-extended to
16 bits before it is used. Range: −16..15.

LABEL An assembly language construct that identifies a location symbolically (i.e., by means
of a name, rather than its 16-bit address).

mem[address] Denotes the contents of memory at the given address.

offset6 A 6-bit value; bits [5:0] of an instruction; used with the Base+offset addressing mode.
Bits [5:0] are taken as a 6-bit signed 2’s complement integer, sign-extended to
16 bits and then added to the Base Register to form an address. Range: −32..31.

PC Program Counter; 16-bit register that contains the memory address of the next
instruction to be fetched. For example, during execution of the instruction at address
A, the PC contains address A + 1, indicating the next instruction is contained in
A + 1.

PCoffset9 A 9-bit value; bits [8:0] of an instruction; used with the PC+offset addressing mode.
Bits [8:0] are taken as a 9-bit signed 2’s complement integer, sign-extended to 16
bits and then added to the incremented PC to form an address. Range −256..255.

PCoffset11 An 11-bit value; bits [10:0] of an instruction; used with the JSR opcode to compute
the target address of a subroutine call. Bits [10:0] are taken as an 11-bit 2’s
complement integer, sign-extended to 16 bits and then added to the incremented PC
to form the target address. Range −1024..1023.

PSR Processor Status Register; 16-bit register that contains status information of the
process that is running. PSR[15] = privilege mode. PSR[2:0] contains the condition
codes. PSR[2] = N, PSR[1] = Z, PSR[0] = P.

setcc() Indicates that condition codes N, Z, and P are set based on the value of the result
written to DR. If the value is negative, N = 1, Z = 0, P = 0. If the value is zero,
N = 0, Z = 1, P = 0. If the value is positive, N = 0, Z = 0, P = 1.

SEXT(A) Sign-extend A. The most significant bit of A is replicated as many times as necessary to
extend A to 16 bits. For example, if A = 110000, then SEXT(A) = 1111 1111
1111 0000.

SP The current stack pointer. R6 is the current stack pointer. There are two stacks, one
for each privilege mode. SP is SSP if PSR[15] = 0; SP is USP if PSR[15] = 1.

SR, SR1, SR2 Source Register; one of R0..R7 which specifies the register from which a source
operand is obtained.

SSP The Supervisor Stack Pointer.

trapvect8 An 8-bit value; bits [7:0] of an instruction; used with the TRAP opcode to determine
the starting address of a trap service routine. Bits [7:0] are taken as an unsigned
integer and zero-extended to 16 bits. This is the address of the memory location
containing the starting address of the corresponding service routine. Range 0..255.

USP The User Stack Pointer.

ZEXT(A) Zero-extend A. Zeros are appended to the leftmost bit of A to extend it to 16 bits. For
example, if A = 110000, then ZEXT(A) = 0000 0000 0011 0000.
