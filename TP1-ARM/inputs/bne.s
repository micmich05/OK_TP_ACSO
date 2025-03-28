.text
    movz  X1, 5
    movz  X2, 10
    cmp   X1, X2
    bne   target
    adds  X3, X0, 1    // no debe ejecutarse

target:
    HLT   0
