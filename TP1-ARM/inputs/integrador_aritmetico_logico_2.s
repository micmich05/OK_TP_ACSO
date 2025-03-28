.text
    // --- ADDS Immediate (shift == 0) ---
    // Se espera: X0 = X1 + 3 → 10 + 3 = 13
    movz    X1, 10
    adds    X0, X1, 3

    // --- ADDS Immediate (shift == 1) ---
    // Con inmediato 1, se interpreta como 1 << 12 = 4096.
    // Se espera: X2 = 100 + 4096 = 4196
    movz    X1, 100
    adds    X2, X1, 1

    // --- ADDS Extended Register ---
    // Se espera: X3 = X1 + X2 → 20 + 30 = 50
    movz    X1, 20
    movz    X2, 30
    add     X3, X1, X2

    // --- SUBS Immediate (shift == 0) ---
    // Se espera: X4 = X1 - 3 → 50 - 3 = 47
    movz    X1, 50
    subis   X4, X1, 3

    // --- SUBS Extended Register ---
    // Se espera: X5 = X1 - X2 → 100 - 25 = 75
    movz    X1, 100
    movz    X2, 25
    subs    X5, X1, X2

    // --- CMP y BEQ ---
    // Comparamos dos registros iguales para que se cumpla BEQ.
    // Se espera saltar la siguiente instrucción.
    movz    X1, 55
    movz    X2, 55
    cmp     X1, X2
    beq     label_beq   // condición verdadera, se salta la siguiente
    adds    X6, X0, 1   // no se ejecuta
label_beq:
    adds    X6, X0, 2   // se ejecuta; X6 refleja que se saltó la instrucción anterior

    // --- ANDS (Shifted Register) ---
    // Se espera: X7 = X1 & X2; ejemplo: 0xF0F0 & 0x0FF0
    movz    X1, 0xF0F0
    movz    X2, 0x0FF0
    ands    X7, X1, X2

    // --- EOR (Shifted Register) ---
    // Se espera: X8 = X1 XOR X2; ejemplo: 0xAAAA XOR 0x5555
    movz    X1, 0xAAAA
    movz    X2, 0x5555
    eor     X8, X1, X2

    // --- ORR (Shifted Register) ---
    // Se espera: X9 = X1 | X2; ejemplo: 0x1234 OR 0x00FF
    movz    X1, 0x1234
    movz    X2, 0x00FF
    orr     X9, X1, X2

    // --- MUL ---
    // Se espera: X10 = X1 * X2; ejemplo: 7 * 6 = 42
    movz    X1, 7
    movz    X2, 6
    mul     X10, X1, X2

    // --- LSL (Immediate) ---
    // Se espera: X11 = X3 << 4; con X3 = 2 → 2 << 4 = 32
    movz    X3, 2
    lsl     X11, X3, 4

    // --- LSR (Immediate) ---
    // Se espera: X12 = X3 >> 1; con X3 = 2 → 2 >> 1 = 1
    lsr     X12, X3, 1

    // --- CBZ ---
    // Si X13 == 0, salta la instrucción siguiente.
    movz    X13, 0
    cbz     X13, label_cbz
    adds    X14, X0, 1   // no se ejecuta
label_cbz:
    // --- CBNZ ---
    // Si X13 != 0, salta la instrucción siguiente.
    movz    X13, 5
    cbnz    X13, label_cbnz
    adds    X15, X0, 1   // no se ejecuta
label_cbnz:
    HLT     0
