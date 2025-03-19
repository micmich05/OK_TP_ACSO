#include <stdio.h>
#include <stdint.h>

// Máscaras (de menor a mayor cantidad de bits)
#define OPCODE_MASK_26_31 0xFC000000  // bits [26–31] (6 bits)
#define OPCODE_MASK_24_31 0xFF000000  // bits [24–31] (8 bits)
#define OPCODE_MASK_22_31 0xFFC00000  // bits [22–31] (10 bits)
#define OPCODE_MASK_21_31 0xFFE00000  // bits [21–31] (11 bits)
// #define OPCODE_MASK_22_29 0x3FC00000  // bits [22–29] (8 bits)

typedef struct {
    uint32_t opcode;
    const char *mnemonic;
} OpcodeEntry;

// Sólo ADD es “real”; los demás son relleno para mantener funcionalidad
OpcodeEntry opcode_dict[] = {
    
    {0b10110001, "ADD (Immediate)"},
    {0b10101011, "ADD (Extended register)"},
    {0b11101011, "SUB (Extended register)"},
    {0b11110001, "SUB (Immediate)"},

    {0b11010000, "MOV"},

    {0b11010010, "MOVZ"},

    {0b110100101, "MOV (Wide Inmediate)"},

    {0b11101010, "ANDS (Shifted Register)"},
    {0b11001010, "EOR (Shifted Register)"},

    {0b1101001101, "LSL (Immediate)"},

//    {0b11101011, "CMP"},
    {0b01010100, "B COND"},

    {0b11111000000, "STUR"}, //Reconocer STURB, STUR Y LDUR
    {0b11111000010, "LDUR"},

    {0b00111000000, "STURB"},
    {0b00111000010, "LDURB"},
    {0b11010100, "HALT"},

    // {0b00000000, "LDR"},
    // {0b00000000, "STR"},
    // {0b00000000, "BRANCH"},
    {0, NULL}
};


void decode_instruction(uint32_t instr) {
    const uint32_t masks[] = {
        OPCODE_MASK_21_31,
        OPCODE_MASK_22_31,
        OPCODE_MASK_24_31,
        // OPCODE_MASK_22_29,
        OPCODE_MASK_26_31
    };

    const int shifts[] = {21, 22, 24, 26};

    for (int i = 0; i < 4; i++) {
        uint32_t key = (instr & masks[i]) >> shifts[i];

        for (int j = 0; opcode_dict[j].mnemonic != NULL; j++) {
            if (opcode_dict[j].opcode == key) {
                printf("%s\n", opcode_dict[j].mnemonic);
                return;
            }
        }
    }
    printf("Instruccion no encontrada\n");
}

int main(void) {
    uint32_t instruction;
    printf("Ingresa instruccion (hex): ");
    if (scanf("%x", &instruction) != 1) {
        fprintf(stderr, "Formato invalido\n");
        return 1;
    }

    decode_instruction(instruction);
    return 0;
}
