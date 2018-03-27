include c:\masm32\include\masm32rt.inc
.xmm
.const
.code
start:
push ebp
mov ebp, esp
sub esp, 8
push 10
push 1
pop dword ptr [ebp - 4]
dec dword ptr [ebp - 4]
jmp $FOR_COND0@
$FOR_BODY1@:
push dword ptr [ebp - 4]
pop eax
printf("%d\n", eax)
$FOR_COND0@:
inc dword ptr [ebp - 4]
mov eax, dword ptr [esp - 0]
cmp dword ptr [ebp - 4], eax
jle $FOR_BODY1@
$FOR_END2@:
add esp, 4
push 11111111
pop eax
printf("%d\n", eax)
push 4
push 1
pop dword ptr [ebp - 4]
dec dword ptr [ebp - 4]
jmp $FOR_COND3@
$FOR_BODY4@:
push dword ptr [ebp - 4]
push 1
pop dword ptr [ebp - 8]
dec dword ptr [ebp - 8]
jmp $FOR_COND6@
$FOR_BODY7@:
push dword ptr [ebp - 4]
push dword ptr [ebp - 8]
pop ebx
pop eax
imul eax, ebx
push eax
pop eax
printf("%d\n", eax)
$FOR_COND6@:
inc dword ptr [ebp - 8]
mov eax, dword ptr [esp - 0]
cmp dword ptr [ebp - 8], eax
jle $FOR_BODY7@
$FOR_END8@:
add esp, 4
$FOR_COND3@:
inc dword ptr [ebp - 4]
mov eax, dword ptr [esp - 0]
cmp dword ptr [ebp - 4], eax
jle $FOR_BODY4@
$FOR_END5@:
add esp, 4
mov esp, ebp
pop ebp
exit
end start
