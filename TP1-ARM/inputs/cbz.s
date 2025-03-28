.text
    movz   X3, 0
    cbz    X3, target
    adds   X4, X0, #1
target:
    HLT    0
