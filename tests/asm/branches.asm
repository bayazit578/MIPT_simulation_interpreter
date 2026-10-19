li x0, 0
li x1, 1024
li x5, 8192
outer:
li x2, 8192
li x3, 0
li x4, 0
loop:
beq x3, x0, 1
j   odd
li  x3, 1
j   hop3
odd:
li x3, 0
j  hop1
hop1:
j hop3
hop3:
beq x3, x0, 1
j   hop2
j   hop2
hop2:
beq x3, x0, 1
j   hop4
j   hop4
hop4:
addi x4, x4, 1
addi x2, x2, 65535
beq  x2, x0, 1
j    loop
beq  x4, x5, 1
j    fail
beq  x3, x0, 1
j    fail
addi x1, x1, 65535
beq  x1, x0, 1
j    outer
li   x7, 93
sysc 0
fail:
li   x0, 1
li   x7, 93
sysc 0
