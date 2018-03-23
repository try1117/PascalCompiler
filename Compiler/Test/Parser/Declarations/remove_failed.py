from os import listdir
import re
import subprocess
import os

r = re.compile('.+failed\.out')
for i in listdir('./'):
    name = r.match(i)
    if name:
        print('Removing ' + i)
        os.remove(i)
