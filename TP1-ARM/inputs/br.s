.text
    ADD X0, X0, 0X4
    LSL X0, X0, 20
    ADD X0, X0, 20
    BR X0
    B label1
    HLT 0

label1:
    ADD X1, X2, 10                 
    BR X0                           
    ADD X2,X3,1
    ADD X3,X4,1
    ADD X4,X5,1
    ADD X5,X6,1
    HLT 0