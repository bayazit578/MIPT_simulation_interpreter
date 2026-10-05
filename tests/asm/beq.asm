li x1, 7
li x2, 7
beq x1, x2, 2
j fail
j fail
li x2, 9
li x3, 0
beq x1, x2, 2
li x3, 1
j checked
j fail
checked:
li x4, 1
beq x3, x4, 2
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
