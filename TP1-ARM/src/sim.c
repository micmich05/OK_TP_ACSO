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
void adds_register(uint32_t instr);
void ands(uint32_t instr);
void eor(uint32_t instr);
void logical_shift(uint32_t instr);
void movz (uint32_t instr);
void sturb (uint32_t instr);
void stur (uint32_t instr);
void ldur (uint32_t instr);
void ldurb (uint32_t instr);
uint8_t mem_read_8(uint64_t addr);
void mem_write_8(uint64_t addr, uint8_t value);
uint64_t mem_read_64(uint64_t addr);
void mem_write_64(uint64_t addr, uint64_t value);
void orr (uint32_t instr);
void b(uint32_t instr);
void br(uint32_t instr);
void sturh(uint32_t instr);
void ldurh(uint32_t instr);
void mul(uint32_t instr);
void cbz(uint32_t instr);
void cbnz(uint32_t instr);
void update_pc();
void update_flags(uint64_t result);
void adds_imm(uint32_t instr);
void add_imm(uint32_t instr);
void process_instruction();
void add_register(uint32_t instr);






// Se eliminó la declaración externa de adds_imm, ya que se implementa a continuación

// Funciones integradas de add.c



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
    {0b10110001, "ADDS (Immediate)"},
    {0b10010001, "ADD (Immediate)"},
    {0b10101011, "ADDS (Extended register)"},
    {0b10001011, "ADD (Extended register)"},
    {0b11101011, "SUBS (Extended register)"},
    {0b11110001, "SUBS (Immediate)"},
    {0b11010010, "MOVZ"},
    {0b11101010000, "ANDS (Shifted Register)"},
    {0b11001010, "EOR (Shifted Register)"},
    {0b1101001101, "LOGICAL SHIFT"},
    {0b01010100, "B COND"},
    {0b11111000000, "STUR"}, 
    {0b11111000010, "LDUR"},
    {0b00111000000, "STURB"},
    {0b00111000010, "LDURB"},
    {0b11010100, "HALT"},
    {0b10101010000,"ORR (Shifted Register)"},
    {0b000101, "B"},
    {0b11010110, "BR"},
    {0b01111000000, "STURH"},
    {0b01111000010, "LDURH"},
    {0b10011011000, "MUL"},
    {0b10110100, "CBZ"},
    {0b10110101, "CBNZ"},
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

    for (int i = 0; i < 5; i++) {
        uint32_t key = (instr & masks[i]) >> shifts[i];

        for (int j = 0; opcode_dict[j].mnemonic != NULL; j++) {
            if (opcode_dict[j].opcode == key) {


                // Llama a adds_imm si se detecta "ADD (Immediate)"
                if (strcmp(opcode_dict[j].mnemonic, "ADDS (Immediate)") == 0) {
                    adds_imm(instr);
                }
                // Llama a bcond si se detecta "B COND"
                else if (strcmp(opcode_dict[j].mnemonic, "B COND") == 0) {
                    bcond(instr);
                }
                else if (strcmp(opcode_dict[j].mnemonic, "SUBS (Immediate)") == 0) {
                    subis(instr);
                }
                // Si la instrucción es HALT se detiene la simulación
                else if (strcmp(opcode_dict[j].mnemonic, "HALT") == 0) {
                    RUN_BIT = FALSE;
                    NEXT_STATE.PC = CURRENT_STATE.PC + 4;

                }
                else if (strcmp(opcode_dict[j].mnemonic, "SUBS (Extended register)") == 0) {
                    subs_register(instr);
                }
                else if (strcmp(opcode_dict[j].mnemonic, "ADDS (Extended register)") == 0) {

                    adds_register(instr);
                }
                else if (strcmp(opcode_dict[j].mnemonic, "ANDS (Shifted Register)") == 0) {
                    ands(instr);
                }
                else if( strcmp(opcode_dict[j].mnemonic, "ADD (Immediate)") == 0) {
                    add_imm(instr);
                }
                else if (strcmp(opcode_dict[j].mnemonic, "ADD (Extended register)") == 0) {
                    add_register(instr);
                }

                else if (strcmp(opcode_dict[j].mnemonic, "EOR (Shifted Register)") == 0) {
                    eor(instr);
                }
                else if (strcmp(opcode_dict[j].mnemonic, "LOGICAL SHIFT") == 0) {
                    logical_shift(instr);
                }
                else if (strcmp(opcode_dict[j].mnemonic, "MOVZ") == 0) {
                    movz(instr);
                }
                else if (strcmp(opcode_dict[j].mnemonic, "STUR") == 0) {
                    stur(instr);
                }
                else if (strcmp(opcode_dict[j].mnemonic, "LDUR") == 0) {
                    ldur(instr);
                }
                else if (strcmp(opcode_dict[j].mnemonic, "STURB") == 0) {
                    sturb(instr);
                }
                else if (strcmp(opcode_dict[j].mnemonic, "LDURB") == 0) {
                    ldurb(instr);
                }
                else if (strcmp(opcode_dict[j].mnemonic, "ORR (Shifted Register)") == 0) {
                    orr(instr);
                }
                else if (strcmp(opcode_dict[j].mnemonic, "B") == 0) {
                    b(instr);
                }
                else if (strcmp(opcode_dict[j].mnemonic, "BR") == 0) {
                    br(instr);
                }
                
                else if (strcmp(opcode_dict[j].mnemonic, "STURH") == 0) {
                    sturh(instr);
                }
                else if (strcmp(opcode_dict[j].mnemonic, "LDURH") == 0) {
                    ldurh(instr);
                }
                else if (strcmp(opcode_dict[j].mnemonic, "MUL") == 0) {
                    mul(instr);
                }
                else if (strcmp(opcode_dict[j].mnemonic, "CBZ") == 0) {
                    cbz(instr);
                }
                else if (strcmp(opcode_dict[j].mnemonic, "CBNZ") == 0) {
                    cbnz(instr);
                }
                
                // Si es otra instrucción, solo se muestra su mnemónico
                else {

                    // Actualiza el PC para instrucciones que no modifican la branch
                    NEXT_STATE.PC = CURRENT_STATE.PC + 4;
                }
                NEXT_STATE.REGS[31] = 0;
                CURRENT_STATE = NEXT_STATE;
                return;
            }
        }
    }

    NEXT_STATE.PC = CURRENT_STATE.PC + 4;
    CURRENT_STATE = NEXT_STATE;
}
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
    }

    // Extiende a datasize (64 bits)
    imm = imm & ~0ULL;

    uint64_t result = CURRENT_STATE.REGS[n] + imm;

    // Guardar el resultado en el registro destino
    NEXT_STATE.REGS[d] = result;

    //actualizo flags
    update_flags(result);


    // Actualiza el PC para avanzar a la siguiente instrucción
    update_pc();
}

