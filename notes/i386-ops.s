
Lloop:

mov $0xdeadbeef, %eax
mov $0xdeadbeef, %edi
mov $0xdeadbeef, %esi
mov $0xdeadbeef, %edx
mov $0xdeadbeef, %ecx
mov $0xdeadbeef, %ebx
mov %eax, %edi
mov %eax, %esi
mov %eax, %edx
mov %eax, %ecx
mov %eax, %ebx

mov %ebx, %eax
mov %ebx, %edi
mov %ebx, %esi
mov %ebx, %edx
mov %ebx, %ecx
mov %ebx, %ebx

cmovs %eax, %eax
cmovs %eax, %edi
cmovs %eax, %esi
cmovs %eax, %edx
cmovs %eax, %ecx
cmovs %eax, %ebx
cmovs %edi, %edi
cmovs %edi, %esi
cmovs %esi, %edx
cmovs %esi, %esi

cmove %eax, %eax
cmove %eax, %ecx
cmove %eax, %edx
cmove %ecx, %eax
cmove %edx, %eax
cmovne %eax, %eax
cmovne %eax, %ecx
cmovne %eax, %edx
cmovne %ecx, %eax
cmovne %edx, %eax

mov %esi, %edi
mov (%esi), %edi
mov (%esi), %esi
mov (%edi), %edi
add %esi, %edi
add %edi, %esi
add %edi, %eax
add %esi, %eax

push %edi
push %esi
call *%eax
add $8, %esp
ret

movb (%eax), %dl
movb (%ecx), %dl
movb (%edx), %dl
and $0xff, %eax
and $0xff, %ecx
and $0xff, %edx

movb %dl, (%eax)
movb %dl, (%ecx)
movb %dl, (%edx)

add $0xdeadbeef, %eax
add $0xdeadbeef, %ecx
add $0xdeadbeef, %edx
sub $0xdeadbeef, %eax
sub $0xdeadbeef, %ecx
sub $0xdeadbeef, %edx

sub %eax, %eax
sub %ecx, %eax
sub %edx, %eax

sub %eax, %eax
sub %eax, %ecx
sub %eax, %edx

and %eax, %eax
and %ecx, %eax
and %edx, %eax

and %eax, %eax
and %eax, %ecx
and %eax, %edx

or %eax, %eax
or %eax, %ecx
or %eax, %edx

xor %eax, %eax
xor %eax, %ecx
xor %eax, %edx

not %eax
not %ecx
not %edx

imul %eax, %eax
imul %eax, %ecx
imul %eax, %edx

imul %ecx, %eax
imul %edx, %eax
imul %ebx, %eax

idiv %eax
idiv %ecx
idiv %edx

call *%eax
call *%ecx
call *%edx

cmp $0xdeadcafe, %eax
cmp $0xdeadcafe, %ecx
cmp $0xdeadcafe, %edx
cmp $0xdeadcafe, %ebx

cmp %eax, %eax
cmp %eax, %ecx
cmp %eax, %edx
cmp %eax, %ebx

cmp %eax, %eax
cmp %ecx, %eax
cmp %edx, %eax

jmp Lloop
je Lloop
je Lloop
jne Lloop
jne Lloop
js Lloop
js Lloop

pop %eax
pop %edx
pop %ecx
pop %ebx
pop %esp

shr %cl, %eax
shr %cl, %edx
shl %cl, %ecx
shl %cl, %ebx

mov -4(%esp), %eax
mov -8(%esp), %eax
mov -12(%esp), %eax
mov 4(%esp), %ecx
mov 8(%esp), %ecx
mov 12(%esp), %ecx

mov %eax, 4(%esp)
mov %ecx, 8(%esp)
mov %edx, 12(%esp)

add $0xdeadbeef, %esp
add $8, %esp
add $12, %esp

