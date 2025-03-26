#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>  // para strcmp
#include "shell.h"
uint64_t SignExtend(uint64_t value, int bits);
bool check_cond(int cond);
uint64_t zeroExtend(uint64_t imm, int datasize);
uint64_t addWithCarry(uint64_t x, uint64_t y, int carry_in) ;
void bcond(uint32_t instr);
void subis(uint32_t instr);
void subs_register(uint32_t instr);


// Se eliminó la declaración externa de adds_imm, ya que se implementa a continuación

// Funciones integradas de add.c

void adds_imm(uint32_t instr) {
    int d = (instr >> 0) & 0x1F;         // bits [4:0]: registro destino
    int n = (instr >> 5) & 0x1F;         // bits [9:5]: registro fuente
    int datasize = 64;
    int imm12 = (instr >> 10) & 0xFFF;   // bits [21:10]
    int shift = (instr >> 22) & 0x1;     // solo 1 bit en lugar de 2

    // Inicialmente imm es imm12
    uint64_t imm = imm12;

    switch (shift) {
        case 0:
            imm = zeroExtend(imm, datasize);
            break;
        case 1:
            imm = zeroExtend(imm << 12, datasize);
            break;
        default:
            printf("ReservedValue()\n");
            return;
    }

    // Extiende a datasize (64 bits)
    imm = imm & ~0ULL;

    uint64_t operand1 = CURRENT_STATE.REGS[n];
    uint64_t result = addWithCarry(operand1, imm, 0);

    // Guardar el resultado en el registro destino
    NEXT_STATE.REGS[d] = result;

    // Actualiza el PC para avanzar a la siguiente instrucción
    NEXT_STATE.PC = CURRENT_STATE.PC + 4;

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
                // Imprime el mnemónico de la instrucción detectada
                printf("Ejecutando %s\n", opcode_dict[j].mnemonic);

                // Llama a adds_imm si se detecta "ADD (Immediate)"
                if (strcmp(opcode_dict[j].mnemonic, "ADD (Immediate)") == 0) {
                    adds_imm(instr);
                }
                // Llama a bcond si se detecta "B COND"
                else if (strcmp(opcode_dict[j].mnemonic, "B COND") == 0) {
                    bcond(instr);
                }
                else if (strcmp(opcode_dict[j].mnemonic, "SUB (Immediate)") == 0) {
                    subis(instr);
                }
                // Si la instrucción es HALT se detiene la simulación
                else if (strcmp(opcode_dict[j].mnemonic, "HALT") == 0) {
                    RUN_BIT = FALSE;
                    printf("Ejecutando HALT: finalizando simulación\n");
                    exit(0);
                }
                else if (strcmp(opcode_dict[j].mnemonic, "SUB (Extended register)") == 0) {
                    subs_register(instr);
                }
                // Si es otra instrucción, solo se muestra su mnemónico
                else {
                    printf("%s\n", opcode_dict[j].mnemonic);
                    // Actualiza el PC para instrucciones que no modifican la branch
                    NEXT_STATE.PC = CURRENT_STATE.PC + 4;
                }

                CURRENT_STATE = NEXT_STATE;
                return;
            }
        }
    }
    printf("Instruccion no encontrada\n");
    // Actualizar PC aún si la instrucción no fue reconocida
    printf("PC: %d\n", CURRENT_STATE.PC);
    printf("Opcode: 0x%08X\n", instr);
    NEXT_STATE.PC = CURRENT_STATE.PC + 4;
    CURRENT_STATE = NEXT_STATE;
}

void bcond (uint32_t instr){
    // Decode
    int cond = (instr >> 0) & 0xF;
    int imm19 = (instr >> 5) & 0x7FFFF;
    uint64_t offset = SignExtend(imm19 << 2, 21);

    // Execute
    if (check_cond(cond)){
        printf("BCOND True: condition %d met, offset = %llu\n", cond, offset);
        // El target es (PC + 4) + offset
        NEXT_STATE.PC = CURRENT_STATE.PC + 4 + offset;
    }
    else{
        printf("BCOND False: condition %d not met, jumping to PC + 4\n", cond);
        NEXT_STATE.PC = CURRENT_STATE.PC + 4;
    }
}

