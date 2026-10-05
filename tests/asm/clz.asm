li x1, 0
clz x2, x1
li x3, 32
beq x2, x3, 2
j fail
j fail
li x1, 1
clz x2, x1
li x3, 31
beq x2, x3, 2
j fail
j fail
li x1, 65535
clz x2, x1
li x3, 0
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
