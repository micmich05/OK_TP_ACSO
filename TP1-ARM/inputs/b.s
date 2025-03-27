.text
    adds X2, X0, 10
    b foo
    

bar:
    HLT 0

foo:
    b bar
    adds X3, X0, 10

    HLT 0
