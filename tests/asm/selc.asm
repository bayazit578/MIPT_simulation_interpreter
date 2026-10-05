li x1, 9
li x2, 3
selc x3, x1, x2
beq x3, x1, 2
j fail
j fail
selc x3, x2, x1
beq x3, x1, 2
j fail
j fail
selc x3, x2, x2
beq x3, x2, 2
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
