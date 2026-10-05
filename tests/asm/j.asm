li x1, 0
j forward
back:
li x1, 7
j checked
forward:
j back
j fail
checked:
li x2, 7
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
