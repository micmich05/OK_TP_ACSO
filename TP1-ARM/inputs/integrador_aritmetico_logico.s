.text
    // --- Operaciones aritméticas y flags ---
    // ADDS immediate (shift==00): X0 = X1 + 3; inicializamos X1 = 10.
    movz   X1, 10
    adds   X0, X1, 3      // X0 = 13

    // ADDS extended: X1 = X1 + X2; establecemos X2 = 7.
    movz   X2, 7
    adds   X1, X1, X2     // X1 = 10 + 7 = 17

    // SUBS immediate (shift==00): X0 = X1 - 3.
    subs   X0, X1, 3      // X0 = 17 - 3 = 14

    // SUBS extended: X2 = X1 - X2.
    subs   X2, X1, X2     // X2 = 17 - 7 = 10

    // --- Operaciones lógicas ---
    // ANDS: X3 = X0 & X2 = 14 & 10.
    ands   X3, X0, X2

    // EOR: X4 = X1 XOR X2 = 17 XOR 10.
    eor    X4, X1, X2

    // ORR: X5 = X1 | X2 = 17 OR 10.
    orr    X5, X1, X2

    // --- Suma con ADD (Extended Register e Immediate con shift==01) ---
    // ADD extended: X0 = X1 + X2; establecemos X1 = 2 y X2 = 7.
    movz   X1, 2
    movz   X2, 7
    add    X0, X1, X2     // X0 = 9

    // ADD immediate (shift==01): se desplaza el inmediato 12 bits a la izquierda.
    // Con imm = 1: X0 = X1 + (1 << 12). Establecemos X1 = 100.
    movz   X1, 100
    add    X0, X1, 1      // X0 = 100 + 4096 = 4196

    // --- Multiplicación ---
    // MUL: multiplicamos X1 * X2. Establecemos X2 = 5.
    movz   X2, 5
    mul    X0, X1, X2     // X0 = 100 * 5 = 500

    // --- Desplazamientos ---
    // LSL: X4 = X3 << 4. Con X3 = (resultado previo de ANDS).
    lsl    X4, X3, 4

    // LSR: X5 = X3 >> 4.
    lsr    X5, X3, 4

    // --- Pruebas de ramas condicionales ---
    // BEQ: Comparamos dos valores iguales.
    movz   X13, 50
    movz   X14, 50
    cmp    X13, X14
    beq    label_beq
    adds   X6, X0, 1     // Esta instrucción se salta
label_beq:

    // BNE: Comparamos valores distintos.
    movz   X13, 30
    movz   X14, 50
    cmp    X13, X14
    bne    label_bne
    adds   X7, X0, 1     // Saltado
label_bne:

    // BGT: X13 > X14.
    movz   X13, 60
    movz   X14, 50
    cmp    X13, X14
    bgt    label_bgt
    adds   X8, X0, 1     // Saltado
label_bgt:

    // BLT: X13 < X14.
    movz   X13, 40
    movz   X14, 50
    cmp    X13, X14
    blt    label_blt
    adds   X9, X0, 1     // Saltado
label_blt:

    // BGE: X13 >= X14.
    movz   X13, 50
    movz   X14, 50
    cmp    X13, X14
    bge    label_bge
    adds   X10, X0, 1    // Saltado
label_bge:

    // BLE: X13 <= X14.
    movz   X13, 40
    movz   X14, 50
    cmp    X13, X14
    ble    label_ble
    adds   X11, X0, 1    // Saltado
label_ble:

    // --- Pruebas CBZ y CBNZ ---
    // CBZ: salta si X3 es cero.
    movz   X3, 0
    cbz    X3, label_cbz
    adds   X12, X0, 1   // Saltado
label_cbz:

    // CBNZ: salta si X3 no es cero.
    movz   X3, 1
    cbnz   X3, label_cbnz
    adds   X13, X0, 1   // Saltado
label_cbnz:

    HLT 0
