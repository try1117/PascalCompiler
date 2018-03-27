include c:\masm32\include\masm32rt.inc
.xmm
.const
.code
start:
push ebp
mov ebp, esp
sub esp, 4
push 100
pop dword ptr [ebp - 4]
push dword ptr [ebp - 4]
push 25
pop ebx
pop eax
cdq 
idiv ebx
push eax
pop eax
printf("%d\n", eax)
mov esp, ebp
pop ebp
exit
end start
