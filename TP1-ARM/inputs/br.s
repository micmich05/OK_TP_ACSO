.text
    adr   X1, target      // cargar en X1 la dirección de “target”
    br    X1             // saltar a la dirección en X1
    adds  X2, X0, 1      // instrucción que debe saltarse

target:
    HLT   0
