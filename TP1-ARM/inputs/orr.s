MOV X0, #0b10101010         // X0 = 0xAA
MOV X1, #0b01010101         // X1 = 0x55
ORR X2, X0, X1              // X2 = X0 | X1 = 0xFF
// Resultado esperado: X2 = 0xFF

// Caso 2: ORR con un registro en cero
MOV X0, #0x0                // X0 = 0x00
MOV X1, #0xDEADBEEF         // X1 = valor cualquiera
ORR X2, X0, X1              // X2 = X1
// Resultado esperado: X2 = 0xDEADBEEF

MOV X0, #0x12345678
ORR X1, X0, X0              // X1 = X0

HLT 0