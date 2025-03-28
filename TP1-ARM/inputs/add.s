.text
    // Test 1: ADD immediate (shift==00)
    // X1 = 10; se suma 3 → X0 debe ser 13.
    movz  X1, 10
    add   X0, X1, 3

    // Test 2: ADD extended register
    // X1 = 2, X2 = 7 → X0 debe ser 9.
    movz  X1, 2
    movz  X2, 7
    add   X0, X1, X2

    // Test 3: ADD immediate (shift==01)
    // X1 = 100; inmediato = 1 se interpreta como (1 << 12) = 4096,
    // por lo que X0 = 100 + 4096 = 4196.
    movz  X1, 100
    add   X0, X1, 1

    HLT   0
