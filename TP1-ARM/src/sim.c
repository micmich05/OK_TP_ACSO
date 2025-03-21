#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>  // para strcmp
#include "shell.h"

// Se eliminó la declaración externa de adds_imm, ya que se implementa a continuación

// Funciones integradas de add.c

uint64_t zeroExtend(uint64_t imm, int datasize) {
    if (datasize >= 64)
        return imm;
    else
        return imm & ((1ULL << datasize) - 1);
}

uint64_t addWithCarry(uint64_t x, uint64_t y, int carry_in) {
    uint64_t unsigned_sum = x + y + carry_in;
    uint64_t result = unsigned_sum & ((1ULL << 64) - 1);

    // Actualiza banderas de estado
    NEXT_STATE.FLAG_N = (result >> 63) & 1;
    NEXT_STATE.FLAG_Z = (result == 0) ? 1 : 0;

    return result;
}

void adds_imm(uint32_t instr) {
    int d = (instr >> 0) & 0b11111;
    int n = (instr >> 5) & 0b11111;
    int datasize = 64;
    int imm12 = (instr >> 10) & 0b111111111111;
    int shift = (instr >> 22) & 0b11;
    // Inicialmente imm es imm12
    uint64_t imm = imm12;

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

    // Extiende a datasize (64 bits)
    imm = imm & ((1ULL << datasize) - 1);

    uint64_t operand1 = CURRENT_STATE.REGS[n];
    uint64_t result = addWithCarry(operand1, imm, 0);

    printf("d: %d\n", d);
    printf("imm: %llu\n", imm);
    printf("operand1: %llu\n", operand1);
    printf("result: %llu\n", result);
}

// Declaración de funciones definidas en shell.c
void initialize(const char *program_file, int num_files);
void get_command(FILE *dumpsim_file);

// Máscaras (de menor a mayor cantidad de bits)
#define OPCODE_MASK_26_31 0xFC000000  // bits [26–31] (6 bits)
#define OPCODE_MASK_24_31 0xFF000000  // bits [24–31] (8 bits)
#define OPCODE_MASK_23_31 0xFF800000  // bits [23–31] (9 bits)
#define OPCODE_MASK_22_31 0xFFC00000  // bits [22–31] (10 bits)
#define OPCODE_MASK_21_31 0xFFE00000  // bits [21–31] (11 bits)

typedef struct {
    uint32_t opcode;
    const char *mnemonic;
} OpcodeEntry;

OpcodeEntry opcode_dict[] = {
    {0b10110001, "ADD (Immediate)"},
    {0b10101011, "ADD (Extended register)"},
    {0b11101011, "SUB (Extended register)"},
    {0b11110001, "SUB (Immediate)"},
    {0b110100101, "MOV (Wide Inmediate)"},
    //{0b11010000, "MOV"},
    {0b11010010, "MOVZ"},
    {0b11101010, "ANDS (Shifted Register)"},
    {0b11001010, "EOR (Shifted Register)"},
    {0b1101001101, "LSL (Immediate)"},
    {0b01010100, "B COND"},
    {0b11111000000, "STUR"}, // Reconocer STURB, STUR y LDUR
    {0b11111000010, "LDUR"},
    {0b00111000000, "STURB"},
    {0b00111000010, "LDURB"},
    {0b11010100, "HALT"},
    {0, NULL}
};

void process_instruction() {
    uint32_t instr = mem_read_32(CURRENT_STATE.PC);  // Se lee la instrucción desde la memoria usando PC

    const uint32_t masks[] = {
        OPCODE_MASK_21_31,
        OPCODE_MASK_22_31,
        OPCODE_MASK_23_31,
        OPCODE_MASK_24_31,
        OPCODE_MASK_26_31
    };

    const int shifts[] = {21, 22, 23, 24, 26};

    for (int i = 0; i < 4; i++) {
        uint32_t key = (instr & masks[i]) >> shifts[i];

        for (int j = 0; opcode_dict[j].mnemonic != NULL; j++) {
            if (opcode_dict[j].opcode == key) {
                // Llama a adds_imm si se detecta "ADD (Immediate)"
                if (strcmp(opcode_dict[j].mnemonic, "ADD (Immediate)") == 0) {
                    adds_imm(instr);
                }
                // Si la instrucción es HALT se detiene la simulación
                else if (strcmp(opcode_dict[j].mnemonic, "HALT") == 0) {
                    RUN_BIT = FALSE;
                    exit(0);
                }
                // Si es otra instrucción, solo se muestra su mnemónico
                else {
                    printf("%s\n", opcode_dict[j].mnemonic);
                }

                // Actualiza el PC
                NEXT_STATE.PC = CURRENT_STATE.PC + 4;
                CURRENT_STATE = NEXT_STATE;
                return;
            }
        }
    }
    printf("Instruccion no encontrada\n");

    // Actualizar PC aún si la instrucción no fue reconocida
    NEXT_STATE.PC = CURRENT_STATE.PC + 4;
    CURRENT_STATE = NEXT_STATE;
}