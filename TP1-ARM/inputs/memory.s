.text
movz   X1, 0
movz   X2, 0xABCD
stur   X2, [X1, 0x10]
ldur   X3, [X1, 0x10]
sturb  W4, [X1, 0x20]
ldurb  W5, [X1, 0x20]
sturh  W6, [X1, 0x30]
ldurh  W7, [X1, 0x30]
lsl    X8, X3, 2
lsr    X9, X3, 1
cmp    X8, X9
blt    skip2
adds   X10, X0, 1

skip2:
cbnz   X5, end2
adds   X11, X0, 2

end2:
HLT    0