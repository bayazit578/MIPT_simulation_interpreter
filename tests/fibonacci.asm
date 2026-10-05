li x1, 0
li x2, 1
add x2, x1, x3
addi x1, x2, 0
addi x2, x3, 0
li x4, 1
beq x2, x4, 2
j fail
j fail
add x2, x1, x3
addi x1, x2, 0
addi x2, x3, 0
li x4, 2
beq x2, x4, 2
j fail
j fail
add x2, x1, x3
addi x1, x2, 0
addi x2, x3, 0
li x4, 3
beq x2, x4, 2
j fail
j fail
add x2, x1, x3
addi x1, x2, 0
addi x2, x3, 0
li x4, 5
beq x2, x4, 2
j fail
j fail
add x2, x1, x3
addi x1, x2, 0
addi x2, x3, 0
li x4, 8
beq x2, x4, 2
j fail
j fail
add x2, x1, x3
addi x1, x2, 0
addi x2, x3, 0
li x4, 13
beq x2, x4, 2
j fail
j fail
add x2, x1, x3
addi x1, x2, 0
addi x2, x3, 0
li x4, 21
beq x2, x4, 2
j fail
j fail
add x2, x1, x3
addi x1, x2, 0
addi x2, x3, 0
li x4, 34
beq x2, x4, 2
j fail
j fail
add x2, x1, x3
addi x1, x2, 0
addi x2, x3, 0
li x4, 55
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