void add_imm(uint32_t instr) {
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
    }

    // Extiende a datasize (64 bits)
    imm = imm & ~0ULL;

    uint64_t result = CURRENT_STATE.REGS[n] + imm;

    // Guardar el resultado en el registro destino
    NEXT_STATE.REGS[d] = result;

    // Actualiza el PC para avanzar a la siguiente instrucción
    update_pc();
}




void bcond (uint32_t instr){
    // Decode
    int cond = (instr >> 0) & 0xF;
    int imm19 = (instr >> 5) & 0x7FFFF;
    uint64_t offset = SignExtend(imm19 << 2, 21);

    // Execute
    if (check_cond(cond)){
        // El target es (PC + 4) + offset
        NEXT_STATE.PC = CURRENT_STATE.PC + offset;
    }
    else{
        update_pc();
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

    uint64_t result = CURRENT_STATE.REGS[n] - imm;

    // Guardar el resultado en el registro destino
    NEXT_STATE.REGS[d] = result;

    //actualizar flags
    update_flags(result);



    //Update PC
    update_pc();
    
}

void subs_register(uint32_t instr){
    //Decode
    int d = (unsigned int)((instr >> 0) & 0x1F);         // bits [4:0]: registro destino
    int n = (unsigned int)((instr >> 5) & 0x1F);         // bits [9:5]: registro fuente
    int m = (unsigned int)((instr >> 16) & 0x1F);        // bits [20:16]: registro fuente 2

    // Extiende a datasize (64 bits)

    uint64_t result = CURRENT_STATE.REGS[n] - CURRENT_STATE.REGS[m];

    // Guardar el resultado en el registro destino
    NEXT_STATE.REGS[d] = result;

    //actualizar flags
    update_flags(result);

    //Update PC
    update_pc();
    
}

void adds_register(uint32_t instr){
    // Decode
    int d = (instr >> 0) & 0x1F;         // bits [4:0]: registro destino
    int n = (instr >> 5) & 0x1F;         // bits [9:5]: registro fuente
    int m = (instr >> 16) & 0x1F;        // bits [20:16]: registro fuente 2

    // Realizar la suma: resultado = Rn + ext_rm
    uint64_t result = CURRENT_STATE.REGS[n] + CURRENT_STATE.REGS[m];
    
    // Guardar el resultado en el registro destino
    NEXT_STATE.REGS[d] = result;

    //Actualiza flags
    update_flags(result);

    // Actualizar el PC para avanzar a la siguiente instrucción
    update_pc();
    
}

void add_register(uint32_t instr){
    // Decode
    int d = (instr >> 0) & 0x1F;         // bits [4:0]: registro destino
    int n = (instr >> 5) & 0x1F;         // bits [9:5]: registro fuente
    int m = (instr >> 16) & 0x1F;        // bits [20:16]: registro fuente 2

    // Realizar la suma: resultado = Rn + ext_rm
    uint64_t result = CURRENT_STATE.REGS[n] + CURRENT_STATE.REGS[m];
    
    // Guardar el resultado en el registro destino
    NEXT_STATE.REGS[d] = result;


    // Actualizar el PC para avanzar a la siguiente instrucción
    update_pc();
    
}

void ands(uint32_t instr){
    // Decode
    int d = (instr >> 0) & 0x1F;         // bits [4:0]: registro destino
    int n = (instr >> 5) & 0x1F;         // bits [9:5]: registro fuente
    int m = (instr >> 16) & 0x1F;        // bits [20:16]: registro fuente 2

    // Inicialmente imm es imm12

    // Extiende a datasize (64 bits)

    uint64_t result = CURRENT_STATE.REGS[n] & CURRENT_STATE.REGS[m];

    // Guardar el resultado en el registro destino
    NEXT_STATE.REGS[d] = result;

    //actualizar flags
    update_flags(result);

    //Update PC
    update_pc();
}

void eor (uint32_t instr){
    // Decode
    int d = (instr >> 0) & 0x1F;         // bits [4:0]: registro destino
    int n = (instr >> 5) & 0x1F;         // bits [9:5]: registro fuente
    int m = (instr >> 16) & 0x1F;        // bits [20:16]: registro fuente 2

    // Inicialmente imm es imm12

    uint64_t result = CURRENT_STATE.REGS[n] ^ CURRENT_STATE.REGS[m];

    // Guardar el resultado en el registro destino

    NEXT_STATE.REGS[d] = result;

    //Update PC
    update_pc();

}

void logical_shift (uint32_t instr){
    // Decode
    int d = (instr >> 0) & 0x1F;         // bits [4:0]: registro destino
    int n = (instr >> 5) & 0x1F;         // bits [9:5]: registro fuente
    uint8_t imm6 = (instr >> 10) & 0x3F; // bits [15:10]
    uint8_t immr = (instr >> 16) & 0x3F; // bits [21:16]
    uint64_t result;
    
    
    // Para instrucciones LSL, el inmediato efectivo es: 64 - immr.
    // Si imm6 es 0x3F se trata de un LSR, y se usa immr directamente.
    if (imm6 == 0x3F) {
        result = CURRENT_STATE.REGS[n] >> immr;
    } else {
        result = CURRENT_STATE.REGS[n] << (64 - immr);
    }
    
    NEXT_STATE.REGS[d] = result;
    
    // Update PC
    update_pc();
}

void movz (uint32_t instr){
    // Decode
    int d = (instr >> 0) & 0x1F;         // bits [4:0]: registro destino
    int imm16 = (instr >> 5) & 0xFFFF;   // bits [20:5]

    // Inicialmente imm es imm12
    uint64_t imm = imm16;
    imm = zeroExtend(imm, 64);

    // Guardar el resultado en el registro destino
    NEXT_STATE.REGS[d] = imm;

    //Update PC
    update_pc();
}

void sturb (uint32_t instr){
    // Decode
    int t = (instr >> 0) & 0x1F;         // bits [4:0]: registro destino
    int n = (instr >> 5) & 0x1F;         // bits [9:5]: registro fuente
    int imm9 = (instr >> 12) & 0x1FF;   // bits [20:12]

    // Inicialmente imm es imm12
    uint64_t imm = imm9;
    imm = zeroExtend(imm, 64);

    // Guardar el resultado en el registro destino
    mem_write_8(CURRENT_STATE.REGS[n] + imm, CURRENT_STATE.REGS[t]);

    // Los flags FLAG_N y FLAG_Z se actualizan en addWithCarry.
    // Si es necesario actualizar otros flags, se podría hacer aquí.

    //Update PC
    update_pc();
}

void stur (uint32_t instr){
    // Decode
    int t = (instr >> 0) & 0x1F;         // bits [4:0]: registro destino
    int n = (instr >> 5) & 0x1F;         // bits [9:5]: registro fuente
    int imm9 = (instr >> 12) & 0x1FF;   // bits [20:12]

    // Inicialmente imm es imm12
    uint64_t imm = imm9;
    imm = zeroExtend(imm, 64);

    // Guardar el resultado en el registro destino
    mem_write_64(CURRENT_STATE.REGS[n] + imm, CURRENT_STATE.REGS[t]);

    // Los flags FLAG_N y FLAG_Z se actualizan en addWithCarry.
    // Si es necesario actualizar otros flags, se podría hacer aquí.


    //Update PC
    update_pc();
}

void ldur (uint32_t instr){
    // Decode
    int t = (instr >> 0) & 0x1F;         // bits [4:0]: registro destino
    int n = (instr >> 5) & 0x1F;         // bits [9:5]: registro fuente
    int imm9 = (instr >> 12) & 0x1FF;   // bits [20:12]

    // Inicialmente imm es imm12
    uint64_t imm = imm9;
    imm = zeroExtend(imm, 64);

    // Guardar el resultado en el registro destino
    NEXT_STATE.REGS[t] = mem_read_64(CURRENT_STATE.REGS[n] + imm);

    //Update PC
    update_pc();
}

void ldurb (uint32_t instr){
    // Decode
    int t = (instr >> 0) & 0x1F;         // bits [4:0]: destino, en este caso W1
    int n = (instr >> 5) & 0x1F;         // bits [9:5]: fuente, en este caso X2
    int imm9 = (instr >> 12) & 0x1FF;      // bits [20:12]: offset inmediato

    // Zero extend the immediate value
    uint64_t imm = zeroExtend(imm9, 64);

    // For ldurb W1, [X2, #0x10], imm decodificado debe ser 0x10.
    // Lee 8 bits de memoria en la dirección (X2 + imm),
    // luego los extiende a 64 bits (los 56 bits altos se ponen a 0)
    uint8_t loaded_val = mem_read_8(CURRENT_STATE.REGS[n] + imm);
    NEXT_STATE.REGS[t] = (uint64_t) loaded_val;
    


    // Actualiza PC para la siguiente instrucción
    update_pc();
}

void orr (uint32_t instr){
    // Decode
    int d = (instr >> 0) & 0x1F;         // bits [4:0]: registro destino
    int n = (instr >> 5) & 0x1F;         // bits [9:5]: registro fuente
    int m = (instr >> 16) & 0x1F;        // bits [20:16]: registro fuente 2
    
    uint64_t result = CURRENT_STATE.REGS[m] | CURRENT_STATE.REGS[n];
    
    // Guardar el resultado en el registro destino
    NEXT_STATE.REGS[d] = result;
    // Actualizar el PC para avanzar a la siguiente instrucción
    update_pc();
    
}

void b(uint32_t instr){
    // Decode
    int imm26 = (instr >> 0) & 0x3FFFFFF;   // bits [25:0]
    uint64_t offset = SignExtend(imm26 << 2, 28);

    // Execute
    // El target es (PC + 4) + offset
    NEXT_STATE.PC = CURRENT_STATE.PC  + offset;
}

void br(uint32_t instr){
    // Decode
    int n = (instr >> 5) & 0x1F;         // bits [9:5]: registro fuente

    // El target es el contenido del registro n
    NEXT_STATE.PC = CURRENT_STATE.REGS[n];
}

void sturh (uint32_t instr){
    // Decode
    int t = (instr >> 0) & 0x1F;         // bits [4:0]: registro destino
    int n = (instr >> 5) & 0x1F;         // bits [9:5]: registro fuente
    int imm9 = (instr >> 12) & 0x1FF;   // bits [20:12]

    // Inicialmente imm es imm12
    uint64_t imm = imm9;
    imm = zeroExtend(imm, 64);

    // Guardar el resultado en el registro destino
    mem_write_8(CURRENT_STATE.REGS[n] + imm, CURRENT_STATE.REGS[t]);

    // Los flags FLAG_N y FLAG_Z se actualizan en addWithCarry.
    // Si es necesario actualizar otros flags, se podría hacer aquí.


    //Update PC
    update_pc();
}

void ldurh (uint32_t instr){
    // Decode
    int t = (instr >> 0) & 0x1F;         // bits [4:0]: registro destino
    int n = (instr >> 5) & 0x1F;         // bits [9:5]: registro fuente
    int imm9 = (instr >> 12) & 0x1FF;   // bits [20:12]

    // Inicialmente imm es imm12
    uint64_t imm = imm9;
    imm = zeroExtend(imm, 64);

    // Guardar el resultado en el registro destino
    uint8_t loaded_val = mem_read_8(CURRENT_STATE.REGS[n] + imm);
    NEXT_STATE.REGS[t] = (uint64_t) loaded_val;


    //Update PC
    update_pc();
}

void mul(uint32_t instr){
    // Decode
    int d = (instr >> 0) & 0x1F;         // bits [4:0]: registro destino
    int n = (instr >> 5) & 0x1F;         // bits [9:5]: registro fuente
    int m = (instr >> 16) & 0x1F;        // bits [20:16]: registro fuente 2

    uint64_t result = CURRENT_STATE.REGS[n] * CURRENT_STATE.REGS[m];

    // Guardar el resultado en el registro destino
    NEXT_STATE.REGS[d] = result;

    // Actualizar el PC para avanzar a la siguiente instrucción
    update_pc();

}

void cbz (uint32_t instr){
    // Decode
    int t = (instr >> 0) & 0x1F;         // bits [4:0]: registro destino
    int imm19 = (instr >> 5) & 0x7FFFF;   // bits [23:5]
    uint64_t offset = SignExtend(imm19 << 2, 21);

    // Execute
    if (CURRENT_STATE.REGS[t] == 0){

        NEXT_STATE.PC = CURRENT_STATE.PC + offset;
    }
    else{
        update_pc();
    }
}

void cbnz (uint32_t instr){
    // Decode
    int t = (instr >> 0) & 0x1F;         // bits [4:0]: registro destino
    int imm19 = (instr >> 5) & 0x7FFFF;   // bits [23:5]
    uint64_t offset = SignExtend(imm19 << 2, 21);

    // Execute
    if (CURRENT_STATE.REGS[t] != 0){

        NEXT_STATE.PC = CURRENT_STATE.PC + offset;
    }
    else{
        update_pc();
    }
}

///////////FUNCIONES AUXILIARES/////////////

bool check_cond(int cond){
    switch (cond){
        case 0b0000: // Equal
            return CURRENT_STATE.FLAG_Z == 1;
        case 0b0001: // Not equal
            return CURRENT_STATE.FLAG_Z == 0;
        case 0b1010: // Greater than or equal
            return CURRENT_STATE.FLAG_N == 0;
        case 0b1011: // Less than
            return CURRENT_STATE.FLAG_N == 1;
        case 0b1100: // Greater than
            return CURRENT_STATE.FLAG_N == 0 && CURRENT_STATE.FLAG_Z == 0;
        case 0b1101: // Less than or equal
            return (CURRENT_STATE.FLAG_N == 1) || (CURRENT_STATE.FLAG_Z == 1);
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


// Lee un byte de memoria usando mem_read_32
uint8_t mem_read_8(uint64_t addr) {
    uint64_t aligned_addr = addr & ~0x3ULL; // Alinear la dirección a 4 bytes
    int shift = (addr & 0x3) * 8;           // Desfase interno (0 a 3)
    uint32_t word = mem_read_32(aligned_addr);
    return (word >> shift) & 0xFF;
}

// Escribe un byte en memoria usando mem_read_32 y mem_write_32
void mem_write_8(uint64_t addr, uint8_t value) {
    uint64_t aligned_addr = addr & ~0x3ULL;
    int shift = (addr & 0x3) * 8;
    uint32_t word = mem_read_32(aligned_addr);
    word &= ~(0xFFU << shift);            // Limpiar los 8 bits correspondientes
    word |= ((uint32_t)value << shift);   // Insertar el valor
    mem_write_32(aligned_addr, word);
}

// Lee 64 bits de memoria usando dos lecturas de 32 bits consecutivas
uint64_t mem_read_64(uint64_t addr) {
    uint32_t low = mem_read_32(addr);
    uint32_t high = mem_read_32(addr + 4);
    return (((uint64_t) high) << 32) | low;
}

// Escribe 64 bits en memoria usando dos escrituras de 32 bits consecutivas
void mem_write_64(uint64_t addr, uint64_t value) {
    uint32_t low = (uint32_t)(value & 0xFFFFFFFF);
    uint32_t high = (uint32_t)(value >> 32);
    mem_write_32(addr, low);
    mem_write_32(addr + 4, high);
}

void update_flags(uint64_t result) {
    NEXT_STATE.FLAG_N = (result >> 63) & 1; // Actualiza FLAG_N
    NEXT_STATE.FLAG_Z = (result == 0) ? 1 : 0; // Actualiza FLAG_Z
}

void update_pc()
{
    NEXT_STATE.PC = CURRENT_STATE.PC + 4;
}