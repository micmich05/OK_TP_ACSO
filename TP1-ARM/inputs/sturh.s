.text
    movz   X2, 0
    movz   X1, 0xABCD
    sturh  W1, [X2, #0x10]
    ldurh  W3, [X2, #0x10]
    HLT    0
