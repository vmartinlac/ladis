import os
import re
import shutil
import sys

def get_target_path(to_path):
    content = os.listdir(to_path)
    ids = os.listdir(to_path)
    ids = [ re.match('result_([0-9]+)', item) for item in ids ]
    ids = [ int(m.group(1)) for m in ids if m ]
    next_id = max(ids) + 1
    filename = 'result_{}'.format(next_id)
    return os.path.join(to_path, filename)

from_path = sys.argv[1]
to_path = sys.argv[2]

for f in os.listdir(from_path):
    if re.match('result_[0-9]+', f):
        myfrom = os.path.join(from_path, f)
        myto = get_target_path(to_path)
        print('{} => {}'.format(myfrom, myto))
        shutil.move(myfrom, myto)

