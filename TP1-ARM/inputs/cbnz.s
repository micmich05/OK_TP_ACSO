.text
    movz   X3, 1
    cbnz   X3, done
    adds   X4, X0, #1
done:
    HLT    0

