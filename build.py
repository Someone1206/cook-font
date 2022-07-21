import os
import platform

out_dir = 'build'

if not os.path.exists(out_dir):
    os.mkdir(out_dir)

os.chdir(out_dir)

os.system('cmake ..')

if platform.system() == 'Linux' or platform.system() == 'Darwin':
    os.system('make')
else:
    print('Platform is Windows.')
    print('Did not find make. Execute vuild manually')