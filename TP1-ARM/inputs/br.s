.text
    // Asumimos que el código se carga en 0x10000000.
    // Queremos que 'target' esté en 0x10000020 (32 bytes después del inicio).
    movz  X1, 0x10000020    // Carga en X1 la dirección absoluta de 'target'
    br    X1               // Salta a 'target'
    adds  X2, X0, #1       // Estas instrucciones se deben saltar
    adds  X3, X0, #2
    adds  X4, X0, #3
    adds  X5, X0, #4
    adds  X6, X0, #5
    adds  X7, X0, #6

target:
    HLT   0

