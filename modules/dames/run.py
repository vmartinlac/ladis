
import os
import random
import shutil
import subprocess

min_depth = 6
max_depth = 6
opponent_skill_min = 0
opponent_skill_max = 0
log_filename = 'log.txt'

while True:
    depth = random.randint(min_depth, max_depth)
    opponent_skill = random.randint(opponent_skill_min, opponent_skill_max)

    log_file = open(log_filename, 'w')

    print("New tournament with opponent_skill={} and minimax_depth={}.".format(opponent_skill, depth))
    subprocess.run(['LADIS_dames', str(depth), str(opponent_skill)], stdout=log_file)

    log_file.close()

    content = os.listdir('.')
    count = 0
    dirname = "result_{}".format(count)
    while dirname in content:
        count += 1
        dirname = "result_{}".format(count)
    os.mkdir(dirname)
    shutil.move(log_filename, dirname)
    for f in content:
        if f.startswith("screen_"):
            shutil.move(f, dirname)

