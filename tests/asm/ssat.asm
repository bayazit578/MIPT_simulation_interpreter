li x1, 300
ssat x1, x1, 8
li x2, 127
beq x1, x2, 2
j fail
j fail
li x1, 65236
ssat x1, x1, 8
li x2, 65408
beq x1, x2, 2
j fail
j fail
li x1, 42
ssat x1, x1, 8
li x2, 42
beq x1, x2, 2
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
