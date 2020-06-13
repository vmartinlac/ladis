
import os
import random
import shutil
import subprocess

min_depth = 2
max_depth = 9

while True:
    depth = random.randint(min_depth, max_depth)
    opponent_skill = random.randint(0, 5)

    subprocess.run(['LADIS_dames', str(depth), str(opponent_skill)])

    content = os.listdir('.')
    count = 0
    dirname = "result_{}".format(count)
    while dirname in content:
        count += 1
        dirname = "result_{}".format(count)
    os.mkdir(dirname)
    for f in content:
        if f.startswith("screen_"):
            shutil.move(f, dirname)

