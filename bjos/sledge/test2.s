STMDB SP!, { LR }
LDMIA SP!, { LR }
cmp r4,#123 
loop:
bne loop
