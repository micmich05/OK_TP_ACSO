.text
movz   X1, 7
movz   X2, 3
adds   X3, X1, X2
subs   X4, X3, X2
ands   X5, X3, X4
eor    X6, X5, X1
orr    X7, X6, X4
mul    X8, X7, X2
cmp    X8, X8
beq    end1
adds   X9, X0, 1
end1:
HLT    0
