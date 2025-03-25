#include <stdbool.h>
#include <stdint.h>
void bcond (uint32_t instr){
    //Decode
    int cond = (instr >> 0) & 0xF;
    int imm19 = (instr >> 5) & 0x7FFFF;

    uint64_t offset = SignExtend(imm19 << 2);

    //Execute
    if (check_cond(cond)){
        //Update PC
        NEXT_STATE.PC = CURRENT_STATE.PC + offset;
    }
    else{
        //Update PC
        NEXT_STATE.PC = CURRENT_STATE.PC + 4;
    }
}

bool check_cond(int cond){
    switch (cond){
        case 0b0000: //Equal
            return CURRENT_STATE.FLAG_Z;
        case 0b0001: //Not equal
            return !CURRENT_STATE.FLAG_Z;
        case 0b1010: //Greater than or equal
            return CURRENT_STATE.FLAG_N == 0;
        case 0b1011: //Less than
            return CURRENT_STATE.FLAG_N != 0;
        case 0b1100: //Greater than
            return !CURRENT_STATE.FLAG_Z && (CURRENT_STATE.FLAG_N == 0);
        case 0b1101: //Less than or equal
            return CURRENT_STATE.FLAG_Z || (CURRENT_STATE.FLAG_N != 0);
        default:
            return false;
    }
}

uint64_t SignExtend(uint64_t value, int bits) {
    uint64_t mask = 1ULL << (bits - 1);
    return (value ^ mask) - mask;
}