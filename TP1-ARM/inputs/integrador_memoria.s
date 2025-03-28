.text
    // --- Pruebas de memoria ---
    // Usamos X2 como base (corresponderá a 0x10000000 en el simulador).
    movz   X2, 0

    // STUR: Almacena el contenido de X1 en M[X2 + 0x10].
    movz   X1, 0x1234      // Valor de 16 bits (se extiende a 64 bits)
    stur   X1, [X2, #0x10]
    ldur   X3, [X2, #0x10]  // X3 debe ser igual a X1

    // STURB: Almacena los 8 bits bajos de X1 en M[X2 + 0x20].
    sturb  X1, [X2, #0x20]
    ldurb W4, [X2, #0x20]  // W4 contendrá los 8 bits bajos de X1

    // STURH: Almacena los 16 bits bajos de X1 en M[X2 + 0x30].
    sturh  W1, [X2, #0x30]
    ldurh W5, [X2, #0x30]  // W5 contendrá los 16 bits bajos de X1 (con ceros en los 48 MSB)

    // --- Pruebas de control de flujo con CBZ y CBNZ ---
    // CBZ: Si X3 es 0, salta.
    movz   X1, 0
    cbz    X1, lab_cbz     // Se salta la siguiente instrucción
    adds   X6, X0, 1      // Esta instrucción se omite
lab_cbz:

    // CBNZ: Si X1 no es 0, salta.
    movz   X1, 5
    cbnz   X1, lab_cbnz   // Salta
    adds   X7, X0, 1      // Se omite
lab_cbnz:

    // --- Pruebas de ramas incondicionales ---
    // B: Salto relativo hacia adelante.
    b lab_forward
    adds   X8, X0, 1     // Saltado
lab_forward:
    adds   X9, X0, 2     // Ejecutada

    // B: Salto relativo hacia atrás.
    b lab_backward
    adds   X10, X0, 1    // Saltado
lab_backward:
    adds   X11, X0, 3    // Ejecutada

    // BR: Salto absoluto mediante un registro.
    // Se prepara la dirección absoluta para saltar a 'br_target'.
    // Se asume que 'br_target' está en la dirección 0x10000040.
    movz   X1, 0x10000040
    br     X1           // Salta a 'br_target'
    adds   X12, X0, 1   // Saltado

br_target:
    HLT 0
