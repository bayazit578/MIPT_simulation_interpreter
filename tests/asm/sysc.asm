li x7, 93
li x0, 0
sysc 0
li x0, 1
sysc 0
j fail
li x0, 0
li x7, 93
sysc 0
fail:
li x0, 1
li x7, 93
sysc 0
j fail
