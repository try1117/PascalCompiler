include c:\masm32\include\masm32rt.inc
.xmm
.const
.code
start:
push ebp
mov ebp, esp
sub esp, 8
push 0
pop dword ptr [ebp - 4]
push 10
pop dword ptr [ebp - 8]
jmp $WHILE_COND0@
$WHILE_BODY1@:
push dword ptr [ebp - 4]
push 1
pop ebx
pop eax
add eax, ebx
push eax
pop eax
printf("%d\n", eax)
push dword ptr [ebp - 4]
push 1
pop ebx
pop eax
add eax, ebx
push eax
pop dword ptr [ebp - 4]
$WHILE_COND0@:
push dword ptr [ebp - 4]
push dword ptr [ebp - 8]
pop ebx
cmp dword ptr [esp - 0], ebx
setge al
sub al, 1
movsx eax, al
mov dword ptr [esp - 0], eax
pop eax
test eax, eax
jnz $WHILE_BODY1@
$WHILE_END2@:
mov esp, ebp
pop ebp
exit
end start
