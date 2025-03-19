#include <stdint.h>
#include <stdio.h>

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
    uint64_t operand1;

    if (n==31) {
        operand1 = 
    } else {
        result = x[n] + imm;
    }

}

uint64_t zeroExtend(uint64_t imm, int datasize) {
    if (datasize >= 64) {
        return imm;
    } else {
        return imm & ((1ULL << datasize) - 1);
    }
}

int main(void) {
    uint32_t instr = 0xb1800540;
    adds_imm(instr);
}

