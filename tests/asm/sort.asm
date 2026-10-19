li x0, 0
li x1, 512
li x2, 9
st x2, [x1, 0]
li x2, 3
st x2, [x1, 4]
li x2, 7
st x2, [x1, 8]
li x2, 1
st x2, [x1, 12]
li x2, 5
st x2, [x1, 16]
li x3, 4
next_pass:
addi x4, x3, 0
li x5, 512
compare_pair:
ld x6, [x5, 0]
ld x7, [x5, 4]
selc x8, x6, x7
beq x8, x7, 2
st x7, [x5, 0]
st x6, [x5, 4]
addi x5, x5, 4
addi x4, x4, 65535
beq x4, x0, 1
j compare_pair
addi x3, x3, 65535
beq x3, x0, 1
j next_pass
li x1, 512
ld x6, [x1, 0]
li x7, 1
beq x6, x7, 1
j fail
ld x6, [x1, 4]
li x7, 3
beq x6, x7, 1
j fail
ld x6, [x1, 8]
li x7, 5
beq x6, x7, 1
j fail
ld x6, [x1, 12]
li x7, 7
beq x6, x7, 1
j fail
ld x6, [x1, 16]
li x7, 9
beq x6, x7, 1
j fail
li x7, 93
sysc 0
fail:
li x0, 1
li x7, 93
sysc 0
