from os import listdir
import re
import subprocess
import os

r = re.compile('(?P<name>.+)\.in')
for i in listdir('./'):
    name = r.match(i)
    if name:
        subprocess.call([r'C:\Users\danilov\Desktop\5_semester\COMPILER\PascalCompiler\Compiler\Debug\Compiler.exe', '-l', i])
        f1, f2 = open('{}.out'.format(name.group('name')), 'w'), open('output.txt')
        f1.write(f2.read())
        f1.close()
        f2.close()

os.remove('output.txt')
