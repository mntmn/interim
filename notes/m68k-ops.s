move.l #0xdeadbeef, d0
move.l #0xdeadbeef, d1
move.l #0xdeadbeef, d2
move.l #0xdeadbeef, d3
move.l #0xdeadbeef, d4
move.l #0xdeadbeef, d5
move.l #0xdeadbeef, a0

move.l d0, a0
move.l (a0), d0

move.l d1, a0
move.l (a0), d1

move.l d0, d1
move.l d0, d2
move.l d0, d3

move.l d1, d1
move.l d1, d2
move.l d1, d3

move.l d2, d1
move.l d2, d2
move.l d2, d3

muls.l d0, d0
muls.l d0, d1
muls.l d0, d2

muls.l d1, d0
muls.l d2, d0
muls.l d3, d0

divs.l d0, d0
divs.l d0, d1
divs.l d0, d2

add.l d0, d1
add.l d2, d3
and.l d0, d1
or.l d0, d1
not.l d0
not.l d1
not.l d2
eor.l d0, d0
eor.l d0, d1
eor.l d0, d2
eor.l d1, d0
lsr.l d0, d0
lsr.l d0, d1
lsr.l d0, d2
lsr.l d1, d0
lsr.l d1, d2
lsl.l d0, d1

sub.l d0, d1
sub.l d1, d0
sub.l d2, d3

move.l d0, -(sp)
move.l d1, -(sp)
move.l d2, -(sp)
move.l (sp)+, d0
move.l (sp)+, d1
move.l (sp)+, d2

move.l d0, (4, sp)
move.l d1, (8, sp)
move.l d2, (16, sp)
move.l (4, sp), d0
move.l (8, sp), d1
move.l (16, sp), d2

cmp.l d0, d0
cmp.l d0, d1
cmp.l d0, d2
cmp.l d0, d0
cmp.l d1, d0
cmp.l d2, d0

cmp.l #0xdeadbeef, d0
cmp.l #0xdeadbeef, d1

jsr (a0)

addq.l #4, sp
addq.l #8, sp

add.l #33, sp
sub.l #33, sp

add.l #0xdeadbeef, d0
add.l #0xdeadbeef, d1
add.l #0xdeadbeef, d2
move.b (d0), d3
move.b (d1), d3
move.b (d2), d3
move.b d3, (d0)
move.b d3, (d1)
move.b d3, (d2)

loop:
beq loop
beq loop
bne loop
bne loop
bmi loop
bpl loop
bra loop

beq skip
mov.l d0, d1
skip:

rts
