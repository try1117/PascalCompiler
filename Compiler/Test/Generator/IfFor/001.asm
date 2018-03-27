include c:\masm32\include\masm32rt.inc
.xmm
.const
.code
start:
push ebp
mov ebp, esp
sub esp, 8
push 123
pop dword ptr [ebp - 4]
push dword ptr [ebp - 4]
push 123
pop ebx
cmp dword ptr [esp - 0], ebx
setne al
sub al, 1
movsx eax, al
mov dword ptr [esp - 0], eax
pop eax
test eax, eax
jz $IFFAIL0@
push 1
pop eax
printf("%d\n", eax)
jmp $IFEND1@
$IFFAIL0@:
push 0
pop eax
printf("%d\n", eax)
$IFEND1@:
push dword ptr [ebp - 4]
push 123
pop ebx
cmp dword ptr [esp - 0], ebx
sete al
sub al, 1
movsx eax, al
mov dword ptr [esp - 0], eax
pop eax
test eax, eax
jz $IFFAIL2@
push dword ptr [ebp - 4]
pop eax
printf("%d\n", eax)
jmp $IFEND3@
$IFFAIL2@:
push 0
pop eax
printf("%d\n", eax)
$IFEND3@:
push 200
pop dword ptr [ebp - 8]
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
jz $IFFAIL4@
push -1
pop eax
printf("%d\n", eax)
jmp $IFEND5@
$IFFAIL4@:
push 1
pop eax
printf("%d\n", eax)
$IFEND5@:
push dword ptr [ebp - 4]
push dword ptr [ebp - 8]
pop ebx
cmp dword ptr [esp - 0], ebx
setle al
sub al, 1
movsx eax, al
mov dword ptr [esp - 0], eax
pop eax
test eax, eax
jz $IFFAIL6@
push 1
pop eax
printf("%d\n", eax)
jmp $IFEND7@
$IFFAIL6@:
push -1
pop eax
printf("%d\n", eax)
$IFEND7@:
mov esp, ebp
pop ebp
exit
end start
