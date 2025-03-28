.text
    movz  X1, 10
    movz  X2, 5
    cmp   X1, X2
    bgt   target
    adds  X3, X0, 1    // no debe ejecutarse

target:
    HLT   0
