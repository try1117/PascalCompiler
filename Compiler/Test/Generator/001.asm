include c:\masm32\include\masm32rt.inc
.xmm
.const
.code
start:
push ebp
mov ebp, esp
sub esp, 0
push 5844
mov eax, esp
printf("%d\n", dword ptr [eax - 0])
add esp, 4
mov esp, ebp
pop ebp
exit
end start
