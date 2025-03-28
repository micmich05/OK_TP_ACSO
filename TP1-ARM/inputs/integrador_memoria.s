.text
    // Se asume que X2 se usará como base (corresponde a 0x10000000 en el simulador)
    movz    X2, 0x1000

    // --- STUR y LDUR ---
    // Almacena en M[X2 + 0x10] el valor de X1.
    movz    X1, 0x1234       // valor a almacenar
    stur    X1, [X2, #0x10]
    ldur    X3, [X2, #0x10]   // X3 debe tener 0x1234

    // --- STURB y LDURB ---
    // Almacena 8 bits (byte) de X1 en M[X2 + 0x20].
    movz    X1, 0xFF         // valor: 0xFF
    sturb   w1, [X2, #32]     // usa w1 en lugar de X1 y offset decimal correspondiente
    ldurb   w4, [X2, #32]     // w4 contendrá 0xFF (zero-extended a 64 bits)

    // --- STURH y LDURH ---
    // Almacena 16 bits de X1 en M[X2 + 0x30].
    movz    X1, 0xABCD
    sturh   w1, [X2, #48]     // usa w1 y offset decimal correspondiente
    ldurh   w5, [X2, #48]     // w5 contendrá 0xABCD

    // --- Reflejo de resultados mediante aritmética ---
    // Suma de los valores cargados: X6 = X3 + X4
    add     X6, X3, X4      // X6 = 0x1234 + 0xFF

    // Suma final: X7 = X6 + X5
    add     X7, X6, X5      // X7 = (0x1234 + 0xFF) + 0xABCD

    hlt     0