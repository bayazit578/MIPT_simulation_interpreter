li x1, 12345
addi x2, x8, 12345
beq x1, x2, 2
j fail
j fail
li x1, 65535
li x2, 1
add x1, x2, x3
beq x3, x8, 2
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
