
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

add.l d0, d1
add.l d2, d3
and.l d0, d1
or.l d0, d1
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

jsr (a0)

addq.l #4, sp
addq.l #8, sp

add.l #0xdeadbeef, d0
move.b (d0), d0
move.b d3, (d0)
move.b d3, (d1)
move.b d3, (d2)

rts
