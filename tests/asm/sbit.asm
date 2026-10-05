sbit x1, x8, 0
li x2, 1
beq x1, x2, 2
j fail
j fail
sbit x1, x8, 5
li x2, 32
beq x1, x2, 2
j fail
j fail
sbit x1, x8, 31
clz x2, x1
beq x2, x8, 2
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
