.text
    movz  X1, 5
    movz  X2, 5
    cmp   X1, X2
    bge   target
    adds  X3, X0, 1    // no debe ejecutarse

target:
    HLT   0
