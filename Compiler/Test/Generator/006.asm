include c:\masm32\include\masm32rt.inc
.xmm
.const
.code
start:
push ebp
mov ebp, esp
sub esp, 8
push 10
pop dword ptr [ebp - 4]
push 4
pop dword ptr [ebp - 8]
push 5
push dword ptr [ebp - 4]
pop ebx
pop eax
imul eax, ebx
push eax
push dword ptr [ebp - 8]
push dword ptr [ebp - 4]
pop ebx
pop eax
imul eax, ebx
push eax
push dword ptr [ebp - 8]
pop ebx
pop eax
imul eax, ebx
push eax
pop ebx
pop eax
add eax, ebx
push eax
pop eax
printf("%d\n", eax)
mov esp, ebp
pop ebp
exit
end start
