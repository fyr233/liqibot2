'''
useage:
    SelectSetu.py"
output:
    Setu/xxxxxxx.jpg

'''

import os
import random
import time

imgfolderdir = '../../miraiOK_Release/data/MiraiApiHttp/images/Setu/'

localtime = time.localtime(time.time())
if localtime[3] < 4 or localtime[3] > 21:
    filenames = os.listdir(imgfolderdir)
    index = random.randint(0, len(filenames)-1)
    print('Setu/' + filenames[index])
