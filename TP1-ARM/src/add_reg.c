void adds_reg(uint32_t instr) {
    int d = (instr >> 0) & 0x1F;         // bits [4:0]: registro destino
    int n = (instr >> 5) & 0x1F;         // bits [9:5]: registro fuente
    int m = (instr >> 16) & 0x1F;        // bits [20:16]: registro fuente 2
    int shift = (instr >> 22) & 0x3;     // bits [23:22]: tipo de shift
    int imm6 = (instr >> 10) & 0x3F;     // bits [15:10]: inmediato de 6 bits
    int datasize = 64;


    // Decode shift
    char shift_type[4];
    switch (shift) {
        case 0b00:
            strcpy(shift_type, "LSL");
            break;
        case 0b01:
            strcpy(shift_type, "LSR");
            break;
        case 0b10:
            strcpy(shift_type, "ASR");
            break;
        case 0b11:
            strcpy(shift_type, "ROR");
            break;
        default:
            printf("ReservedValue()\n");
            return;
    }

    int shift_amount = (unsigned int)imm6;

