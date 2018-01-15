from os import listdir
import re
import subprocess
import os

r = re.compile('(?P<name>.+)\.in')
for i in listdir('./'):
    name = r.match(i)
    if name:
        subprocess.call([r'C:\Users\danilov\Desktop\5_semester\COMPILER\PascalCompiler\Compiler\Debug\Compiler.exe', '-exp', i])
        f1, f2 = open('{}.out'.format(name.group('name'))), open('output.txt')
        a, b = f1.read(), f2.read();
        if a != b:
            print('Test "{}" failed'.format(i))
            print(b, file=open('{}_failed.out'.format(name.group('name')), 'w'), end='')
        else:
            print('Test "{}" passed'.format(i))
        f1.close()
        f2.close()

os.remove('output.txt')
