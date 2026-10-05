li x1, 512
li x2, 1234
st x2, [x1, 4]
ld x3, [x1, 4]
beq x2, x3, 2
j fail
j fail
li x2, 5678
st x2, [x1, 16380]
ld x3, [x1, 16380]
beq x2, x3, 2
j fail
j fail
li x0, 0
li x7, 93
sysc 0
fail:
li x0, 1
li x7, 93
sysc 0
j fail
