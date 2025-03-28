.text
    // --- BEQ: se salta si X1 == X2 ---
    movz    X1, 100
    movz    X2, 100
    cmp     X1, X2
    beq     label_beq   // condición verdadera, se salta la siguiente instrucción
    adds    X3, X0, 1   // no se ejecuta
label_beq:
    adds    X3, X0, 2   // se ejecuta: X3 = (resultado esperado)

    // --- BNE: se salta si X1 != X2 ---
    movz    X1, 50
    movz    X2, 60
    cmp     X1, X2
    bne     label_bne   // condición verdadera
    adds    X4, X0, 1   // no se ejecuta
label_bne:
    adds    X4, X0, 2   // se ejecuta

    // --- BGT: se salta si X1 > X2 ---
    movz    X1, 70
    movz    X2, 60
    cmp     X1, X2
    bgt     label_bgt   // se salta
    adds    X5, X0, 1   // no se ejecuta
label_bgt:
    adds    X5, X0, 2   // se ejecuta

    // --- BLT: se salta si X1 < X2 ---
    movz    X1, 40
    movz    X2, 50
    cmp     X1, X2
    blt     label_blt   // se salta
    adds    X6, X0, 1   // no se ejecuta
label_blt:
    adds    X6, X0, 2   // se ejecuta

    // --- BGE: se salta si X1 >= X2 ---
    movz    X1, 55
    movz    X2, 55
    cmp     X1, X2
    bge     label_bge   // se salta
    adds    X7, X0, 1   // no se ejecuta
label_bge:
    adds    X7, X0, 2   // se ejecuta

    // --- BLE: se salta si X1 <= X2 ---
    movz    X1, 30
    movz    X2, 40
    cmp     X1, X2
    ble     label_ble   // se salta
    adds    X8, X0, 1   // no se ejecuta
label_ble:
    adds    X8, X0, 2   // se ejecuta

    // --- B Incondicional: salto hacia adelante ---
    b       label_forward
    adds    X9, X0, 1   // no se ejecuta
label_forward:
    adds    X9, X0, 2   // se ejecuta

    HLT     0
