set arg1=%1
\masm32\bin\ml /c /coff "%arg1%.asm"
/masm32/bin/link /subsystem:console "%arg1%.obj"
del "%arg1%.obj"
