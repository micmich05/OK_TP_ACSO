.text
movz   X1, 0
cbz    X1, L1
adds   X2, X0, 1

L1:
movz   X3, 5
movz   X4, 10
cmp    X3, X4
blt    L2
adds   X5, X0, 2

L2:
cmp    X3, X3
beq    L3
adds   X6, X0, 3

L3:
movz   X7, 10
movz   X8, 5
cmp    X7, X8
bgt    L4
adds   X9, X0, 4

L4:
movz   X10, 7
movz   X11, 7
cmp    X10, X11
bge    L5
adds   X12, X0, 5

L5:
movz   X13, 3
movz   X14, 5
cmp    X13, X14
ble    L6
adds   X15, X0, 6

L6:
movz   X17, 1
cbnz   X17, L8
adds   X18, X0, 8

L8:
movz   X19, 2
movz   X20, 3
cmp    X19, X20
bne    L9
adds   X21, X0, 9

L9:
b      L7
adds   X22, X0, 10

L7:
HLT    0
