#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include "shell.h"

uint64_t zeroExtend(uint64_t imm, int datasize);
uint64_t addWithCarry(uint64_t x, uint64_t y, bool carry_in);

void adds_imm(uint32_t instr) {
    int d = (uint32_t)((instr >> 0) & 0b11111);
    int n = (uint32_t)((instr >> 5) & 0b11111);
    int datasize = 64;
    int imm12 = (instr >> 10) & 0b111111111111;
    int shift = (instr >> 22) & 0b11;
    uint64_t imm;


    switch (shift) {
        case 0b00:
            imm = zeroExtend(imm, datasize);
            break;
        case 0b01:
            imm = zeroExtend(imm << 12, datasize);
            break;
        default:
            printf("ReservedValue()\n");
            return;
    }

    // Zero extend imm to datasize
    imm = imm & ((1ULL << datasize) - 1);

    uint64_t result;
    uint64_t operand1 = CURRENT_STATE.REGS[n];

    result = addWithCarry(operand1, imm, 0);

    // NEXT_STATE.REGS[d] = result;
    // NEXT_STATE.PC = CURRENT_STATE.PC + 4;

    printf("d: %d\n", d);
    printf("imm: %llu\n", imm);
    printf("operand1: %llu\n", operand1);
    printf("result: %llu\n", result);
    // printf("NEXT_STATE.REGS[d]: %lld\n", NEXT_STATE.REGS[d]);
    // printf("NEXT_STATE.PC: %llu\n", NEXT_STATE.PC);
    // printf("result: %ld\n", result);
    // printf("NEXT_STATE.REGS[d]: %ld\n", NEXT_STATE.REGS[d]);
    // printf("NEXT_STATE.PC: %ld\n", NEXT_STATE.PC);

}

uint64_t zeroExtend(uint64_t imm, int datasize) {
    if (datasize >= 64) {
        return imm;
    } else {
        return imm & ((1ULL << datasize) - 1);
    }
}

uint64_t addWithCarry(uint64_t x, uint64_t y, bool carry_in) {
    int unsigned_sum = (uint64_t) x + (uint64_t) y + carry_in;
    uint64_t result = unsigned_sum & ((1ULL << 64) - 1);

    // Set flags
    NEXT_STATE.FLAG_N = (result >> 63) & 1;
    NEXT_STATE.FLAG_Z = (result == 0) ? 1 : 0;

    return result;
}

int main(void) {
    uint32_t instr = 0xb1800540;
    adds_imm(instr);
}

