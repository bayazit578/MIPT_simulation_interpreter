li x0, 0
li x1, 1
li x2, 2
li x3, 31
li x4, 32
li x5, 8
li x7, 512
li x8, 640
st x1, [x8, 20]
st x1, [x8, 32]
st x1, [x8, 36]
st x1, [x8, 60]
st x1, [x8, 76]
st x1, [x8, 80]
st x1, [x8, 84]
st x1, [x8, 92]
st x1, [x8, 96]
st x1, [x8, 104]
st x1, [x8, 108]
st x1, [x8, 116]
st x1, [x8, 120]
st x1, [x8, 124]
li x9, 512
li x10, 32
init_crc:
st x1, [x9, 0]
addi x9, x9, 4
addi x10, x10, 65535
beq x10, x0, 1
j init_crc
next_bit:
ld x6, [x7, 0]
li x11, 512
li x12, 640
li x13, 0
shift_crc:
ld x14, [x11, 4]
ld x15, [x12, 0]
beq x6, x0, 1
add x14, x15, x14
bne x14, x2, 1
addi x14, x14, 65534
st x14, [x11, 0]
addi x11, x11, 4
addi x12, x12, 4
addi x13, x13, 1
beq x13, x3, 1
j shift_crc
st x6, [x11, 0]
addi x5, x5, 65535
beq x5, x0, 1
j next_bit
li x16, 636
li x17, 32
li x18, 0
rebuild_crc:
add x18, x18, x18
ld x19, [x16, 0]
bne x19, x0, 1
addi x18, x18, 1
addi x16, x16, 65532
addi x17, x17, 65535
beq x17, x0, 1
j rebuild_crc
li x20, 53763
li x21, 16
expected_high:
add x20, x20, x20
addi x21, x21, 65535
beq x21, x0, 1
j expected_high
addi x20, x20, 61325
beq x18, x20, 1
j fail
li x7, 93
sysc 0
fail:
li x0, 1
li x7, 93
sysc 0
