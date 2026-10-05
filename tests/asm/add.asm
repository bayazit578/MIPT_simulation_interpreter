li x1, 7
li x2, 5
add x1, x2, x3
li x4, 12
beq x3, x4, 2
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
