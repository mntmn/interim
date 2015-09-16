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
