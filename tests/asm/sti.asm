li x1, 512
li x2, 123
sti x2, x1, 4
li x3, 516
beq x1, x3, 2
j fail
j fail
ld x4, [x1, 0]
beq x2, x4, 2
j fail
j fail
sti x2, x1, 16380
li x3, 512
beq x1, x3, 2
j fail
j fail
ld x4, [x1, 0]
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
