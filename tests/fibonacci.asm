li x7, 65
sysc 0
addi x1, x0, 0
li x2, 0
li x3, 1
li x4, 0
loop:
bne x1, x4, 1
j output
add x5, x2, x3
addi x2, x3, 0
addi x3, x5, 0
addi x1, x1, 65535
j loop
output:
addi x0, x2, 0
li x7, 66
sysc 0
li x0, 0
li x7, 93
sysc 0
