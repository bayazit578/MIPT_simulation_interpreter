li x1, 7
addi x2, x1, 5
li x3, 12
beq x2, x3, 2
j fail
j fail
addi x2, x1, 65535
li x3, 6
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
