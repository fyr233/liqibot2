'''
usage:
    SelectSetu.py
output:
    Setu/xxxxxxx.jpg

'''

import os
import random
import time

imgfolderdir = '../../mcl-1.0.5/data/net.mamoe.mirai-api-http/images/Setu/'

localtime = time.localtime(time.time())
if localtime[3] < 6 or localtime[3] > 22:
    filenames = os.listdir(imgfolderdir)
    index = random.randint(0, len(filenames)-1)
    print('Setu/' + filenames[index])
