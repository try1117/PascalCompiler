from os import listdir
import re
import subprocess
import os

r = re.compile('(?P<name>.+)\.in')
for i in listdir('./'):
    name = r.match(i)
    if name:
        subprocess.call([r'C:\Users\danilov\Desktop\5_semester\COMPILER\PascalCompiler\Compiler\Debug\Compiler.exe', '-g', i])
        f1, f2 = open('{}.tree'.format(name.group('name')), 'w'), open('syntax_tree.txt')
        f3, f4 = open('{}.asm'.format(name.group('name')), 'w'), open('asm_code.txt')

        f1.write(f2.read())
        f3.write(f4.read())
        f1.close(), f2.close()
        f3.close(), f4.close()

os.remove('syntax_tree.txt')
os.remove('asm_code.txt')