void subis(uint32_t instr){
    //Decode
    int d = (unsigned int)((instr >> 0) & 0x1F);         // bits [4:0]: registro destino
    int n = (unsigned int)((instr >> 5) & 0x1F);         // bits [9:5]: registro fuente
    int datasize = 64;
    int imm12 = (instr >> 10) & 0xFFF;   // bits [21:10]
    int shift = (instr >> 22) & 0x1;     // solo 1 bit en lugar de 2

    // Inicialmente imm es imm12
    uint64_t imm = imm12;

    switch (shift) {
        case 0:
            imm = zeroExtend(imm, datasize);
            break;
        case 1:
            imm = zeroExtend(imm << 12, datasize);
            break;
    }

    // Extiende a datasize (64 bits)

    uint64_t operand1 = CURRENT_STATE.REGS[n];
    uint64_t operand2 = -(int64_t)imm;
    uint64_t result = addWithCarry(operand1, imm, 0); //TODO: Ver carry 0

    // Guardar el resultado en el registro destino
    NEXT_STATE.REGS[d] = result;

    // Los flags FLAG_N y FLAG_Z se actualizan en addWithCarry.
    // Si es necesario actualizar otros flags, se podría hacer aquí.

    printf("d: %d\n", d);
    printf("imm: %llu\n", imm);
    printf("operand1: %llu\n", operand1);
    printf("result: %llu\n", result);

    //Update PC
    NEXT_STATE.PC = CURRENT_STATE.PC + 4;
    
}

void subs_register(uint32_t instr){
    //Decode
    int d = (unsigned int)((instr >> 0) & 0x1F);         // bits [4:0]: registro destino
    int n = (unsigned int)((instr >> 5) & 0x1F);         // bits [9:5]: registro fuente
    int m = (unsigned int)((instr >> 16) & 0x1F);        // bits [20:16]: registro fuente 2
    int shift = (instr >> 22) & 0x3;     // 2 bits

    // Inicialmente imm es imm12
    uint64_t imm = CURRENT_STATE.REGS[m];

    switch (shift) {
        case 0:
            imm = zeroExtend(imm, 64);
            break;
        case 1:
            imm = zeroExtend(imm << 12, 64);
            break;
    }

    // Extiende a datasize (64 bits)

    uint64_t operand1 = CURRENT_STATE.REGS[n];
    uint64_t operand2 = -(int64_t)imm;
    uint64_t result = addWithCarry(operand1, imm, 0); //TODO: Ver carry 0

    // Guardar el resultado en el registro destino
    NEXT_STATE.REGS[d] = result;

    // Los flags FLAG_N y FLAG_Z se actualizan en addWithCarry.
    // Si es necesario actualizar otros flags, se podría hacer aquí.

    printf("d: %d\n", d);
    printf("imm: %llu\n", imm);
    printf("operand1: %llu\n", operand1);
    printf("result: %llu\n", result);

    //Update PC
    NEXT_STATE.PC = CURRENT_STATE.PC + 4;
    
}


///////////FUNCIONES AUXILIARES/////////////

bool check_cond(int cond){
    switch (cond){
        case 0b0000: //Equal
            return CURRENT_STATE.FLAG_Z == 1;
        case 0b0001: //Not equal
            return CURRENT_STATE.FLAG_Z == 0;
        case 0b1010: //Greater than or equal
            return CURRENT_STATE.FLAG_N == 0 || CURRENT_STATE.FLAG_Z == 1;
        case 0b1011: //Less than
            return CURRENT_STATE.FLAG_N != 1 && CURRENT_STATE.FLAG_Z == 0;
        case 0b1100: //Greater than
            return CURRENT_STATE.FLAG_N == 0 && CURRENT_STATE.FLAG_Z == 0;
        case 0b1101: //Less than or equal
            return CURRENT_STATE.FLAG_N == 1 || CURRENT_STATE.FLAG_Z == 1;
        default:
            return false;
    }
}

uint64_t SignExtend(uint64_t value, int bits) {
    uint64_t mask = 1ULL << (bits - 1);
    return (value ^ mask) - mask;
}

uint64_t zeroExtend(uint64_t imm, int datasize) {
    if (datasize >= 64)
        return imm;
    else
        return imm & ((1ULL << datasize) - 1);
}

uint64_t addWithCarry(uint64_t x, uint64_t y, int carry_in) {
    int unsigned_sum =(unsigned int) x + (unsigned int)y + (unsigned int)carry_in;
    uint64_t result = unsigned_sum & ((1ULL << 64) - 1);

    // Actualiza banderas de estado
    NEXT_STATE.FLAG_N = (result >> 63) & 1;
    NEXT_STATE.FLAG_Z = (result == 0) ? 1 : 0;

    return result;
}