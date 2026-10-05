li x1, 512
li x2, 123
li x3, 456
stp x2, x3, 4(x1)
ld x4, [x1, 4]
beq x2, x4, 2
j fail
j fail
ld x4, [x1, 8]
beq x3, x4, 2
j fail
j fail
stp x3, x2, 2040(x1)
ld x4, [x1, 16376]
beq x3, x4, 2
j fail
j fail
ld x4, [x1, 16380]
beq x2, x4, 2
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
