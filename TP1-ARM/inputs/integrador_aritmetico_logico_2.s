.text
    // --- ADDS Immediate (shift == 0) ---
    // X1 = 10; X0 = X1 + 3  → 10 + 3 = 13
    movz    X1, 10
    adds    X0, X1, 3

    // --- ADDS Immediate (shift == 1) ---
    // X1 = 100; inmediato = 1 se interpreta como (1 << 12) = 4096
    // X2 = 100 + 4096 = 4196
    movz    X1, 100
    adds    X2, X1, 1

    // --- ADDS Extended Register ---
    // X1 = 20, X2 = 30  → X3 = 20 + 30 = 50
    movz    X1, 20
    movz    X2, 30
    adds_register   X3, X1, X2

    // --- SUBS Immediate (shift == 0) ---
    // X1 = 50; X4 = X1 - 3  → 50 - 3 = 47
    movz    X1, 50
    subis   X4, X1, 3

    // --- SUBS Extended Register ---
    // X1 = 100, X2 = 25  → X5 = 100 - 25 = 75
    movz    X1, 100
    movz    X2, 25
    subs_register  X5, X1, X2

    // --- CMP y BEQ ---
    // Configuramos X1 y X2 iguales para que se cumpla BEQ y se salte la siguiente instrucción.
    movz    X1, 55
    movz    X2, 55
    cmp     X1, X2
    beq     label_beq   // se debe saltar la instrucción siguiente
    adds    X6, X0, 1   // no se ejecuta
label_beq:
    adds    X6, X0, 2   // se ejecuta

    // --- ANDS (Shifted Register) ---
    // X1 = 0xF0F0, X2 = 0x0FF0 → X7 = X1 & X2
    movz    X1, 0xF0F0
    movz    X2, 0x0FF0
    ands    X7, X1, X2

    // --- EOR (Shifted Register) ---
    // X1 = 0xAAAA, X2 = 0x5555 → X8 = X1 XOR X2
    movz    X1, 0xAAAA
    movz    X2, 0x5555
    eor     X8, X1, X2

    // --- ORR (Shifted Register) ---
    // X1 = 0x1234, X2 = 0x00FF → X9 = X1 | X2
    movz    X1, 0x1234
    movz    X2, 0x00FF
    orr     X9, X1, X2

    // --- MUL ---
    // X1 = 7, X2 = 6 → X10 = 42
    movz    X1, 7
    movz    X2, 6
    mul     X10, X1, X2

    // --- LSL (Immediate) ---
    // Suponiendo que el ensamblador codifica "lsl X11, X3, 4" con:
    //   - X3 = 2; se espera X11 = 2 << 4 = 32.
    movz    X3, 2
    lsl     X11, X3, 4

    // --- LSR (Immediate) ---
    // Con X3 = 32; se espera X12 = 32 >> 1 = 16.
    lsr     X12, X3, 1

    // --- CBZ ---
    // Si X13 == 0 se salta la instrucción siguiente.
    movz    X13, 0
    cbz     X13, label_cbz
    adds    X14, X0, 1   // no se ejecuta
label_cbz:
    // --- CBNZ ---
    // Si X13 != 0 se salta la instrucción siguiente.
    movz    X13, 5
    cbnz    X13, label_cbnz
    adds    X15, X0, 1   // no se ejecuta
label_cbnz:
    HLT     0
