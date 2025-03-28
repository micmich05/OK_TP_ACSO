.text
    add    X1, X0, #32    // prepara X1 para saltar al label
    movz   X2, 4
    add    X3, X2, X2     // ADD (extended register)
    add    X4, X0, #3     // ADD (immediate)
    subs   X5, X4, #1     // SUBS (immediate, flags update)
    cmp    X5, #5         // CMP (immediate, flags update)
    br     X1             // BR salto incondicional usando registro
    adds   X6, X0, #1     // instrucción que debe saltarse
    
label:
    HLT    0
