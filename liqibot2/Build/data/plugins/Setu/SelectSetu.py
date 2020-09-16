import os
import random

imgfolderdir = '../../miraiOK_Release/data/MiraiApiHttp/images/Setu/'

filenames = os.listdir(imgfolderdir)
index = random.randint(0, len(filenames)-1)
print('Setu/' + filenames[index])